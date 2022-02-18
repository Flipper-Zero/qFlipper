#include "flipperzero.h"

#include <QDebug>
#include <QLoggingCategory>

#include "preferences.h"
#include "flipperupdates.h"

#include "devicestate.h"
#include "screenstreamer.h"
#include "virtualdisplay.h"

#include "protobufsession.h"
#include "commandinterface.h"
#include "utilityinterface.h"
#include "recoveryinterface.h"

#include "toplevel/wirelessstackupdateoperation.h"
#include "toplevel/firmwareinstalloperation.h"
#include "toplevel/settingsrestoreoperation.h"
#include "toplevel/settingsbackupoperation.h"
#include "toplevel/factoryresetoperation.h"
#include "toplevel/fullrepairoperation.h"
#include "toplevel/fullupdateoperation.h"

#include "preferences.h"

#include "pixmaps/updating.h"
#include "pixmaps/updateok.h"

Q_LOGGING_CATEGORY(CAT_DEVICE, "DEVICE")

#define CHANNEL_DEVELOPMENT "development"
#define CHANNEL_RELEASE_CANDIDATE "release-candidate"
#define CHANNEL_RELEASE "release"

using namespace Flipper;
using namespace Updates;
using namespace Zero;

FlipperZero::FlipperZero(const Zero::DeviceInfo &info, QObject *parent):
    QObject(parent),
    m_state(new DeviceState(info, this)),
    m_rpc(new ProtobufSession(info.portInfo, this)),
    m_oldStuff(new CommandInterface(m_state, this)),
    m_recovery(new RecoveryInterface(m_state, this)),
    m_utility(new UtilityInterface(m_state, m_oldStuff, this)),
    m_streamer(new ScreenStreamer(m_state, m_rpc, this)),
    m_virtualDisplay(new VirtualDisplay(m_state, m_oldStuff, this))
{
    // TODO: set protobuf version

    connect(m_state, &DeviceState::isOnlineChanged, this, &FlipperZero::onIsOnlineChanged);
    connect(m_state, &DeviceState::deviceInfoChanged, this, &FlipperZero::stateChanged);

    m_rpc->startSession();
}

FlipperZero::~FlipperZero()
{
    m_state->setOnline(false);
}

DeviceState *FlipperZero::deviceState() const
{
    return m_state;
}

// TODO: Handle -rcxx suffixes correctly
bool FlipperZero::canUpdate(const Updates::VersionInfo &versionInfo) const
{
    const auto &storageInfo = m_state->deviceInfo().storage;
    const auto &radioStackVersion = m_state->deviceInfo().radioVersion;

    const auto noAssets = storageInfo.isExternalPresent && !storageInfo.isAssetsInstalled;
    const auto noRadioStack = radioStackVersion.isEmpty();

    if(noAssets || noRadioStack) {
        return true;
    }

    static const auto DEVELOPMENT = QStringLiteral("development");
    static const auto RELEASE_CANDIDATE = QStringLiteral("release-candidate");
    static const auto RELEASE = QStringLiteral("release");

    const auto &firmwareInfo = m_state->deviceInfo().firmware;

    const auto &deviceChannel = firmwareInfo.channel;
    const auto &deviceVersion = (deviceChannel == QStringLiteral("development")) ?
                firmwareInfo.commit :
                firmwareInfo.version;

    const auto &deviceDate = firmwareInfo.date;

    const auto &serverChannel = globalPrefs->firmwareUpdateChannel();
    const auto &serverVersion = versionInfo.number();
    const auto &serverDate = versionInfo.date();

    if(deviceChannel == RELEASE) {
        if(serverChannel == RELEASE) {
            return VersionInfo::compare(deviceVersion, serverVersion) < 0;
        } else if(serverChannel == RELEASE_CANDIDATE) {
            return VersionInfo::compare(deviceVersion, serverVersion) < 0;
        } else if(serverChannel == DEVELOPMENT) {
            return deviceDate <= serverDate;
        }

    } else if(deviceChannel == RELEASE_CANDIDATE) {
        if(serverChannel == RELEASE) {
            return VersionInfo::compare(deviceVersion, serverVersion) <= 0;
        } else if(serverChannel == RELEASE_CANDIDATE) {
            return VersionInfo::compare(deviceVersion, serverVersion) < 0;
        } else if(serverChannel == DEVELOPMENT) {
            return deviceDate <= serverDate;
        }

    } else if(deviceChannel == DEVELOPMENT) {
        if(serverChannel == RELEASE) {
            return deviceDate <= serverDate;
        } else if(serverChannel == RELEASE_CANDIDATE) {
            return deviceDate <= serverDate;
        } else if(serverChannel == DEVELOPMENT) {
            return (deviceVersion != serverVersion) && (deviceDate <= serverDate);
        }
    }

    return false;
}

