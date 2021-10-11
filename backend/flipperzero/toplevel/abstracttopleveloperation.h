#pragma once

#include "abstractoperation.h"

namespace Flipper {
namespace Zero {

class DeviceState;

class AbstractTopLevelOperation : public AbstractOperation
{
    Q_OBJECT

public:
    AbstractTopLevelOperation(DeviceState *deviceState, QObject *parent = nullptr);
    virtual ~AbstractTopLevelOperation();

    DeviceState *deviceState() const;

    void start() override;

protected:
    void advanceOperationState();
    void registerOperation(AbstractOperation *operation);

private slots:
    virtual void nextStateLogic() = 0;

private:
    virtual void onSubOperationErrorOccured();

    DeviceState *m_deviceState;
};

}
}

