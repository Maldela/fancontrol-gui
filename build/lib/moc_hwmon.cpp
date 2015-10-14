/****************************************************************************
** Meta object code from reading C++ file 'hwmon.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/src/hwmon.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hwmon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fancontrol__Hwmon_t {
    QByteArrayData data[20];
    char stringdata0[213];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fancontrol__Hwmon_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fancontrol__Hwmon_t qt_meta_stringdata_Fancontrol__Hwmon = {
    {
QT_MOC_LITERAL(0, 0, 17), // "Fancontrol::Hwmon"
QT_MOC_LITERAL(1, 18, 11), // "fansChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "pwmFansChanged"
QT_MOC_LITERAL(4, 46, 12), // "tempsChanged"
QT_MOC_LITERAL(5, 59, 18), // "configUpdateNeeded"
QT_MOC_LITERAL(6, 78, 19), // "sensorsUpdateNeeded"
QT_MOC_LITERAL(7, 98, 12), // "errorChanged"
QT_MOC_LITERAL(8, 111, 12), // "updateConfig"
QT_MOC_LITERAL(9, 124, 13), // "updateSensors"
QT_MOC_LITERAL(10, 138, 8), // "setError"
QT_MOC_LITERAL(11, 147, 5), // "error"
QT_MOC_LITERAL(12, 153, 8), // "testFans"
QT_MOC_LITERAL(13, 162, 4), // "name"
QT_MOC_LITERAL(14, 167, 4), // "path"
QT_MOC_LITERAL(15, 172, 5), // "index"
QT_MOC_LITERAL(16, 178, 4), // "fans"
QT_MOC_LITERAL(17, 183, 15), // "QList<QObject*>"
QT_MOC_LITERAL(18, 199, 7), // "pwmFans"
QT_MOC_LITERAL(19, 207, 5) // "temps"

    },
    "Fancontrol::Hwmon\0fansChanged\0\0"
    "pwmFansChanged\0tempsChanged\0"
    "configUpdateNeeded\0sensorsUpdateNeeded\0"
    "errorChanged\0updateConfig\0updateSensors\0"
    "setError\0error\0testFans\0name\0path\0"
    "index\0fans\0QList<QObject*>\0pwmFans\0"
    "temps"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fancontrol__Hwmon[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       6,   78, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   72,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x09 /* Protected */,

 // methods: name, argc, parameters, tag, flags
      12,    0,   77,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,

 // methods: parameters
    QMetaType::Void,

 // properties: name, type, flags
      13, QMetaType::QString, 0x00095401,
      14, QMetaType::QString, 0x00095401,
      15, QMetaType::Int, 0x00095401,
      16, 0x80000000 | 17, 0x00495009,
      18, 0x80000000 | 17, 0x00495009,
      19, 0x80000000 | 17, 0x00495009,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       1,
       2,

       0        // eod
};

void Fancontrol::Hwmon::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Hwmon *_t = static_cast<Hwmon *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fansChanged(); break;
        case 1: _t->pwmFansChanged(); break;
        case 2: _t->tempsChanged(); break;
        case 3: _t->configUpdateNeeded(); break;
        case 4: _t->sensorsUpdateNeeded(); break;
        case 5: _t->errorChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updateConfig(); break;
        case 7: _t->updateSensors(); break;
        case 8: _t->setError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->testFans(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Hwmon::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::fansChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (Hwmon::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::pwmFansChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (Hwmon::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::tempsChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (Hwmon::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::configUpdateNeeded)) {
                *result = 3;
            }
        }
        {
            typedef void (Hwmon::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::sensorsUpdateNeeded)) {
                *result = 4;
            }
        }
        {
            typedef void (Hwmon::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Hwmon::errorChanged)) {
                *result = 5;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
        case 4:
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QObject*> >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Hwmon *_t = static_cast<Hwmon *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->path(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->index(); break;
        case 3: *reinterpret_cast< QList<QObject*>*>(_v) = _t->fans(); break;
        case 4: *reinterpret_cast< QList<QObject*>*>(_v) = _t->pwmFans(); break;
        case 5: *reinterpret_cast< QList<QObject*>*>(_v) = _t->temps(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fancontrol::Hwmon::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Fancontrol__Hwmon.data,
      qt_meta_data_Fancontrol__Hwmon,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fancontrol::Hwmon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fancontrol::Hwmon::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fancontrol__Hwmon.stringdata0))
        return static_cast<void*>(const_cast< Hwmon*>(this));
    return QObject::qt_metacast(_clname);
}

int Fancontrol::Hwmon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Fancontrol::Hwmon::fansChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fancontrol::Hwmon::pwmFansChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Fancontrol::Hwmon::tempsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Fancontrol::Hwmon::configUpdateNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void Fancontrol::Hwmon::sensorsUpdateNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void Fancontrol::Hwmon::errorChanged(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
