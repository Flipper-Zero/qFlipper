#pragma once

#include <QObject>
#include <QByteArray>

#include "abstractoperationhelper.h"
#include "flipperzero/deviceinfo.h"

class QTimer;
class QSerialPort;

namespace Flipper {
namespace Zero {

class ProtobufSession;

class AbstractDeviceInfoHelper : public AbstractOperationHelper
{
    Q_OBJECT

public:
    AbstractDeviceInfoHelper(QObject *parent = nullptr);
    virtual ~AbstractDeviceInfoHelper();

    static AbstractDeviceInfoHelper *create(const USBDeviceInfo &info, QObject *parent = nullptr);
    const DeviceInfo &result() const;

protected:
    DeviceInfo m_deviceInfo;
};

class VCPDeviceInfoHelper : public AbstractDeviceInfoHelper
{
    Q_OBJECT

    enum OperationState {
        FindingSerialPort = AbstractOperationHelper::User,
        StartingRPCSession,
        FetchingDeviceInfo,
        CheckingSDCard,
        CheckingManifest,
        GettingTimeSkew,
        SyncingTime,
        StoppingRPCSession
    };

public:
    VCPDeviceInfoHelper(const USBDeviceInfo &info, QObject *parent = nullptr);

private:
    void nextStateLogic() override;

    void findSerialPort();
    void startRPCSession();
    void fetchDeviceInfo();
    void checkSDCard();
    void checkManifest();
    void getTimeSkew();
    void syncTime();
    void stopRPCSession();

private slots:
    void onRPCSessionStateChanged();

private:
    static const QString &branchToChannelName(const QByteArray &branchName);
    ProtobufSession *m_session;
};

class DFUDeviceInfoHelper : public AbstractDeviceInfoHelper
{
    Q_OBJECT

public:
    DFUDeviceInfoHelper(const USBDeviceInfo &info, QObject *parent = nullptr);

private:
    void nextStateLogic() override;
};

}
}
