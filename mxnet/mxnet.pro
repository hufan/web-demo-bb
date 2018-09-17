#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T18:20:02
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mxnet
TEMPLATE = app

target.path = /home/sunny
INSTALLS += target

SOURCES += main.cpp\
    systemactionwidget.cpp \
    systemcontentwidget.cpp \
    mxmaindialog.cpp \
    servicedetailpage.cpp \
    serviceitem.cpp \
    servicetab.cpp \
    servicetestpage.cpp \
    serviceconfigpage.cpp

HEADERS  += \
    systemactionwidget.h \
    systemcontentwidget.h \
    mxmaindialog.h \
    servicedetailpage.h \
    serviceitem.h \
    servicetab.h \
    servicetestpage.h \
    serviceconfigpage.h

FORMS    +=
target.path = /home/myir

inst1.files = ./05_ethernet.desktop
inst1.path = /usr/share/applications
inst2.files = ./res/images/ethernet192.png
inst2.path = /usr/share/pixmaps

INSTALLS += target inst1 inst2

include(../mxbase/mxbase.pri)



DISTFILES += \
    res/images/ethernet192.png \
    05_ethernet.desktop


