#-------------------------------------------------
#
# Project created by QtCreator 2017-12-09T00:40:21
#
#-------------------------------------------------


QT       += widgets network testlib dbus
DBUS_INTERFACES += mxde.xml
TARGET = mxdbus
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    mxde.cpp \
    mxdbus.cpp

HEADERS += \
    mxde.h \
    mxdbus.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    mxde.xml
