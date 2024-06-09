#pragma once

#include <QByteArray>

#include <flipper.pb.h>

#include "bandinfo.h"

class RegionData
{
public:
    RegionData(const QByteArray &countryCode, const BandInfoList &bands);
    ~RegionData();

    const QByteArray encode() const;

private:
    PB_Region m_message;
    BandInfoList m_bands;
};

