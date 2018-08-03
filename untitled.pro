#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T16:50:34
#
#-------------------------------------------------

QT       += core gui

QT       += sql

LIBS += -l sqlite3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dataview.cpp \
    sqlite3_base.cpp \
    publicfile.cpp

HEADERS  += mainwindow.h \
    dataview.h \
    sqlite3_base.h \
    publicfile.h

FORMS    += mainwindow.ui \
    dataview.ui


LIBS += -L/usr/lib/libsqlite3.so.0.8.6
LIBS += -L/usr/lib/libsqlite3.so.0

unix {
    target.path = /usr/lib
    INSTALLS += target
}