bool FlipperZero::canInstall(const Updates::VersionInfo &versionInfo) const
{
    Q_UNUSED(versionInfo)

    const auto &deviceChannel = m_state->deviceInfo().firmware.channel;
    const auto &serverChannel = globalPrefs->firmwareUpdateChannel();

    return deviceChannel != serverChannel;
}

bool FlipperZero::canRepair(const Updates::VersionInfo &versionInfo) const
{
    Q_UNUSED(versionInfo)
    return m_state->isRecoveryMode();
}

void FlipperZero::fullUpdate(const Updates::VersionInfo &versionInfo)
{
    registerOperation(new FullUpdateOperation(m_recovery, m_utility, m_state, versionInfo, this));
}

void FlipperZero::fullRepair(const Updates::VersionInfo &versionInfo)
{
    registerOperation(new FullRepairOperation(m_recovery, m_utility, m_state, versionInfo, this));
}

void FlipperZero::createBackup(const QUrl &directoryUrl)
{
    registerOperation(new SettingsBackupOperation(m_utility, m_state, directoryUrl, this));
}

void FlipperZero::restoreBackup(const QUrl &directoryUrl)
{
    registerOperation(new SettingsRestoreOperation(m_utility, m_state, directoryUrl, this));
}

void FlipperZero::factoryReset()
{
    registerOperation(new FactoryResetOperation(m_utility, m_state, this));
}

void FlipperZero::installFirmware(const QUrl &fileUrl)
{
    registerOperation(new FirmwareInstallOperation(m_recovery, m_utility, m_state, fileUrl.toLocalFile(), this));
}

void FlipperZero::installWirelessStack(const QUrl &fileUrl)
{
    registerOperation(new WirelessStackUpdateOperation(m_recovery, m_utility, m_state, fileUrl.toLocalFile(), this));
}

void FlipperZero::installFUS(const QUrl &fileUrl, uint32_t address)
{
    registerOperation(new FUSUpdateOperation(m_recovery, m_utility, m_state, fileUrl.toLocalFile(), address, this));
}

void FlipperZero::sendInputEvent(int key, int type)
{
    m_streamer->sendInputEvent(key, type);
}

void FlipperZero::finalizeOperation()
{
    // TODO: write a better implementation that would:
    // 1. Check if the port is open and functional
    // 2. Test if the RPC session is up an running
    // 3. Open RPC session if necessary
    // 4. Start screen streaming

    if(m_state->isError()) {
        m_state->clearError();
    }

    if(!m_state->isRecoveryMode()) {
        m_virtualDisplay->stop();
        m_streamer->start();
    }
}

void FlipperZero::onIsOnlineChanged()
{
    if(!m_state->isOnline() || m_state->isError() || m_state->isRecoveryMode()) {
        return;
    } else if(!m_state->isPersistent()) {
        m_streamer->start();
    } else {
        m_virtualDisplay->start(QByteArray((char*)updating_bits, sizeof(updating_bits)));
    }
}

void FlipperZero::registerOperation(AbstractOperation *operation)
{
    connect(operation, &AbstractOperation::finished, this, [=]() {
        if(operation->isError()) {
            qCCritical(CAT_DEVICE).noquote() << operation->description() << "ERROR:" << operation->errorString();
            m_state->setError(operation->error(), operation->errorString());

        } else {
            m_virtualDisplay->sendFrame(QByteArray((char*)update_ok_bits, sizeof(update_ok_bits)));
            qCInfo(CAT_DEVICE).noquote() << operation->description() << "SUCCESS";
        }

        operation->deleteLater();
        emit operationFinished();
    });

    qCInfo(CAT_DEVICE).noquote() << operation->description() << "START";

    if(m_state->isRecoveryMode()) {
        operation->start();

    } else {
        connect(m_state, &DeviceState::isStreamingEnabledChanged, operation, [=]() {
            //TODO: Check that ScreenStreamer has stopped without errors
            if(m_state->isStreamingEnabled()) {
                // TODO: Finish with error
            } else {
                m_virtualDisplay->start(QByteArray((char*)updating_bits, sizeof(updating_bits)));
                operation->start();
            }
        });

        m_streamer->stop();
    }
}
