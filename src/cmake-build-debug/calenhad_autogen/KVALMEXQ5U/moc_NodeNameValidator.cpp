/****************************************************************************
** Meta object code from reading C++ file 'NodeNameValidator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/nodeedit/NodeNameValidator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NodeNameValidator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator_t qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator = {
    {
QT_MOC_LITERAL(0, 0, 37), // "calenhad::nodeedit::NodeNameV..."
QT_MOC_LITERAL(1, 38, 7), // "message"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 4), // "text"
QT_MOC_LITERAL(4, 52, 7) // "success"

    },
    "calenhad::nodeedit::NodeNameValidator\0"
    "message\0\0text\0success"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__nodeedit__NodeNameValidator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void calenhad::nodeedit::NodeNameValidator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NodeNameValidator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->success(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NodeNameValidator::*)(const QString & ) const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodeNameValidator::message)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NodeNameValidator::*)() const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodeNameValidator::success)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::nodeedit::NodeNameValidator::staticMetaObject = { {
    &QRegularExpressionValidator::staticMetaObject,
    qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator.data,
    qt_meta_data_calenhad__nodeedit__NodeNameValidator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::nodeedit::NodeNameValidator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::nodeedit::NodeNameValidator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__nodeedit__NodeNameValidator.stringdata0))
        return static_cast<void*>(this);
    return QRegularExpressionValidator::qt_metacast(_clname);
}

int calenhad::nodeedit::NodeNameValidator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRegularExpressionValidator::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void calenhad::nodeedit::NodeNameValidator::message(const QString & _t1)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< calenhad::nodeedit::NodeNameValidator *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void calenhad::nodeedit::NodeNameValidator::success()const
{
    QMetaObject::activate(const_cast< calenhad::nodeedit::NodeNameValidator *>(this), &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
