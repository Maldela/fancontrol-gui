/****************************************************************************
** Meta object code from reading C++ file 'sensor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/src/sensor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fancontrol__Sensor_t {
    QByteArrayData data[10];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fancontrol__Sensor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fancontrol__Sensor_t qt_meta_stringdata_Fancontrol__Sensor = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Fancontrol::Sensor"
QT_MOC_LITERAL(1, 19, 11), // "nameChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "errorChanged"
QT_MOC_LITERAL(4, 45, 6), // "update"
QT_MOC_LITERAL(5, 52, 5), // "index"
QT_MOC_LITERAL(6, 58, 4), // "name"
QT_MOC_LITERAL(7, 63, 4), // "path"
QT_MOC_LITERAL(8, 68, 6), // "parent"
QT_MOC_LITERAL(9, 75, 6) // "Hwmon*"

    },
    "Fancontrol::Sensor\0nameChanged\0\0"
    "errorChanged\0update\0index\0name\0path\0"
    "parent\0Hwmon*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fancontrol__Sensor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       4,   34, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       5, QMetaType::UInt, 0x00095401,
       6, QMetaType::QString, 0x00495103,
       7, QMetaType::QString, 0x00095401,
       8, 0x80000000 | 9, 0x00095409,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,

       0        // eod
};

void Fancontrol::Sensor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Sensor *_t = static_cast<Sensor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nameChanged(); break;
        case 1: _t->errorChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->update(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Sensor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Sensor::nameChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (Sensor::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Sensor::errorChanged)) {
                *result = 1;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Sensor *_t = static_cast<Sensor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< uint*>(_v) = _t->index(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->path(); break;
        case 3: *reinterpret_cast< Hwmon**>(_v) = _t->parent(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Sensor *_t = static_cast<Sensor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setName(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fancontrol::Sensor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Fancontrol__Sensor.data,
      qt_meta_data_Fancontrol__Sensor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fancontrol::Sensor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fancontrol::Sensor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fancontrol__Sensor.stringdata0))
        return static_cast<void*>(const_cast< Sensor*>(this));
    return QObject::qt_metacast(_clname);
}

int Fancontrol::Sensor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
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
void Fancontrol::Sensor::nameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fancontrol::Sensor::errorChanged(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
