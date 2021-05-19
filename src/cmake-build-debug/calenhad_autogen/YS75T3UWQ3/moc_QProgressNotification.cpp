/****************************************************************************
** Meta object code from reading C++ file 'QProgressNotification.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../messages/QProgressNotification.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QProgressNotification.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__notification__QProgressNotification_t {
    QByteArrayData data[12];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__notification__QProgressNotification_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__notification__QProgressNotification_t qt_meta_stringdata_calenhad__notification__QProgressNotification = {
    {
QT_MOC_LITERAL(0, 0, 45), // "calenhad::notification::QProg..."
QT_MOC_LITERAL(1, 46, 8), // "complete"
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 6), // "cancel"
QT_MOC_LITERAL(4, 63, 10), // "setMessage"
QT_MOC_LITERAL(5, 74, 7), // "message"
QT_MOC_LITERAL(6, 82, 7), // "setToDo"
QT_MOC_LITERAL(7, 90, 4), // "toDo"
QT_MOC_LITERAL(8, 95, 11), // "setProgress"
QT_MOC_LITERAL(9, 107, 8), // "progress"
QT_MOC_LITERAL(10, 116, 4), // "kill"
QT_MOC_LITERAL(11, 121, 11) // "setComplete"

    },
    "calenhad::notification::QProgressNotification\0"
    "complete\0\0cancel\0setMessage\0message\0"
    "setToDo\0toDo\0setProgress\0progress\0"
    "kill\0setComplete"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__notification__QProgressNotification[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   51,    2, 0x0a /* Public */,
       6,    1,   54,    2, 0x0a /* Public */,
       8,    1,   57,    2, 0x0a /* Public */,
      10,    0,   60,    2, 0x0a /* Public */,
      11,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::notification::QProgressNotification::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QProgressNotification *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->complete(); break;
        case 1: _t->cancel(); break;
        case 2: _t->setMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setToDo((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->setProgress((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 5: _t->kill(); break;
        case 6: _t->setComplete(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QProgressNotification::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QProgressNotification::complete)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QProgressNotification::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QProgressNotification::cancel)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::notification::QProgressNotification::staticMetaObject = { {
    QMetaObject::SuperData::link<QNotification::staticMetaObject>(),
    qt_meta_stringdata_calenhad__notification__QProgressNotification.data,
    qt_meta_data_calenhad__notification__QProgressNotification,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::notification::QProgressNotification::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::notification::QProgressNotification::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__notification__QProgressNotification.stringdata0))
        return static_cast<void*>(this);
    return QNotification::qt_metacast(_clname);
}

int calenhad::notification::QProgressNotification::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QNotification::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void calenhad::notification::QProgressNotification::complete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::notification::QProgressNotification::cancel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
