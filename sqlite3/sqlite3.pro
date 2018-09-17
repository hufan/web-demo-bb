QT += core
QT -= gui

CONFIG += c++11

TARGET = sqlite3
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

target.path = /home/sunny
INSTALLS += target

SOURCES += main.cpp \
    systemactionwidget.cpp \
    systemcontentwidget.cpp \
    mxmaindialog.cpp \
    sqlite3_base.cpp


HEADERS  += \
    systemactionwidget.h \
    systemcontentwidget.h \
    mxmaindialog.h \
    sqlite3_base.h

target.path = /home/myir

inst1.files = ./09_sqlite3.desktop
inst1.path = /usr/share/applications
inst2.files = ./res/images/system192.png
inst2.path = /usr/share/pixmaps

INSTALLS += target inst1 inst2

include(../mxbase/mxbase.pri)


LIBS += -l sqlite3

DISTFILES += \
    09_sqlite3.desktop
