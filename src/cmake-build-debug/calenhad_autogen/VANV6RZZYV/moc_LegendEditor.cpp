/****************************************************************************
** Meta object code from reading C++ file 'LegendEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/controls/legend/LegendEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LegendEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__controls__legend__LegendEditor_t {
    QByteArrayData data[13];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__controls__legend__LegendEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__controls__legend__LegendEditor_t qt_meta_stringdata_calenhad__controls__legend__LegendEditor = {
    {
QT_MOC_LITERAL(0, 0, 40), // "calenhad::controls::legend::L..."
QT_MOC_LITERAL(1, 41, 13), // "legendChanged"
QT_MOC_LITERAL(2, 55, 0), // ""
QT_MOC_LITERAL(3, 56, 38), // "QVector<calenhad::legend::Leg..."
QT_MOC_LITERAL(4, 95, 11), // "resizeEvent"
QT_MOC_LITERAL(5, 107, 13), // "QResizeEvent*"
QT_MOC_LITERAL(6, 121, 1), // "e"
QT_MOC_LITERAL(7, 123, 15), // "mousePressEvent"
QT_MOC_LITERAL(8, 139, 12), // "QMouseEvent*"
QT_MOC_LITERAL(9, 152, 10), // "updateRamp"
QT_MOC_LITERAL(10, 163, 4), // "zoom"
QT_MOC_LITERAL(11, 168, 5), // "steps"
QT_MOC_LITERAL(12, 174, 3) // "pan"

    },
    "calenhad::controls::legend::LegendEditor\0"
    "legendChanged\0\0QVector<calenhad::legend::LegendEntry>\0"
    "resizeEvent\0QResizeEvent*\0e\0mousePressEvent\0"
    "QMouseEvent*\0updateRamp\0zoom\0steps\0"
    "pan"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__controls__legend__LegendEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x09 /* Protected */,
       7,    1,   50,    2, 0x09 /* Protected */,
       9,    0,   53,    2, 0x09 /* Protected */,
      10,    1,   54,    2, 0x09 /* Protected */,
      12,    1,   57,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void calenhad::controls::legend::LegendEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LegendEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->legendChanged((*reinterpret_cast< const QVector<calenhad::legend::LegendEntry>(*)>(_a[1]))); break;
        case 1: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->updateRamp(); break;
        case 4: _t->zoom((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 5: _t->pan((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LegendEditor::*)(const QVector<calenhad::legend::LegendEntry> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LegendEditor::legendChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::controls::legend::LegendEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_calenhad__controls__legend__LegendEditor.data,
    qt_meta_data_calenhad__controls__legend__LegendEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::controls::legend::LegendEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::controls::legend::LegendEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__controls__legend__LegendEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int calenhad::controls::legend::LegendEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void calenhad::controls::legend::LegendEditor::legendChanged(const QVector<calenhad::legend::LegendEntry> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
