#include "serialfinder.h"

#include <QTimer>

SerialFinder::SerialFinder(const QString &serialNumber, QObject *parent):
    QObject(parent),
    m_timer(new QTimer(this)),
    m_serialNumber(serialNumber),
    m_numTries(100),
    m_periodMs(15)
{
    connect(m_timer, &QTimer::timeout, this, &SerialFinder::findMatchingPort);

    m_timer->setSingleShot(true);
    m_timer->start(0);
}

void SerialFinder::setNumberOfTries(int numTries)
{
    m_numTries = numTries;
}

void SerialFinder::setTryPeriod(int periodMs)
{
    m_periodMs = periodMs;
}

void SerialFinder::findMatchingPort()
{
    if(!(--m_numTries)) {
        emit finished(QSerialPortInfo());
        return;
    }

    const auto portInfos = QSerialPortInfo::availablePorts();
    const auto it = std::find_if(portInfos.cbegin(), portInfos.cend(), [&](const QSerialPortInfo &info) {
        return info.serialNumber() == m_serialNumber;
    });

    if(it != portInfos.cend()) {
        emit finished(*it);
        return;
    }

    m_timer->start(m_periodMs);
}
