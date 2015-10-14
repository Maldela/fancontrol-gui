/****************************************************************************
** Meta object code from reading C++ file 'pwmfan.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/src/pwmfan.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pwmfan.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fancontrol__PwmFan_t {
    QByteArrayData data[31];
    char stringdata0[319];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fancontrol__PwmFan_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fancontrol__PwmFan_t qt_meta_stringdata_Fancontrol__PwmFan = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Fancontrol::PwmFan"
QT_MOC_LITERAL(1, 19, 10), // "pwmChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "tempChanged"
QT_MOC_LITERAL(4, 43, 14), // "hasTempChanged"
QT_MOC_LITERAL(5, 58, 14), // "minTempChanged"
QT_MOC_LITERAL(6, 73, 14), // "maxTempChanged"
QT_MOC_LITERAL(7, 88, 13), // "minPwmChanged"
QT_MOC_LITERAL(8, 102, 13), // "maxPwmChanged"
QT_MOC_LITERAL(9, 116, 15), // "minStartChanged"
QT_MOC_LITERAL(10, 132, 14), // "minStopChanged"
QT_MOC_LITERAL(11, 147, 13), // "activeChanged"
QT_MOC_LITERAL(12, 161, 14), // "testingChanged"
QT_MOC_LITERAL(13, 176, 14), // "pwmModeChanged"
QT_MOC_LITERAL(14, 191, 6), // "update"
QT_MOC_LITERAL(15, 198, 12), // "continueTest"
QT_MOC_LITERAL(16, 211, 4), // "test"
QT_MOC_LITERAL(17, 216, 9), // "abortTest"
QT_MOC_LITERAL(18, 226, 3), // "pwm"
QT_MOC_LITERAL(19, 230, 4), // "temp"
QT_MOC_LITERAL(20, 235, 5), // "Temp*"
QT_MOC_LITERAL(21, 241, 7), // "hasTemp"
QT_MOC_LITERAL(22, 249, 7), // "minTemp"
QT_MOC_LITERAL(23, 257, 7), // "maxTemp"
QT_MOC_LITERAL(24, 265, 6), // "minPwm"
QT_MOC_LITERAL(25, 272, 6), // "maxPwm"
QT_MOC_LITERAL(26, 279, 8), // "minStart"
QT_MOC_LITERAL(27, 288, 7), // "minStop"
QT_MOC_LITERAL(28, 296, 6), // "active"
QT_MOC_LITERAL(29, 303, 7), // "testing"
QT_MOC_LITERAL(30, 311, 7) // "pwmMode"

    },
    "Fancontrol::PwmFan\0pwmChanged\0\0"
    "tempChanged\0hasTempChanged\0minTempChanged\0"
    "maxTempChanged\0minPwmChanged\0maxPwmChanged\0"
    "minStartChanged\0minStopChanged\0"
    "activeChanged\0testingChanged\0"
    "pwmModeChanged\0update\0continueTest\0"
    "test\0abortTest\0pwm\0temp\0Temp*\0hasTemp\0"
    "minTemp\0maxTemp\0minPwm\0maxPwm\0minStart\0"
    "minStop\0active\0testing\0pwmMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fancontrol__PwmFan[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
      12,  110, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,
       4,    0,   96,    2, 0x06 /* Public */,
       5,    0,   97,    2, 0x06 /* Public */,
       6,    0,   98,    2, 0x06 /* Public */,
       7,    0,   99,    2, 0x06 /* Public */,
       8,    0,  100,    2, 0x06 /* Public */,
       9,    0,  101,    2, 0x06 /* Public */,
      10,    0,  102,    2, 0x06 /* Public */,
      11,    0,  103,    2, 0x06 /* Public */,
      12,    0,  104,    2, 0x06 /* Public */,
      13,    0,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  106,    2, 0x09 /* Protected */,
      15,    0,  107,    2, 0x09 /* Protected */,

 // methods: name, argc, parameters, tag, flags
      16,    0,  108,    2, 0x02 /* Public */,
      17,    0,  109,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void,

 // properties: name, type, flags
      18, QMetaType::Int, 0x00495103,
      19, 0x80000000 | 20, 0x0049510b,
      21, QMetaType::Bool, 0x00495103,
      22, QMetaType::Int, 0x00495103,
      23, QMetaType::Int, 0x00495103,
      24, QMetaType::Int, 0x00495103,
      25, QMetaType::Int, 0x00495103,
      26, QMetaType::Int, 0x00495103,
      27, QMetaType::Int, 0x00495103,
      28, QMetaType::Bool, 0x00495103,
      29, QMetaType::Bool, 0x00495001,
      30, QMetaType::Int, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       6,
       7,
       8,
       9,
      10,
      11,

       0        // eod
};

