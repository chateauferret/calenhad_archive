/****************************************************************************
** Meta object code from reading C++ file 'LegendChooser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/legend/LegendChooser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LegendChooser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__legend__LegendChooser_t {
    QByteArrayData data[11];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__legend__LegendChooser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__legend__LegendChooser_t qt_meta_stringdata_calenhad__legend__LegendChooser = {
    {
QT_MOC_LITERAL(0, 0, 31), // "calenhad::legend::LegendChooser"
QT_MOC_LITERAL(1, 32, 14), // "legendSelected"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 4), // "name"
QT_MOC_LITERAL(4, 53, 8), // "setLabel"
QT_MOC_LITERAL(5, 62, 8), // "oldLabel"
QT_MOC_LITERAL(6, 71, 8), // "newLabel"
QT_MOC_LITERAL(7, 80, 14), // "setCurrentIcon"
QT_MOC_LITERAL(8, 95, 4), // "icon"
QT_MOC_LITERAL(9, 100, 7), // "refresh"
QT_MOC_LITERAL(10, 108, 12) // "legendChosen"

    },
    "calenhad::legend::LegendChooser\0"
    "legendSelected\0\0name\0setLabel\0oldLabel\0"
    "newLabel\0setCurrentIcon\0icon\0refresh\0"
    "legendChosen"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__legend__LegendChooser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   42,    2, 0x0a /* Public */,
       7,    1,   47,    2, 0x0a /* Public */,
       9,    0,   50,    2, 0x0a /* Public */,
      10,    0,   51,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QIcon,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::legend::LegendChooser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LegendChooser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->legendSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->setLabel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->setCurrentIcon((*reinterpret_cast< const QIcon(*)>(_a[1]))); break;
        case 3: _t->refresh(); break;
        case 4: _t->legendChosen(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LegendChooser::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendChooser::legendSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::legend::LegendChooser::staticMetaObject = { {
    QMetaObject::SuperData::link<QComboBox::staticMetaObject>(),
    qt_meta_stringdata_calenhad__legend__LegendChooser.data,
    qt_meta_data_calenhad__legend__LegendChooser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::legend::LegendChooser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::legend::LegendChooser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__legend__LegendChooser.stringdata0))
        return static_cast<void*>(this);
    return QComboBox::qt_metacast(_clname);
}

int calenhad::legend::LegendChooser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
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
void calenhad::legend::LegendChooser::legendSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
