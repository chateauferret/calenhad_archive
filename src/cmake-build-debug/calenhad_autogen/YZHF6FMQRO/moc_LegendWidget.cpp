/****************************************************************************
** Meta object code from reading C++ file 'LegendWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/legend/LegendWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LegendWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__legend__LegendWidget_t {
    QByteArrayData data[10];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__legend__LegendWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__legend__LegendWidget_t qt_meta_stringdata_calenhad__legend__LegendWidget = {
    {
QT_MOC_LITERAL(0, 0, 30), // "calenhad::legend::LegendWidget"
QT_MOC_LITERAL(1, 31, 11), // "nameChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 12), // "notesChanged"
QT_MOC_LITERAL(4, 57, 11), // "iconChanged"
QT_MOC_LITERAL(5, 69, 13), // "legendChanged"
QT_MOC_LITERAL(6, 83, 20), // "QVector<LegendEntry>"
QT_MOC_LITERAL(7, 104, 9), // "checkName"
QT_MOC_LITERAL(8, 114, 10), // "updateName"
QT_MOC_LITERAL(9, 125, 8) // "populate"

    },
    "calenhad::legend::LegendWidget\0"
    "nameChanged\0\0notesChanged\0iconChanged\0"
    "legendChanged\0QVector<LegendEntry>\0"
    "checkName\0updateName\0populate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__legend__LegendWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       3,    1,   52,    2, 0x06 /* Public */,
       4,    1,   55,    2, 0x06 /* Public */,
       5,    1,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   61,    2, 0x09 /* Protected */,
       8,    0,   62,    2, 0x09 /* Protected */,
       9,    0,   63,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QIcon,    2,
    QMetaType::Void, 0x80000000 | 6,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::legend::LegendWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LegendWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->notesChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->iconChanged((*reinterpret_cast< QIcon(*)>(_a[1]))); break;
        case 3: _t->legendChanged((*reinterpret_cast< const QVector<LegendEntry>(*)>(_a[1]))); break;
        case 4: _t->checkName(); break;
        case 5: _t->updateName(); break;
        case 6: _t->populate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LegendWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendWidget::nameChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LegendWidget::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendWidget::notesChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LegendWidget::*)(QIcon );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendWidget::iconChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LegendWidget::*)(const QVector<LegendEntry> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendWidget::legendChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::legend::LegendWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_calenhad__legend__LegendWidget.data,
    qt_meta_data_calenhad__legend__LegendWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::legend::LegendWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::legend::LegendWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__legend__LegendWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int calenhad::legend::LegendWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void calenhad::legend::LegendWidget::nameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void calenhad::legend::LegendWidget::notesChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void calenhad::legend::LegendWidget::iconChanged(QIcon _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void calenhad::legend::LegendWidget::legendChanged(const QVector<LegendEntry> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
