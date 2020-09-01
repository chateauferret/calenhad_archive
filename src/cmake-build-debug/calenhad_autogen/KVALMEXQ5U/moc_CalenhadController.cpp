/****************************************************************************
** Meta object code from reading C++ file 'CalenhadController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../nodeedit/CalenhadController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalenhadController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__nodeedit__CalenhadController_t {
    QByteArrayData data[19];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__nodeedit__CalenhadController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__nodeedit__CalenhadController_t qt_meta_stringdata_calenhad__nodeedit__CalenhadController = {
    {
QT_MOC_LITERAL(0, 0, 38), // "calenhad::nodeedit::CalenhadC..."
QT_MOC_LITERAL(1, 39, 14), // "canUndoChanged"
QT_MOC_LITERAL(2, 54, 0), // ""
QT_MOC_LITERAL(3, 55, 14), // "canRedoChanged"
QT_MOC_LITERAL(4, 70, 12), // "toolSelected"
QT_MOC_LITERAL(5, 83, 6), // "zoomIn"
QT_MOC_LITERAL(6, 90, 7), // "zoomOut"
QT_MOC_LITERAL(7, 98, 9), // "zoomToFit"
QT_MOC_LITERAL(8, 108, 15), // "zoomToSelection"
QT_MOC_LITERAL(9, 124, 10), // "toggleGrid"
QT_MOC_LITERAL(10, 135, 10), // "snapToGrid"
QT_MOC_LITERAL(11, 146, 10), // "moduleTree"
QT_MOC_LITERAL(12, 157, 5), // "paste"
QT_MOC_LITERAL(13, 163, 10), // "editAction"
QT_MOC_LITERAL(14, 174, 4), // "kill"
QT_MOC_LITERAL(15, 179, 4), // "yank"
QT_MOC_LITERAL(16, 184, 4), // "undo"
QT_MOC_LITERAL(17, 189, 4), // "redo"
QT_MOC_LITERAL(18, 194, 7) // "showXml"

    },
    "calenhad::nodeedit::CalenhadController\0"
    "canUndoChanged\0\0canRedoChanged\0"
    "toolSelected\0zoomIn\0zoomOut\0zoomToFit\0"
    "zoomToSelection\0toggleGrid\0snapToGrid\0"
    "moduleTree\0paste\0editAction\0kill\0yank\0"
    "undo\0redo\0showXml"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__nodeedit__CalenhadController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    0,   90,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   91,    2, 0x0a /* Public */,
       5,    0,   92,    2, 0x0a /* Public */,
       6,    0,   93,    2, 0x0a /* Public */,
       7,    0,   94,    2, 0x0a /* Public */,
       8,    0,   95,    2, 0x0a /* Public */,
       9,    0,   96,    2, 0x0a /* Public */,
      10,    0,   97,    2, 0x0a /* Public */,
      11,    0,   98,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    2,  100,    2, 0x0a /* Public */,
      16,    0,  105,    2, 0x0a /* Public */,
      17,    0,  106,    2, 0x0a /* Public */,
      18,    0,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::nodeedit::CalenhadController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalenhadController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->canUndoChanged(); break;
        case 1: _t->canRedoChanged(); break;
        case 2: _t->toolSelected(); break;
        case 3: _t->zoomIn(); break;
        case 4: _t->zoomOut(); break;
        case 5: _t->zoomToFit(); break;
        case 6: _t->zoomToSelection(); break;
        case 7: _t->toggleGrid(); break;
        case 8: _t->snapToGrid(); break;
        case 9: _t->moduleTree(); break;
        case 10: _t->paste(); break;
        case 11: _t->editAction((*reinterpret_cast< const bool(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 12: _t->undo(); break;
        case 13: _t->redo(); break;
        case 14: _t->showXml(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalenhadController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadController::canUndoChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalenhadController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadController::canRedoChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::nodeedit::CalenhadController::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_calenhad__nodeedit__CalenhadController.data,
    qt_meta_data_calenhad__nodeedit__CalenhadController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::nodeedit::CalenhadController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::nodeedit::CalenhadController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__nodeedit__CalenhadController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int calenhad::nodeedit::CalenhadController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void calenhad::nodeedit::CalenhadController::canUndoChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::nodeedit::CalenhadController::canRedoChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
