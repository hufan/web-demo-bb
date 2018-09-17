/****************************************************************************
** Meta object code from reading C++ file 'mxde_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mxde_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mxde_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface_t {
    QByteArrayData data[27];
    char stringdata0[389];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface_t qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface = {
    {
QT_MOC_LITERAL(0, 0, 37), // "ComMyirtechMxdeMxdeInterfaceI..."
QT_MOC_LITERAL(1, 38, 23), // "sigGpioDirectionChanged"
QT_MOC_LITERAL(2, 62, 0), // ""
QT_MOC_LITERAL(3, 63, 7), // "message"
QT_MOC_LITERAL(4, 71, 19), // "sigGpioLevelChanged"
QT_MOC_LITERAL(5, 91, 23), // "sigLedBrightnessChanged"
QT_MOC_LITERAL(6, 115, 17), // "sigLedDutyChanged"
QT_MOC_LITERAL(7, 133, 20), // "sigLedTriggerChanged"
QT_MOC_LITERAL(8, 154, 11), // "getGpioList"
QT_MOC_LITERAL(9, 166, 26), // "QDBusPendingReply<QString>"
QT_MOC_LITERAL(10, 193, 10), // "getLedList"
QT_MOC_LITERAL(11, 204, 16), // "setGpioDirection"
QT_MOC_LITERAL(12, 221, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(13, 241, 4), // "gpio"
QT_MOC_LITERAL(14, 246, 9), // "direction"
QT_MOC_LITERAL(15, 256, 13), // "setGpioExport"
QT_MOC_LITERAL(16, 270, 12), // "setGpioLevel"
QT_MOC_LITERAL(17, 283, 5), // "value"
QT_MOC_LITERAL(18, 289, 15), // "setGpioUnexport"
QT_MOC_LITERAL(19, 305, 16), // "setLedBrightress"
QT_MOC_LITERAL(20, 322, 3), // "led"
QT_MOC_LITERAL(21, 326, 10), // "brightness"
QT_MOC_LITERAL(22, 337, 10), // "setLedDuty"
QT_MOC_LITERAL(23, 348, 8), // "delay_on"
QT_MOC_LITERAL(24, 357, 9), // "delay_off"
QT_MOC_LITERAL(25, 367, 13), // "setLedTrigger"
QT_MOC_LITERAL(26, 381, 7) // "trigger"

    },
    "ComMyirtechMxdeMxdeInterfaceInterface\0"
    "sigGpioDirectionChanged\0\0message\0"
    "sigGpioLevelChanged\0sigLedBrightnessChanged\0"
    "sigLedDutyChanged\0sigLedTriggerChanged\0"
    "getGpioList\0QDBusPendingReply<QString>\0"
    "getLedList\0setGpioDirection\0"
    "QDBusPendingReply<>\0gpio\0direction\0"
    "setGpioExport\0setGpioLevel\0value\0"
    "setGpioUnexport\0setLedBrightress\0led\0"
    "brightness\0setLedDuty\0delay_on\0delay_off\0"
    "setLedTrigger\0trigger"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ComMyirtechMxdeMxdeInterfaceInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       5,    1,   90,    2, 0x06 /* Public */,
       6,    1,   93,    2, 0x06 /* Public */,
       7,    1,   96,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   99,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    2,  101,    2, 0x0a /* Public */,
      15,    1,  106,    2, 0x0a /* Public */,
      16,    2,  109,    2, 0x0a /* Public */,
      18,    1,  114,    2, 0x0a /* Public */,
      19,    2,  117,    2, 0x0a /* Public */,
      22,    3,  122,    2, 0x0a /* Public */,
      25,    2,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    0x80000000 | 9,
    0x80000000 | 9,
    0x80000000 | 12, QMetaType::Int, QMetaType::QString,   13,   14,
    0x80000000 | 12, QMetaType::Int,   13,
    0x80000000 | 12, QMetaType::Int, QMetaType::Int,   13,   17,
    0x80000000 | 12, QMetaType::Int,   13,
    0x80000000 | 12, QMetaType::QString, QMetaType::Int,   20,   21,
    0x80000000 | 12, QMetaType::QString, QMetaType::Int, QMetaType::Int,   20,   23,   24,
    0x80000000 | 12, QMetaType::QString, QMetaType::QString,   20,   26,

       0        // eod
};

void ComMyirtechMxdeMxdeInterfaceInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ComMyirtechMxdeMxdeInterfaceInterface *_t = static_cast<ComMyirtechMxdeMxdeInterfaceInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigGpioDirectionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->sigGpioLevelChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->sigLedBrightnessChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->sigLedDutyChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sigLedTriggerChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: { QDBusPendingReply<QString> _r = _t->getGpioList();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QString>*>(_a[0]) = _r; }  break;
        case 6: { QDBusPendingReply<QString> _r = _t->getLedList();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QString>*>(_a[0]) = _r; }  break;
        case 7: { QDBusPendingReply<> _r = _t->setGpioDirection((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 8: { QDBusPendingReply<> _r = _t->setGpioExport((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 9: { QDBusPendingReply<> _r = _t->setGpioLevel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 10: { QDBusPendingReply<> _r = _t->setGpioUnexport((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 11: { QDBusPendingReply<> _r = _t->setLedBrightress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 12: { QDBusPendingReply<> _r = _t->setLedDuty((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 13: { QDBusPendingReply<> _r = _t->setLedTrigger((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ComMyirtechMxdeMxdeInterfaceInterface::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ComMyirtechMxdeMxdeInterfaceInterface::sigGpioDirectionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ComMyirtechMxdeMxdeInterfaceInterface::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ComMyirtechMxdeMxdeInterfaceInterface::sigGpioLevelChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ComMyirtechMxdeMxdeInterfaceInterface::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ComMyirtechMxdeMxdeInterfaceInterface::sigLedBrightnessChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ComMyirtechMxdeMxdeInterfaceInterface::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ComMyirtechMxdeMxdeInterfaceInterface::sigLedDutyChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ComMyirtechMxdeMxdeInterfaceInterface::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ComMyirtechMxdeMxdeInterfaceInterface::sigLedTriggerChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject ComMyirtechMxdeMxdeInterfaceInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface.data,
      qt_meta_data_ComMyirtechMxdeMxdeInterfaceInterface,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ComMyirtechMxdeMxdeInterfaceInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ComMyirtechMxdeMxdeInterfaceInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ComMyirtechMxdeMxdeInterfaceInterface.stringdata0))
        return static_cast<void*>(const_cast< ComMyirtechMxdeMxdeInterfaceInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int ComMyirtechMxdeMxdeInterfaceInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ComMyirtechMxdeMxdeInterfaceInterface::sigGpioDirectionChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ComMyirtechMxdeMxdeInterfaceInterface::sigGpioLevelChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ComMyirtechMxdeMxdeInterfaceInterface::sigLedBrightnessChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ComMyirtechMxdeMxdeInterfaceInterface::sigLedDutyChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ComMyirtechMxdeMxdeInterfaceInterface::sigLedTriggerChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
