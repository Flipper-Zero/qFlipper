#include "startstreamoperation.h"

#include <QSerialPort>

#include "flipperzero/mainprotobufmessage.h"

using namespace Flipper;
using namespace Zero;

StartStreamOperation::StartStreamOperation(QSerialPort *serialPort, QObject *parent):
    AbstractProtobufOperation(serialPort, parent)
{}

const QString StartStreamOperation::description() const
{
    return QStringLiteral("Start Screen Streaming (protobuf) @%1").arg(serialPort()->portName());
}

void StartStreamOperation::onSerialPortReadyRead()
{
    EmptyResponse response(serialPort());

    if(!response.receive()) {
        return;
    } else if(!response.isOk()) {
        finishWithError(QStringLiteral("Device replied with an error response"));
    } else if(!response.isValidType()) {
        finishWithError(QStringLiteral("Expected empty reply, got something else"));
    } else {
        finish();
    }
}

bool StartStreamOperation::begin()
{
    GuiStartScreenStreamRequest request(serialPort());
    return request.send();
}