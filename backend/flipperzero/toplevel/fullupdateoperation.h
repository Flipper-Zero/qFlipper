#pragma once

#include "abstracttopleveloperation.h"

#include <QMap>
#include <QFile>

#include "flipperupdates.h"

namespace Flipper {
namespace Zero {

class FirmwareHelper;
class UtilityInterface;
class RecoveryInterface;

class FullUpdateOperation : public AbstractTopLevelOperation
{
    Q_OBJECT

    enum OperationState {
        FetchingFirmware = AbstractOperation::User,
        SavingBackup,
        StartingRecovery,
        SettingBootMode,
        DownloadingRadioFirmware,
        DownloadingFirmware,
        CorrectingOptionBytes,
        ExitingRecovery,
        DownloadingAssets,
        RestoringBackup,
        RestartingDevice
    };

public:
    FullUpdateOperation(RecoveryInterface *recovery, UtilityInterface *utility, DeviceState *state, const Updates::VersionInfo &versionInfo, QObject *parent = nullptr);
    const QString description() const override;

private slots:
    void nextStateLogic() override;

private:
    void fetchFirmware();
    void saveBackup();
    void startRecovery();
    void setBootMode();
    void downloadRadioFirmware();
    void downloadFirmware();
    void correctOptionBytes();
    void exitRecovery();
    void downloadAssets();
    void restoreBackup();
    void restartDevice();

    RecoveryInterface *m_recovery;
    UtilityInterface *m_utility;
    FirmwareHelper *m_helper;

    Updates::VersionInfo m_versionInfo;
};

}
}

