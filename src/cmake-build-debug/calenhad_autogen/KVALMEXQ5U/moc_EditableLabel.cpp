/****************************************************************************
** Meta object code from reading C++ file 'EditableLabel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/nodeedit/EditableLabel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EditableLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__nodeedit__EditableLabel_t {
    QByteArrayData data[11];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__nodeedit__EditableLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__nodeedit__EditableLabel_t qt_meta_stringdata_calenhad__nodeedit__EditableLabel = {
    {
QT_MOC_LITERAL(0, 0, 33), // "calenhad::nodeedit::EditableL..."
QT_MOC_LITERAL(1, 34, 19), // "editingStateChanged"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 5), // "state"
QT_MOC_LITERAL(4, 61, 11), // "textChanged"
QT_MOC_LITERAL(5, 73, 4), // "text"
QT_MOC_LITERAL(6, 78, 10), // "textEdited"
QT_MOC_LITERAL(7, 89, 11), // "showMessage"
QT_MOC_LITERAL(8, 101, 7), // "message"
QT_MOC_LITERAL(9, 109, 12), // "clearMessage"
QT_MOC_LITERAL(10, 122, 15) // "finishedEditing"

    },
    "calenhad::nodeedit::EditableLabel\0"
    "editingStateChanged\0\0state\0textChanged\0"
    "text\0textEdited\0showMessage\0message\0"
    "clearMessage\0finishedEditing"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__nodeedit__EditableLabel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   53,    2, 0x0a /* Public */,
       9,    0,   56,    2, 0x0a /* Public */,
      10,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::nodeedit::EditableLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EditableLabel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->editingStateChanged((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 1: _t->textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->showMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->clearMessage(); break;
        case 5: _t->finishedEditing(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EditableLabel::*)(const bool & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EditableLabel::editingStateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EditableLabel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EditableLabel::textChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EditableLabel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EditableLabel::textEdited)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::nodeedit::EditableLabel::staticMetaObject = { {
    &QGraphicsTextItem::staticMetaObject,
    qt_meta_stringdata_calenhad__nodeedit__EditableLabel.data,
    qt_meta_data_calenhad__nodeedit__EditableLabel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::nodeedit::EditableLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::nodeedit::EditableLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__nodeedit__EditableLabel.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsTextItem::qt_metacast(_clname);
}

int calenhad::nodeedit::EditableLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsTextItem::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void calenhad::nodeedit::EditableLabel::editingStateChanged(const bool & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void calenhad::nodeedit::EditableLabel::textChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void calenhad::nodeedit::EditableLabel::textEdited(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
