/****************************************************************************
** Meta object code from reading C++ file 'Calenhad.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/nodeedit/Calenhad.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Calenhad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calenhad__nodeedit__Calenhad_t {
    QByteArrayData data[17];
    char stringdata0[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calenhad__nodeedit__Calenhad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calenhad__nodeedit__Calenhad_t qt_meta_stringdata_calenhad__nodeedit__Calenhad = {
    {
QT_MOC_LITERAL(0, 0, 28), // "calenhad::nodeedit::Calenhad"
QT_MOC_LITERAL(1, 29, 12), // "titleChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 5), // "title"
QT_MOC_LITERAL(4, 49, 10), // "newProject"
QT_MOC_LITERAL(5, 60, 12), // "closeProject"
QT_MOC_LITERAL(6, 73, 8), // "saveFile"
QT_MOC_LITERAL(7, 82, 10), // "saveFileAs"
QT_MOC_LITERAL(8, 93, 16), // "CalenhadFileType"
QT_MOC_LITERAL(9, 110, 8), // "fileType"
QT_MOC_LITERAL(10, 119, 8), // "loadFile"
QT_MOC_LITERAL(11, 128, 5), // "fname"
QT_MOC_LITERAL(12, 134, 11), // "openProject"
QT_MOC_LITERAL(13, 146, 4), // "open"
QT_MOC_LITERAL(14, 151, 4), // "quit"
QT_MOC_LITERAL(15, 156, 17), // "projectProperties"
QT_MOC_LITERAL(16, 174, 13) // "manageLegends"

    },
    "calenhad::nodeedit::Calenhad\0titleChanged\0"
    "\0title\0newProject\0closeProject\0saveFile\0"
    "saveFileAs\0CalenhadFileType\0fileType\0"
    "loadFile\0fname\0openProject\0open\0quit\0"
    "projectProperties\0manageLegends"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calenhad__nodeedit__Calenhad[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       4,    0,   82,    2, 0x0a /* Public */,
       5,    0,   83,    2, 0x0a /* Public */,
       6,    0,   84,    2, 0x0a /* Public */,
       7,    1,   85,    2, 0x0a /* Public */,
      10,    1,   88,    2, 0x0a /* Public */,
      10,    0,   91,    2, 0x2a /* Public | MethodCloned */,
      10,    2,   92,    2, 0x0a /* Public */,
      12,    1,   97,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    0,  101,    2, 0x0a /* Public */,
      15,    0,  102,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,   11,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calenhad::nodeedit::Calenhad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Calenhad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->titleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->newProject(); break;
        case 2: _t->closeProject(); break;
        case 3: _t->saveFile(); break;
        case 4: _t->saveFileAs((*reinterpret_cast< const CalenhadFileType(*)>(_a[1]))); break;
        case 5: _t->loadFile((*reinterpret_cast< const CalenhadFileType(*)>(_a[1]))); break;
        case 6: _t->loadFile(); break;
        case 7: _t->loadFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const CalenhadFileType(*)>(_a[2]))); break;
        case 8: _t->openProject((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->open(); break;
        case 10: _t->quit(); break;
        case 11: _t->projectProperties(); break;
        case 12: _t->manageLegends(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject calenhad::nodeedit::Calenhad::staticMetaObject = { {
    QMetaObject::SuperData::link<calenhad::notification::QNotificationHost::staticMetaObject>(),
    qt_meta_stringdata_calenhad__nodeedit__Calenhad.data,
    qt_meta_data_calenhad__nodeedit__Calenhad,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calenhad::nodeedit::Calenhad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calenhad::nodeedit::Calenhad::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calenhad__nodeedit__Calenhad.stringdata0))
        return static_cast<void*>(this);
    return calenhad::notification::QNotificationHost::qt_metacast(_clname);
}

int calenhad::nodeedit::Calenhad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = calenhad::notification::QNotificationHost::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
