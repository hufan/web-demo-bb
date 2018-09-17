#-------------------------------------------------
#
# Project created by QtCreator 2017-12-09T02:42:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mxapp
TEMPLATE = app

target.path = /home/myir/
INSTALLS += target
include(../mxbase/mxbase.pri)

SOURCES += main.cpp\
        mxmaindialog.cpp \
        homeactionwidget.cpp \
        homecontentwidget.cpp \
        boxactionwidget.cpp \
        boxcontentwidget.cpp

HEADERS  += mxmaindialog.h \
            homeactionwidget.h \
            homeactionwidget.h \
            homecontentwidget.h \
            boxactionwidget.h \
            boxcontentwidget.h

RESOURCES += \
    main.qrc

