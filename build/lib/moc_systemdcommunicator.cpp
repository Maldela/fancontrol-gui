/****************************************************************************
** Meta object code from reading C++ file 'systemdcommunicator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/src/systemdcommunicator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemdcommunicator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fancontrol__SystemdCommunicator_t {
    QByteArrayData data[12];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fancontrol__SystemdCommunicator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fancontrol__SystemdCommunicator_t qt_meta_stringdata_Fancontrol__SystemdCommunicator = {
    {
QT_MOC_LITERAL(0, 0, 31), // "Fancontrol::SystemdCommunicator"
QT_MOC_LITERAL(1, 32, 18), // "serviceNameChanged"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 21), // "serviceEnabledChanged"
QT_MOC_LITERAL(4, 74, 20), // "serviceActiveChanged"
QT_MOC_LITERAL(5, 95, 12), // "errorChanged"
QT_MOC_LITERAL(6, 108, 23), // "updateServiceProperties"
QT_MOC_LITERAL(7, 132, 14), // "restartService"
QT_MOC_LITERAL(8, 147, 5), // "error"
QT_MOC_LITERAL(9, 153, 13), // "serviceExists"
QT_MOC_LITERAL(10, 167, 14), // "serviceEnabled"
QT_MOC_LITERAL(11, 182, 13) // "serviceActive"

    },
    "Fancontrol::SystemdCommunicator\0"
    "serviceNameChanged\0\0serviceEnabledChanged\0"
    "serviceActiveChanged\0errorChanged\0"
    "updateServiceProperties\0restartService\0"
    "error\0serviceExists\0serviceEnabled\0"
    "serviceActive"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fancontrol__SystemdCommunicator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   56, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    3,   48,    2, 0x09 /* Protected */,

 // methods: name, argc, parameters, tag, flags
       7,    0,   55,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap, QMetaType::QStringList,    2,    2,    2,

 // methods: parameters
    QMetaType::Bool,

 // properties: name, type, flags
       8, QMetaType::QString, 0x00495001,
       9, QMetaType::Bool, 0x00495001,
      10, QMetaType::Bool, 0x00495103,
      11, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       3,
       0,
       1,
       2,

       0        // eod
};

void Fancontrol::SystemdCommunicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SystemdCommunicator *_t = static_cast<SystemdCommunicator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serviceNameChanged(); break;
        case 1: _t->serviceEnabledChanged(); break;
        case 2: _t->serviceActiveChanged(); break;
        case 3: _t->errorChanged(); break;
        case 4: _t->updateServiceProperties((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 5: { bool _r = _t->restartService();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SystemdCommunicator::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemdCommunicator::serviceNameChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (SystemdCommunicator::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemdCommunicator::serviceEnabledChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (SystemdCommunicator::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemdCommunicator::serviceActiveChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (SystemdCommunicator::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SystemdCommunicator::errorChanged)) {
                *result = 3;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        SystemdCommunicator *_t = static_cast<SystemdCommunicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->error(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->serviceExists(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->serviceEnabled(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->serviceActive(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        SystemdCommunicator *_t = static_cast<SystemdCommunicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setServiceEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setServiceActive(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fancontrol::SystemdCommunicator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Fancontrol__SystemdCommunicator.data,
      qt_meta_data_Fancontrol__SystemdCommunicator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fancontrol::SystemdCommunicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fancontrol::SystemdCommunicator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fancontrol__SystemdCommunicator.stringdata0))
        return static_cast<void*>(const_cast< SystemdCommunicator*>(this));
    return QObject::qt_metacast(_clname);
}

int Fancontrol::SystemdCommunicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
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
void Fancontrol::SystemdCommunicator::serviceNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fancontrol::SystemdCommunicator::serviceEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Fancontrol::SystemdCommunicator::serviceActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Fancontrol::SystemdCommunicator::errorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
