/****************************************************************************
** Meta object code from reading C++ file 'CalenhadMapWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/mapping/CalenhadMapWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalenhadMapWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget_t {
    QByteArrayData data[19];
    char stringdata0[356];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget_t qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget = {
    {
QT_MOC_LITERAL(0, 0, 36), // "calenhad::mapping::CalenhadMa..."
QT_MOC_LITERAL(1, 37, 8), // "rendered"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 13), // "zoomRequested"
QT_MOC_LITERAL(4, 61, 4), // "zoom"
QT_MOC_LITERAL(5, 66, 15), // "zoomInRequested"
QT_MOC_LITERAL(6, 82, 16), // "zoomOutRequested"
QT_MOC_LITERAL(7, 99, 13), // "setProjection"
QT_MOC_LITERAL(8, 113, 10), // "projection"
QT_MOC_LITERAL(9, 124, 8), // "navigate"
QT_MOC_LITERAL(10, 133, 42), // "calenhad::controls::globe::Na..."
QT_MOC_LITERAL(11, 176, 1), // "e"
QT_MOC_LITERAL(12, 178, 12), // "exportImages"
QT_MOC_LITERAL(13, 191, 23), // "setMouseDoubleClickMode"
QT_MOC_LITERAL(14, 215, 55), // "calenhad::controls::globe::Ca..."
QT_MOC_LITERAL(15, 271, 4), // "mode"
QT_MOC_LITERAL(16, 276, 16), // "setMouseDragMode"
QT_MOC_LITERAL(17, 293, 48), // "calenhad::controls::globe::Ca..."
QT_MOC_LITERAL(18, 342, 13) // "textureHeight"

    },
    "calenhad::mapping::CalenhadMapWidget\0"
    "rendered\0\0zoomRequested\0zoom\0"
    "zoomInRequested\0zoomOutRequested\0"
    "setProjection\0projection\0navigate\0"
    "calenhad::controls::globe::NavigationEvent\0"
    "e\0exportImages\0setMouseDoubleClickMode\0"
    "calenhad::controls::globe::CalenhadGlobeDoubleClickMode\0"
    "mode\0setMouseDragMode\0"
    "calenhad::controls::globe::CalenhadGlobeDragMode\0"
    "textureHeight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__mapping__CalenhadMapWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       5,    0,   68,    2, 0x06 /* Public */,
       6,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   70,    2, 0x0a /* Public */,
       9,    1,   73,    2, 0x0a /* Public */,
      12,    0,   76,    2, 0x0a /* Public */,
      13,    1,   77,    2, 0x0a /* Public */,
      16,    1,   80,    2, 0x0a /* Public */,
      18,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   15,
    QMetaType::Int,

       0        // eod
};

void calenhad::mapping::CalenhadMapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalenhadMapWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rendered(); break;
        case 1: _t->zoomRequested((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 2: _t->zoomInRequested(); break;
        case 3: _t->zoomOutRequested(); break;
        case 4: _t->setProjection((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->navigate((*reinterpret_cast< const calenhad::controls::globe::NavigationEvent(*)>(_a[1]))); break;
        case 6: _t->exportImages(); break;
        case 7: _t->setMouseDoubleClickMode((*reinterpret_cast< const calenhad::controls::globe::CalenhadGlobeDoubleClickMode(*)>(_a[1]))); break;
        case 8: _t->setMouseDragMode((*reinterpret_cast< const calenhad::controls::globe::CalenhadGlobeDragMode(*)>(_a[1]))); break;
        case 9: { int _r = _t->textureHeight();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalenhadMapWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadMapWidget::rendered)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalenhadMapWidget::*)(const double & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadMapWidget::zoomRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalenhadMapWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadMapWidget::zoomInRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CalenhadMapWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadMapWidget::zoomOutRequested)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::mapping::CalenhadMapWidget::staticMetaObject = { {
    &QOpenGLWidget::staticMetaObject,
    qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget.data,
    qt_meta_data_calenhad__mapping__CalenhadMapWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::mapping::CalenhadMapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::mapping::CalenhadMapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__mapping__CalenhadMapWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_4_3_Core"))
        return static_cast< QOpenGLFunctions_4_3_Core*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int calenhad::mapping::CalenhadMapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void calenhad::mapping::CalenhadMapWidget::rendered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void calenhad::mapping::CalenhadMapWidget::zoomRequested(const double & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void calenhad::mapping::CalenhadMapWidget::zoomInRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void calenhad::mapping::CalenhadMapWidget::zoomOutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
