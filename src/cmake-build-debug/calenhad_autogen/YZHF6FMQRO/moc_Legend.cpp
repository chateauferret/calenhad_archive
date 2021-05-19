/****************************************************************************
** Meta object code from reading C++ file 'Legend.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../legend/Legend.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Legend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__legend__Legend_t {
    QByteArrayData data[11];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__legend__Legend_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__legend__Legend_t qt_meta_stringdata_calenhad__legend__Legend = {
    {
QT_MOC_LITERAL(0, 0, 24), // "calenhad::legend::Legend"
QT_MOC_LITERAL(1, 25, 13), // "legendChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 7), // "renamed"
QT_MOC_LITERAL(4, 48, 10), // "setEntries"
QT_MOC_LITERAL(5, 59, 20), // "QVector<LegendEntry>"
QT_MOC_LITERAL(6, 80, 7), // "entries"
QT_MOC_LITERAL(7, 88, 15), // "setInterpolated"
QT_MOC_LITERAL(8, 104, 11), // "interpolate"
QT_MOC_LITERAL(9, 116, 7), // "setName"
QT_MOC_LITERAL(10, 124, 4) // "name"

    },
    "calenhad::legend::Legend\0legendChanged\0"
    "\0renamed\0setEntries\0QVector<LegendEntry>\0"
    "entries\0setInterpolated\0interpolate\0"
    "setName\0name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__legend__Legend[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   43,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
       9,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void calenhad::legend::Legend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Legend *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->legendChanged(); break;
        case 1: _t->renamed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->setEntries((*reinterpret_cast< const QVector<LegendEntry>(*)>(_a[1]))); break;
        case 3: _t->setInterpolated((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 4: _t->setName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Legend::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Legend::legendChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Legend::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Legend::renamed)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::legend::Legend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_calenhad__legend__Legend.data,
    qt_meta_data_calenhad__legend__Legend,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::legend::Legend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::legend::Legend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__legend__Legend.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int calenhad::legend::Legend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}

// SIGNAL 0
void calenhad::legend::Legend::legendChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::legend::Legend::renamed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
