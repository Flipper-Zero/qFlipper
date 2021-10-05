#include "abstracttopleveloperation.h"

#include <QTimer>

#include "flipperzero/devicestate.h"

using namespace Flipper;
using namespace Zero;

AbstractTopLevelOperation::AbstractTopLevelOperation(DeviceState *deviceState, QObject *parent):
    AbstractOperation(parent),
    m_deviceState(deviceState)
{}

DeviceState *AbstractTopLevelOperation::deviceState() const
{
    return m_deviceState;
}

void AbstractTopLevelOperation::start()
{
    if(operationState() != AbstractOperation::Ready) {
        finishWithError(QStringLiteral("Trying to start an operation that is either already running or has finished."));
    } else {
        advanceOperationState();
    }
}

void AbstractTopLevelOperation::advanceOperationState()
{
    QTimer::singleShot(0, this, &AbstractTopLevelOperation::nextStateLogic);
}
