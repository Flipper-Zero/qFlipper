#pragma once

#include "abstractrecoveryoperation.h"

class QIODevice;

namespace Flipper {
namespace Zero {

class FirmwareDownloadOperation : public AbstractRecoveryOperation
{
    Q_OBJECT

    enum State {
        DownloadingFirmware = AbstractOperation::User
    };

public:
    FirmwareDownloadOperation(Recovery *recovery, QIODevice *file, QObject *parent = nullptr);
    ~FirmwareDownloadOperation();

    const QString description() const override;

private slots:
    void advanceOperationState() override;

private:
    void downloadFirmware();

    QIODevice *m_file;
};

}
}

