#-------------------------------------------------
#
# Project created by QtCreator 2017-12-26T11:43:20
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mxcan
TEMPLATE = app



SOURCES += main.cpp \
    mxmaindialog.cpp \
    systemactionwidget.cpp \
    systemcontentwidget.cpp

HEADERS  += mxmaindialog.h \
    systemactionwidget.h \
    systemcontentwidget.h

target.path = /home/myir

inst1.files = ./04_can.desktop
inst1.path = /usr/share/applications
inst2.files = ./res/images/can192.png
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

