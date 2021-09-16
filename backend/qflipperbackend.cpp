#include "qflipperbackend.h"

#include "flipperzero/flipperzero.h"
#include "flipperzero/assetmanifest.h"
#include "flipperzero/remotecontroller.h"
#include "flipperzero/recoverycontroller.h"

#include "macros.h"

using namespace Flipper;

QFlipperBackend::QFlipperBackend():
    firmwareUpdates("https://update.flipperzero.one/firmware/directory.json"),
    applicationUpdates("https://update.flipperzero.one/qFlipper/directory.json")
{
    qRegisterMetaType<Flipper::Updates::FileInfo>("Flipper::Updates::FileInfo");
    qRegisterMetaType<Flipper::Updates::VersionInfo>("Flipper::Updates::VersionInfo");
    qRegisterMetaType<Flipper::Updates::ChannelInfo>("Flipper::Updates::ChannelInfo");
    qRegisterMetaType<Flipper::Zero::RemoteController*>("Flipper::Zero::RemoteController*");
    qRegisterMetaType<Flipper::Zero::RecoveryController*>("Flipper::Zero::RecoveryController*");

    qRegisterMetaType<Flipper::Zero::AssetManifest::FileInfo>();
    QMetaType::registerComparators<Flipper::Zero::AssetManifest::FileInfo>();
}

QFlipperBackend::~QFlipperBackend()
{}
