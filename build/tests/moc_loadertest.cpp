/****************************************************************************
** Meta object code from reading C++ file 'loadertest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tests/loadertest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loadertest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LoaderTest_t {
    QByteArrayData data[10];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoaderTest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoaderTest_t qt_meta_stringdata_LoaderTest = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LoaderTest"
QT_MOC_LITERAL(1, 11, 12), // "initTestCase"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 15), // "cleanupTestCase"
QT_MOC_LITERAL(4, 41, 4), // "init"
QT_MOC_LITERAL(5, 46, 7), // "cleanup"
QT_MOC_LITERAL(6, 54, 19), // "getEntryNumbersTest"
QT_MOC_LITERAL(7, 74, 24), // "getEntryNumbersTest_data"
QT_MOC_LITERAL(8, 99, 19), // "parseConfigLineTest"
QT_MOC_LITERAL(9, 119, 24) // "parseConfigLineTest_data"

    },
    "LoaderTest\0initTestCase\0\0cleanupTestCase\0"
    "init\0cleanup\0getEntryNumbersTest\0"
    "getEntryNumbersTest_data\0parseConfigLineTest\0"
    "parseConfigLineTest_data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoaderTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoaderTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoaderTest *_t = static_cast<LoaderTest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->initTestCase(); break;
        case 1: _t->cleanupTestCase(); break;
        case 2: _t->init(); break;
        case 3: _t->cleanup(); break;
        case 4: _t->getEntryNumbersTest(); break;
        case 5: _t->getEntryNumbersTest_data(); break;
        case 6: _t->parseConfigLineTest(); break;
        case 7: _t->parseConfigLineTest_data(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject LoaderTest::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LoaderTest.data,
      qt_meta_data_LoaderTest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LoaderTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoaderTest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LoaderTest.stringdata0))
        return static_cast<void*>(const_cast< LoaderTest*>(this));
    return QObject::qt_metacast(_clname);
}

int LoaderTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
