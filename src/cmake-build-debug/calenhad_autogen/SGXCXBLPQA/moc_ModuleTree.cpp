/****************************************************************************
** Meta object code from reading C++ file 'ModuleTree.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/controls/ModuleTree.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModuleTree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__controls__ModuleTree_t {
    QByteArrayData data[10];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__controls__ModuleTree_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__controls__ModuleTree_t qt_meta_stringdata_calenhad__controls__ModuleTree = {
    {
QT_MOC_LITERAL(0, 0, 30), // "calenhad::controls::ModuleTree"
QT_MOC_LITERAL(1, 31, 9), // "treeShown"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 10), // "treeHidden"
QT_MOC_LITERAL(4, 53, 9), // "buildTree"
QT_MOC_LITERAL(5, 63, 10), // "selectItem"
QT_MOC_LITERAL(6, 74, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 91, 4), // "item"
QT_MOC_LITERAL(8, 96, 8), // "showMenu"
QT_MOC_LITERAL(9, 105, 3) // "pos"

    },
    "calenhad::controls::ModuleTree\0treeShown\0"
    "\0treeHidden\0buildTree\0selectItem\0"
    "QTreeWidgetItem*\0item\0showMenu\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__controls__ModuleTree[] = {

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
       3,    0,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x0a /* Public */,
       8,    1,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QPoint,    9,

       0        // eod
};

void calenhad::controls::ModuleTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModuleTree *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->treeShown(); break;
        case 1: _t->treeHidden(); break;
        case 2: _t->buildTree(); break;
        case 3: _t->selectItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->showMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModuleTree::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModuleTree::treeShown)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModuleTree::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModuleTree::treeHidden)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::controls::ModuleTree::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_calenhad__controls__ModuleTree.data,
    qt_meta_data_calenhad__controls__ModuleTree,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::controls::ModuleTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::controls::ModuleTree::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__controls__ModuleTree.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int calenhad::controls::ModuleTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void calenhad::controls::ModuleTree::treeShown()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::controls::ModuleTree::treeHidden()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
