#include "firmwareoperations.h"

#include <QTime>
#include <QThread>

#include "flipperzero/flipperzero.h"
#include "macros.h"

namespace Flipper {
namespace Zero {

FirmwareDownloadOperation::FirmwareDownloadOperation(FlipperZero *device, QIODevice *file):
    m_device(device),
    m_file(file)
{
    m_device->setStatusMessage(QObject::tr("Firmware download pending..."));
}

FirmwareDownloadOperation::~FirmwareDownloadOperation()
{
    m_file->deleteLater();
}

const QString FirmwareDownloadOperation::name() const
{
    return QString("Firmware Download to %1 %2").arg(m_device->model(), m_device->name());
}

bool FirmwareDownloadOperation::execute()
{
    m_device->setPersistent(true);

    if(!m_device->isDFU()) {
        check_return_bool(m_device->detach(), "Failed to detach device");
    }

    check_return_bool(m_device->downloadFirmware(m_file), "Failed to upload firmware");

    m_device->setPersistent(false);
    return true;
}

WirelessStackDownloadOperation::WirelessStackDownloadOperation(FlipperZero *device, QIODevice *file, uint32_t targetAddress):
    m_device(device),
    m_file(file),
    m_targetAddress(targetAddress)
{
    m_device->setStatusMessage(QObject::tr("Wireless stack download pending..."));
}

WirelessStackDownloadOperation::~WirelessStackDownloadOperation()
{
    m_file->deleteLater();
}

const QString WirelessStackDownloadOperation::name() const
{
    return QString("Wireless Stack/FUS Download to %1 %2").arg(m_device->model(), m_device->name());
}

bool WirelessStackDownloadOperation::execute()
{
    m_device->setPersistent(true);

    if(!m_device->isDFU()) {
        check_return_bool(m_device->detach(), "Failed to detach device");
    }

    check_return_bool(m_device->setBootMode(FlipperZero::BootMode::DFUOnly), "Failed to set device into DFU-only boot mode");
    check_return_bool(m_device->startFUS(), "Failed to start FUS");
    check_return_bool(m_device->isFUSRunning(), "FUS seemed to start, but isn't running anyway");
    check_return_bool(m_device->eraseWirelessStack(), "Failed to erase existing wireless stack");
    check_return_bool(m_device->downloadWirelessStack(m_file, m_targetAddress), "Failed to download wireless stack image");
    check_return_bool(m_device->upgradeWirelessStack(), "Failed to upgrade wireless stack");
    check_return_bool(m_device->setBootMode(FlipperZero::BootMode::Normal), "Failed to set device into Normal boot mode");

    m_device->setPersistent(false);
    return true;
}

}}