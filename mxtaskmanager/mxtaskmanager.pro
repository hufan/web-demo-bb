#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T15:24:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mxtaskmanager
TEMPLATE = app


SOURCES +=\
    systemactionwidget.cpp \
    mxmaindialog.cpp \
    systemcontentwidget.cpp \
    taskinfo.cpp \
    main.cpp \
    miscinfo.cpp

HEADERS  += \
    mxmaindialog.h \
    systemactionwidget.h \
    systemcontentwidget.h \
    taskinfo.h \
    miscinfo.h

target.path = /home/myir

inst1.files = ./06_task_manager.desktop
inst1.path = /usr/share/applications
inst2.files = ./res/images/taskmgr192.png
inst2.path = /usr/share/pixmaps
INSTALLS += target inst1 inst2
include(../mxbase/mxbase.pri)
SOURCES +=

HEADERS  +=

FORMS    +=

DISTFILES += \
    06_task_manager.desktop \
    res/images/taskmgr192.png
