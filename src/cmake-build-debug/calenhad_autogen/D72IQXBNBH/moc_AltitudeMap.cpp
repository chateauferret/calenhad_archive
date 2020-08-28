/****************************************************************************
** Meta object code from reading C++ file 'AltitudeMap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/module/AltitudeMap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AltitudeMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__module__AltitudeMap_t {
    QByteArrayData data[7];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__module__AltitudeMap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__module__AltitudeMap_t qt_meta_stringdata_calenhad__module__AltitudeMap = {
    {
QT_MOC_LITERAL(0, 0, 29), // "calenhad::module::AltitudeMap"
QT_MOC_LITERAL(1, 30, 13), // "updateEntries"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 15), // "editingFinished"
QT_MOC_LITERAL(4, 61, 8), // "resetMap"
QT_MOC_LITERAL(5, 70, 15), // "editAltitudeMap"
QT_MOC_LITERAL(6, 86, 4) // "glsl"

    },
    "calenhad::module::AltitudeMap\0"
    "updateEntries\0\0editingFinished\0resetMap\0"
    "editAltitudeMap\0glsl"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__module__AltitudeMap[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,

       0        // eod
};

void calenhad::module::AltitudeMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AltitudeMap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateEntries(); break;
        case 1: _t->editingFinished(); break;
        case 2: _t->resetMap(); break;
        case 3: _t->editAltitudeMap(); break;
        case 4: { QString _r = _t->glsl();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::module::AltitudeMap::staticMetaObject = { {
    &Module::staticMetaObject,
    qt_meta_stringdata_calenhad__module__AltitudeMap.data,
    qt_meta_data_calenhad__module__AltitudeMap,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::module::AltitudeMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::module::AltitudeMap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__module__AltitudeMap.stringdata0))
        return static_cast<void*>(this);
    return Module::qt_metacast(_clname);
}

int calenhad::module::AltitudeMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Module::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
