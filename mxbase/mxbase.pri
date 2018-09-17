include(../qtsingleapplication/src/qtsingleapplication.pri)
QT *= sql
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/common
INCLUDEPATH += $$PWD/widgets
INCLUDEPATH += $$PWD/application
INCLUDEPATH += $$PWD/virtualkeyboard

SOURCES += \
        $$PWD/common/systembutton.cpp \
        $$PWD/common/mxdesktopfile.cpp \
        $$PWD/common/mxlistmodel.cpp \
        $$PWD/common/mxproperties.cpp \
        $$PWD/virtualkeyboard/frminput.cpp \
        $$PWD/widgets/basewidget.cpp \
        $$PWD/widgets/shadowwidget.cpp \
        $$PWD/application/mxapplication.cpp

HEADERS  += \
        $$PWD/common/systembutton.h \
        $$PWD/common/constant.h \
        $$PWD/common/mxdesktopfile.h \
        $$PWD/common/mxlistmodel.h \
        $$PWD/common/mxproperties.h \
        $$PWD/virtualkeyboard/frminput.h \
        $$PWD/widgets/basewidget.h \
        $$PWD/widgets/shadowwidget.h \
        $$PWD/application/mxapplication.h

FORMS += $$PWD/virtualkeyboard/frminput.ui

RESOURCES += $$PWD/mxbase.qrc
