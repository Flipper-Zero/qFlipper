#pragma once

#include "abstractoperation.h"

namespace Flipper {
namespace Zero {

class DeviceState;
class CommandInterface;

class AbstractUtilityOperation : public AbstractOperation
{
    Q_OBJECT

public:
    AbstractUtilityOperation(CommandInterface *cli, DeviceState *deviceState, QObject *parent = nullptr);
    virtual ~AbstractUtilityOperation() {}

    void start() override;

    CommandInterface *rpc() const;
    DeviceState *deviceState() const;

protected:
    void advanceOperationState();

private slots:
    virtual void nextStateLogic() = 0;

private:
    CommandInterface *m_rpc;
    DeviceState *m_deviceState;
};

}
}

