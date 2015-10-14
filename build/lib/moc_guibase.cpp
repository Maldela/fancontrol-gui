/****************************************************************************
** Meta object code from reading C++ file 'guibase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../lib/src/guibase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guibase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fancontrol__GUIBase_t {
    QByteArrayData data[15];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Fancontrol__GUIBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Fancontrol__GUIBase_t qt_meta_stringdata_Fancontrol__GUIBase = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Fancontrol::GUIBase"
QT_MOC_LITERAL(1, 20, 14), // "minTempChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "maxTempChanged"
QT_MOC_LITERAL(4, 51, 18), // "serviceNameChanged"
QT_MOC_LITERAL(5, 70, 11), // "unitChanged"
QT_MOC_LITERAL(6, 82, 22), // "hasSystemdCommunicator"
QT_MOC_LITERAL(7, 105, 6), // "loader"
QT_MOC_LITERAL(8, 112, 7), // "Loader*"
QT_MOC_LITERAL(9, 120, 10), // "systemdCom"
QT_MOC_LITERAL(10, 131, 20), // "SystemdCommunicator*"
QT_MOC_LITERAL(11, 152, 7), // "minTemp"
QT_MOC_LITERAL(12, 160, 7), // "maxTemp"
QT_MOC_LITERAL(13, 168, 4), // "unit"
QT_MOC_LITERAL(14, 173, 11) // "serviceName"

    },
    "Fancontrol::GUIBase\0minTempChanged\0\0"
    "maxTempChanged\0serviceNameChanged\0"
    "unitChanged\0hasSystemdCommunicator\0"
    "loader\0Loader*\0systemdCom\0"
    "SystemdCommunicator*\0minTemp\0maxTemp\0"
    "unit\0serviceName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fancontrol__GUIBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       6,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       6,    0,   43,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool,

 // properties: name, type, flags
       7, 0x80000000 | 8, 0x00095409,
       9, 0x80000000 | 10, 0x00095409,
      11, QMetaType::QReal, 0x00495103,
      12, QMetaType::QReal, 0x00495103,
      13, QMetaType::Int, 0x00495103,
      14, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       0,
       0,
       1,
       3,
       2,

       0        // eod
};

void Fancontrol::GUIBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GUIBase *_t = static_cast<GUIBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->minTempChanged(); break;
        case 1: _t->maxTempChanged(); break;
        case 2: _t->serviceNameChanged(); break;
        case 3: _t->unitChanged(); break;
        case 4: { bool _r = _t->hasSystemdCommunicator();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GUIBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIBase::minTempChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (GUIBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIBase::maxTempChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (GUIBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIBase::serviceNameChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (GUIBase::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIBase::unitChanged)) {
                *result = 3;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Loader* >(); break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SystemdCommunicator* >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        GUIBase *_t = static_cast<GUIBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< Loader**>(_v) = _t->loader(); break;
        case 1: *reinterpret_cast< SystemdCommunicator**>(_v) = _t->systemdCommunicator(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->minTemp(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->maxTemp(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->unit(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->serviceName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        GUIBase *_t = static_cast<GUIBase *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setMinTemp(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setMaxTemp(*reinterpret_cast< qreal*>(_v)); break;
        case 4: _t->setUnit(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setServiceName(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject Fancontrol::GUIBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Fancontrol__GUIBase.data,
      qt_meta_data_Fancontrol__GUIBase,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Fancontrol::GUIBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fancontrol::GUIBase::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Fancontrol__GUIBase.stringdata0))
        return static_cast<void*>(const_cast< GUIBase*>(this));
    return QObject::qt_metacast(_clname);
}

int Fancontrol::GUIBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
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
void Fancontrol::GUIBase::minTempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Fancontrol::GUIBase::maxTempChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Fancontrol::GUIBase::serviceNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void Fancontrol::GUIBase::unitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
