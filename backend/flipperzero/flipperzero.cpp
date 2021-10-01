#include "flipperzero.h"

#include <QSerialPort>

#include "recovery.h"
#include "recoveryinterface.h"
#include "commandinterface.h"
#include "screenstreaminterface.h"

#include "macros.h"

namespace Flipper {

using namespace Zero;

FlipperZero::FlipperZero(const Zero::DeviceInfo &info, QObject *parent):
    QObject(parent),

    m_isPersistent(false),
    m_isOnline(true),
    m_isError(false),

    m_deviceInfo(info),

    m_progress(0),
    m_screen(nullptr),
    m_recoveryOld(nullptr),
    m_recovery(nullptr),
    m_cli(nullptr)
{
    initInterfaces();
}

FlipperZero::~FlipperZero()
{
    setOnline(false);
}

void FlipperZero::reset(const Zero::DeviceInfo &info)
{
    setDeviceInfo(info);
    initInterfaces();

    setError(QStringLiteral("No error"), false);
    setProgress(0);
    setOnline(true);
}

void FlipperZero::setDeviceInfo(const Zero::DeviceInfo &info)
{
    // Not checking the huge structure for equality
    m_deviceInfo = info;
    emit deviceInfoChanged();
}

void FlipperZero::setPersistent(bool set)
{
    if(set == m_isPersistent) {
        return;
    }

    m_isPersistent = set;
    emit isPersistentChanged();
}

void FlipperZero::setOnline(bool set)
{
    if(set == m_isOnline) {
        return;
    }

    m_isOnline = set;
    emit isOnlineChanged();
}

void FlipperZero::setError(const QString &msg, bool set)
{
    m_isError = set;

    if(!msg.isEmpty()) {
        error_msg(msg);
        m_errorString = msg;
    }

    emit isErrorChanged();
}

bool FlipperZero::isPersistent() const
{
    return m_isPersistent;
}

bool FlipperZero::isOnline() const
{
    return m_isOnline;
}

bool FlipperZero::isError() const
{
    return m_isError;
}

bool FlipperZero::bootToDFU()
{
    setMessage("Entering DFU bootloader mode...");

    auto *serialPort = new QSerialPort(m_deviceInfo.serialInfo, this);

    const auto success = serialPort->open(QIODevice::WriteOnly) && serialPort->setDataTerminalReady(true) &&
                        (serialPort->write(QByteArrayLiteral("\rdfu\r\n")) > 0) && serialPort->waitForBytesWritten(1000);
    if(!success) {
        setError("Can't detach the device: Failed to reset in DFU mode");
        error_msg(QString("Serial port status: %1").arg(serialPort->errorString()));
    }

    serialPort->close();
    serialPort->deleteLater();

    return success;
}

const QString &FlipperZero::name() const
{
    return m_deviceInfo.name;
}

const QString &FlipperZero::model() const
{
    static const QString m = "Flipper Zero";
    return m;
}

const QString &FlipperZero::target() const
{
    return m_deviceInfo.target;
}

const QString &FlipperZero::version() const
{
    if(m_deviceInfo.firmware.branch == QStringLiteral("dev")) {
        return m_deviceInfo.firmware.commit;
    } else {
        return m_deviceInfo.firmware.version;
    }
}

const QString &FlipperZero::messageString() const
{
    return m_statusMessage;
}

const QString &FlipperZero::errorString() const
{
    return m_errorString;
}

double FlipperZero::progress() const
{
    return m_progress;
}

const DeviceInfo &FlipperZero::deviceInfo() const
{
    return m_deviceInfo;
}

bool FlipperZero::isDFU() const
{
    return m_deviceInfo.usbInfo.productID() == 0xdf11;
}

Flipper::Zero::ScreenStreamInterface *FlipperZero::screen() const
{
    return m_screen;
}

Recovery *FlipperZero::recovery() const
{
    return m_recoveryOld;
}

RecoveryInterface *FlipperZero::recoveryNew() const
{
    return m_recovery;
}

CommandInterface *FlipperZero::cli() const
{
    return m_cli;
}

void FlipperZero::setMessage(const QString &message)
{
    info_msg(message);
    m_statusMessage = message;
    emit messageChanged();
}

void FlipperZero::setProgress(double progress)
{
    if(qFuzzyCompare(m_progress, progress)) {
        return;
    }

    m_progress = progress;
    emit progressChanged();
}

void FlipperZero::onInterfaceErrorOccured()
{
    auto *controller = qobject_cast<SignalingFailable*>(sender());

    if(!controller) {
        return;
    }

    setError(controller->errorString());
}

void FlipperZero::initInterfaces()
{
    if(m_screen) {
       m_screen->deleteLater();
       m_screen = nullptr;
    }

    if(m_recovery) {
       m_recovery->deleteLater();
       m_recovery = nullptr;
    }

    if(m_cli) {
       m_cli->deleteLater();
       m_cli = nullptr;
    }

    // TODO: better message delivery system
    if(isDFU()) {
        m_recovery = new RecoveryInterface(m_deviceInfo.usbInfo, this);

//        connect(m_recoveryOld, &Recovery::messageChanged, this, [=]() {
//            setMessage(m_recoveryOld->message());
//        });

//        connect(m_recoveryOld, &Recovery::progressChanged, this, [=]() {
//            setProgress(m_recoveryOld->progress());
//        });

        connect(m_recovery, &SignalingFailable::errorOccured, this, &FlipperZero::onInterfaceErrorOccured);

    } else {
        m_screen = new ScreenStreamInterface(m_deviceInfo.serialInfo, this);
        m_cli = new CommandInterface(m_deviceInfo.serialInfo, this);

        connect(m_cli, &SignalingFailable::errorOccured, this, &FlipperZero::onInterfaceErrorOccured);
    }
}

}
