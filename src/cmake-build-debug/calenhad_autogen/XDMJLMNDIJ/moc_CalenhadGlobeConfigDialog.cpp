/****************************************************************************
** Meta object code from reading C++ file 'CalenhadGlobeConfigDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../controls/globe/CalenhadGlobeConfigDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalenhadGlobeConfigDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog_t {
    QByteArrayData data[5];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog_t qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog = {
    {
QT_MOC_LITERAL(0, 0, 52), // "calenhad::controls::globe::Ca..."
QT_MOC_LITERAL(1, 53, 13), // "legendChanged"
QT_MOC_LITERAL(2, 67, 0), // ""
QT_MOC_LITERAL(3, 68, 25), // "calenhad::legend::Legend*"
QT_MOC_LITERAL(4, 94, 6) // "legend"

    },
    "calenhad::controls::globe::CalenhadGlobeConfigDialog\0"
    "legendChanged\0\0calenhad::legend::Legend*\0"
    "legend"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__controls__globe__CalenhadGlobeConfigDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void calenhad::controls::globe::CalenhadGlobeConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalenhadGlobeConfigDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->legendChanged((*reinterpret_cast< calenhad::legend::Legend*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< calenhad::legend::Legend* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalenhadGlobeConfigDialog::*)(calenhad::legend::Legend * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalenhadGlobeConfigDialog::legendChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::controls::globe::CalenhadGlobeConfigDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog.data,
    qt_meta_data_calenhad__controls__globe__CalenhadGlobeConfigDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::controls::globe::CalenhadGlobeConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::controls::globe::CalenhadGlobeConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__controls__globe__CalenhadGlobeConfigDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int calenhad::controls::globe::CalenhadGlobeConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void calenhad::controls::globe::CalenhadGlobeConfigDialog::legendChanged(calenhad::legend::Legend * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