void Fancontrol::PwmFan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PwmFan *_t = static_cast<PwmFan *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pwmChanged(); break;
        case 1: _t->tempChanged(); break;
        case 2: _t->hasTempChanged(); break;
        case 3: _t->minTempChanged(); break;
        case 4: _t->maxTempChanged(); break;
        case 5: _t->minPwmChanged(); break;
        case 6: _t->maxPwmChanged(); break;
        case 7: _t->minStartChanged(); break;
        case 8: _t->minStopChanged(); break;
        case 9: _t->activeChanged(); break;
        case 10: _t->testingChanged(); break;
        case 11: _t->pwmModeChanged(); break;
        case 12: _t->update(); break;
        case 13: _t->continueTest(); break;
        case 14: { bool _r = _t->test();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: _t->abortTest(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::pwmChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::tempChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::hasTempChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::minTempChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::maxTempChanged)) {
                *result = 4;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::minPwmChanged)) {
                *result = 5;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::maxPwmChanged)) {
                *result = 6;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::minStartChanged)) {
                *result = 7;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::minStopChanged)) {
                *result = 8;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::activeChanged)) {
                *result = 9;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::testingChanged)) {
                *result = 10;
            }
        }
        {
            typedef void (PwmFan::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PwmFan::pwmModeChanged)) {
                *result = 11;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Temp* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        PwmFan *_t = static_cast<PwmFan *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->pwm(); break;
        case 1: *reinterpret_cast< Temp**>(_v) = _t->temp(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->hasTemp(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->minTemp(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->maxTemp(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->minPwm(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->maxPwm(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->minStart(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->minStop(); break;
        case 9: *reinterpret_cast< bool*>(_v) = _t->active(); break;
        case 10: *reinterpret_cast< bool*>(_v) = _t->testing(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->pwmMode(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        PwmFan *_t = static_cast<PwmFan *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPwm(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setTemp(*reinterpret_cast< Temp**>(_v)); break;
        case 2: _t->setHasTemp(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setMinTemp(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setMaxTemp(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setMinPwm(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setMaxPwm(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setMinStart(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setMinStop(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setActive(*reinterpret_cast< bool*>(_v)); break;
        case 11: _t->setPwmMode(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fancontrol::PwmFan::staticMetaObject = {
    { &Fan::staticMetaObject, qt_meta_stringdata_Fancontrol__PwmFan.data,
      qt_meta_data_Fancontrol__PwmFan,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fancontrol::PwmFan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fancontrol::PwmFan::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fancontrol__PwmFan.stringdata0))
        return static_cast<void*>(const_cast< PwmFan*>(this));
    return Fan::qt_metacast(_clname);
}

int Fancontrol::PwmFan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Fan::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 12;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Fancontrol::PwmFan::pwmChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fancontrol::PwmFan::tempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Fancontrol::PwmFan::hasTempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Fancontrol::PwmFan::minTempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void Fancontrol::PwmFan::maxTempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void Fancontrol::PwmFan::minPwmChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void Fancontrol::PwmFan::maxPwmChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void Fancontrol::PwmFan::minStartChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void Fancontrol::PwmFan::minStopChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void Fancontrol::PwmFan::activeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}

// SIGNAL 10
void Fancontrol::PwmFan::testingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, Q_NULLPTR);
}

// SIGNAL 11
void Fancontrol::PwmFan::pwmModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
