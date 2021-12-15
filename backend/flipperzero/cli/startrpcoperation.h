#pragma once

#include "abstractprotobufoperation.h"

namespace Flipper {
namespace Zero {

class StartRPCOperation : public AbstractProtobufOperation
{
    Q_OBJECT

    enum State {
        LeavingCli = AbstractOperation::User,
        WaitingForPing
    };

public:
    StartRPCOperation(QSerialPort *serialPort, QObject *parent = nullptr);
    const QString description() const override;

private slots:
    void onSerialPortReadyRead() override;

private:
    bool begin() override;
    static const QByteArray s_cmd;
};

}
}

