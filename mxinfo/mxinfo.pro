#-------------------------------------------------
#
# Project created by QtCreator 2018-01-18T11:04:04
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mxinfo
TEMPLATE = app

SOURCES += main.cpp\
    mxmaindialog.cpp \
    systemactionwidget.cpp \
    systemcontentwidget.cpp

HEADERS  += \
    mxmaindialog.h \
    systemactionwidget.h \
    systemcontentwidget.h

target.path = /home/myir

inst1.files = ./08_system.desktop
inst1.path = /usr/share/applications
inst2.files = ./res/images/system192.png
inst2.path = /usr/share/pixmaps

INSTALLS += target inst1 inst2

include(../mxbase/mxbase.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../mxdbus/release/ -lmxdbus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../mxdbus/debug/ -lmxdbus
else:unix:!macx: LIBS += -L$$OUT_PWD/../mxdbus/ -lmxdbus

INCLUDEPATH += $$PWD/../mxdbus
DEPENDPATH += $$PWD/../mxdbus

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../mxdbus/release/libmxdbus.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../mxdbus/debug/libmxdbus.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../mxdbus/release/mxdbus.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../mxdbus/debug/mxdbus.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../mxdbus/libmxdbus.a

DISTFILES += \
    res/hardware/audio.png \
    res/hardware/battery.png \
    res/hardware/board.png \
    res/hardware/cdrom.png \
    res/hardware/cpu.png \
    res/hardware/drive.png \
    res/hardware/memory.png \
    res/hardware/monitor.png \
    res/hardware/network.png \
    res/hardware/sensor.png \
    res/hardware/unity.png \
    res/hardware/system.png \
    res/hardware/storage.png \
    08_system.desktop

RESOURCES += \
    img.qrc
