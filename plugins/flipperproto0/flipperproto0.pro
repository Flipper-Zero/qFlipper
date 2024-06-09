QT -= gui

include(../../qflipper_common.pri)

win32: TARGET = flipperproto
else: TARGET = flipperproto0

DESTDIR = $$OUT_PWD/..

TEMPLATE = lib
CONFIG += plugin c++11

VERSION = 0.0.0

INCLUDEPATH += $$PWD/../protobufinterface \
    $$PWD/../../3rdparty/nanopb \
    $$PWD/flipperzero-protobuf-compiled

HEADERS += \
    flipperzero-protobuf-compiled/application.pb.h \
    flipperzero-protobuf-compiled/desktop.pb.h \
    flipperzero-protobuf-compiled/flipper.pb.h \
    flipperzero-protobuf-compiled/gui.pb.h \
    flipperzero-protobuf-compiled/property.pb.h \
    flipperzero-protobuf-compiled/status.pb.h \
    flipperzero-protobuf-compiled/storage.pb.h \
    flipperzero-protobuf-compiled/system.pb.h \
    guirequest.h \
    guiresponse.h \
    mainrequest.h \
    mainresponse.h \
    messagewrapper.h \
    propertyrequest.h \
    propertyresponse.h \
    protobufplugin.h \
    regiondata.h \
    statusrequest.h \
    statusresponse.h \
    storagerequest.h \
    storageresponse.h \
    systemrequest.h \
    systemresponse.h

SOURCES += \
    flipperzero-protobuf-compiled/application.pb.c \
    flipperzero-protobuf-compiled/desktop.pb.c \
    flipperzero-protobuf-compiled/flipper.pb.c \
    flipperzero-protobuf-compiled/gpio.pb.c \
    flipperzero-protobuf-compiled/gui.pb.c \
    flipperzero-protobuf-compiled/property.pb.c \
    flipperzero-protobuf-compiled/status.pb.c \
    flipperzero-protobuf-compiled/storage.pb.c \
    flipperzero-protobuf-compiled/system.pb.c \
    guirequest.cpp \
    guiresponse.cpp \
    mainrequest.cpp \
    mainresponse.cpp \
    messagewrapper.cpp \
    propertyrequest.cpp \
    propertyresponse.cpp \
    protobufplugin.cpp \
    regiondata.cpp \
    statusrequest.cpp \
    statusresponse.cpp \
    storagerequest.cpp \
    storageresponse.cpp \
    systemrequest.cpp \
    systemresponse.cpp

unix|win32 {
    LIBS += -L$$OUT_PWD/../../3rdparty/ -l3rdparty
}

DEFINES += PB_ENABLE_MALLOC

!contains(CONFIG, static) {
    unix:!macx {
        target.path = $$PREFIX/lib/$$NAME/plugins
    } else:macx {
        target.path = $$DESTDIR/../$${NAME}.app/Contents/PlugIns
    } else:win32 {
        target.path = $$DESTDIR/../$$NAME/plugins
    }

    INSTALLS += target
}

