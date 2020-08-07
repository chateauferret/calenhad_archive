/****************************************************************************
** Meta object code from reading C++ file 'CalenhadView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/nodeedit/CalenhadView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalenhadView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__nodeedit__CalenhadView_t {
    QByteArrayData data[8];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__nodeedit__CalenhadView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__nodeedit__CalenhadView_t qt_meta_stringdata_calenhad__nodeedit__CalenhadView = {
    {
QT_MOC_LITERAL(0, 0, 32), // "calenhad::nodeedit::CalenhadView"
QT_MOC_LITERAL(1, 33, 15), // "zoomInRequested"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 16), // "zoomOutRequested"
QT_MOC_LITERAL(4, 67, 10), // "viewZoomed"
QT_MOC_LITERAL(5, 78, 9), // "oldFactor"
QT_MOC_LITERAL(6, 88, 9), // "newFactor"
QT_MOC_LITERAL(7, 98, 12) // "modelChanged"

    },
    "calenhad::nodeedit::CalenhadView\0"
    "zoomInRequested\0\0zoomOutRequested\0"
    "viewZoomed\0oldFactor\0newFactor\0"
    "modelChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__nodeedit__CalenhadView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    2,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    5,    6,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void calenhad::nodeedit::CalenhadView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalenhadView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomInRequested(); break;
        case 1: _t->zoomOutRequested(); break;
        case 2: _t->viewZoomed((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->modelChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalenhadView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadView::zoomInRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalenhadView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadView::zoomOutRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalenhadView::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadView::viewZoomed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::nodeedit::CalenhadView::staticMetaObject = { {
    &QGraphicsView::staticMetaObject,
    qt_meta_stringdata_calenhad__nodeedit__CalenhadView.data,
    qt_meta_data_calenhad__nodeedit__CalenhadView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::nodeedit::CalenhadView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::nodeedit::CalenhadView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__nodeedit__CalenhadView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int calenhad::nodeedit::CalenhadView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void calenhad::nodeedit::CalenhadView::zoomInRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::nodeedit::CalenhadView::zoomOutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void calenhad::nodeedit::CalenhadView::viewZoomed(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
