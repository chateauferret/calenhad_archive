/****************************************************************************
** Meta object code from reading C++ file 'ExpressionWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/exprtk/ExpressionWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExpressionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__expressions__ExpressionWidget_t {
    QByteArrayData data[17];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__expressions__ExpressionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__expressions__ExpressionWidget_t qt_meta_stringdata_calenhad__expressions__ExpressionWidget = {
    {
QT_MOC_LITERAL(0, 0, 39), // "calenhad::expressions::Expres..."
QT_MOC_LITERAL(1, 40, 8), // "compiled"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 5), // "value"
QT_MOC_LITERAL(4, 56, 17), // "expressionChanged"
QT_MOC_LITERAL(5, 74, 10), // "errorFound"
QT_MOC_LITERAL(6, 85, 11), // "editingText"
QT_MOC_LITERAL(7, 97, 15), // "editingFinished"
QT_MOC_LITERAL(8, 113, 9), // "validated"
QT_MOC_LITERAL(9, 123, 7), // "prepare"
QT_MOC_LITERAL(10, 131, 11), // "openLongBox"
QT_MOC_LITERAL(11, 143, 8), // "editText"
QT_MOC_LITERAL(12, 152, 12), // "setValidator"
QT_MOC_LITERAL(13, 165, 33), // "calenhad::module::ParamValida..."
QT_MOC_LITERAL(14, 199, 9), // "validator"
QT_MOC_LITERAL(15, 209, 15), // "variableChanged"
QT_MOC_LITERAL(16, 225, 4) // "name"

    },
    "calenhad::expressions::ExpressionWidget\0"
    "compiled\0\0value\0expressionChanged\0"
    "errorFound\0editingText\0editingFinished\0"
    "validated\0prepare\0openLongBox\0editText\0"
    "setValidator\0calenhad::module::ParamValidator*\0"
    "validator\0variableChanged\0name"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__expressions__ExpressionWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    0,   72,    2, 0x06 /* Public */,
       5,    0,   73,    2, 0x06 /* Public */,
       6,    0,   74,    2, 0x06 /* Public */,
       7,    0,   75,    2, 0x06 /* Public */,
       8,    0,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   77,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    1,   80,    2, 0x0a /* Public */,
      15,    2,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::Double,   16,    3,

       0        // eod
};

void calenhad::expressions::ExpressionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExpressionWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->compiled((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 1: _t->expressionChanged(); break;
        case 2: _t->errorFound(); break;
        case 3: _t->editingText(); break;
        case 4: _t->editingFinished(); break;
        case 5: _t->validated(); break;
        case 6: { bool _r = _t->prepare();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->openLongBox(); break;
        case 8: _t->editText(); break;
        case 9: _t->setValidator((*reinterpret_cast< calenhad::module::ParamValidator*(*)>(_a[1]))); break;
        case 10: _t->variableChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ExpressionWidget::*)(const double & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::compiled)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ExpressionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::expressionChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ExpressionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::errorFound)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ExpressionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::editingText)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ExpressionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::editingFinished)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ExpressionWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ExpressionWidget::validated)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::expressions::ExpressionWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_calenhad__expressions__ExpressionWidget.data,
    qt_meta_data_calenhad__expressions__ExpressionWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::expressions::ExpressionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::expressions::ExpressionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__expressions__ExpressionWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int calenhad::expressions::ExpressionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void calenhad::expressions::ExpressionWidget::compiled(const double & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void calenhad::expressions::ExpressionWidget::expressionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void calenhad::expressions::ExpressionWidget::errorFound()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void calenhad::expressions::ExpressionWidget::editingText()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void calenhad::expressions::ExpressionWidget::editingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void calenhad::expressions::ExpressionWidget::validated()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
