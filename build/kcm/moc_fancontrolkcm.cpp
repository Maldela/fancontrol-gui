/****************************************************************************
** Meta object code from reading C++ file 'fancontrolkcm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../kcm/src/fancontrolkcm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fancontrolkcm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FancontrolKCM_t {
    QByteArrayData data[13];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FancontrolKCM_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FancontrolKCM_t qt_meta_stringdata_FancontrolKCM = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FancontrolKCM"
QT_MOC_LITERAL(1, 14, 20), // "manualControlChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "load"
QT_MOC_LITERAL(4, 41, 4), // "save"
QT_MOC_LITERAL(5, 46, 8), // "defaults"
QT_MOC_LITERAL(6, 55, 4), // "base"
QT_MOC_LITERAL(7, 60, 8), // "GUIBase*"
QT_MOC_LITERAL(8, 69, 13), // "manualControl"
QT_MOC_LITERAL(9, 83, 6), // "loader"
QT_MOC_LITERAL(10, 90, 7), // "Loader*"
QT_MOC_LITERAL(11, 98, 10), // "systemdCom"
QT_MOC_LITERAL(12, 109, 20) // "SystemdCommunicator*"

    },
    "FancontrolKCM\0manualControlChanged\0\0"
    "load\0save\0defaults\0base\0GUIBase*\0"
    "manualControl\0loader\0Loader*\0systemdCom\0"
    "SystemdCommunicator*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FancontrolKCM[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, 0x80000000 | 7, 0x00095409,
       8, QMetaType::Bool, 0x00495103,
       9, 0x80000000 | 10, 0x00095409,
      11, 0x80000000 | 12, 0x00095409,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,

       0        // eod
};

void FancontrolKCM::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FancontrolKCM *_t = static_cast<FancontrolKCM *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->manualControlChanged(); break;
        case 1: _t->load(); break;
        case 2: _t->save(); break;
        case 3: _t->defaults(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FancontrolKCM::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FancontrolKCM::manualControlChanged)) {
                *result = 0;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GUIBase* >(); break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Loader* >(); break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SystemdCommunicator* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        FancontrolKCM *_t = static_cast<FancontrolKCM *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< GUIBase**>(_v) = _t->base(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->manualControl(); break;
        case 2: *reinterpret_cast< Loader**>(_v) = _t->loader(); break;
        case 3: *reinterpret_cast< SystemdCommunicator**>(_v) = _t->systemdCommunicator(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        FancontrolKCM *_t = static_cast<FancontrolKCM *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setManualControl(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject FancontrolKCM::staticMetaObject = {
    { &ConfigModule::staticMetaObject, qt_meta_stringdata_FancontrolKCM.data,
      qt_meta_data_FancontrolKCM,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FancontrolKCM::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FancontrolKCM::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FancontrolKCM.stringdata0))
        return static_cast<void*>(const_cast< FancontrolKCM*>(this));
    return ConfigModule::qt_metacast(_clname);
}

int FancontrolKCM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ConfigModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void FancontrolKCM::manualControlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
