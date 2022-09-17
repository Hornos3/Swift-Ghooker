/****************************************************************************
** Meta object code from reading C++ file 'output.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../output.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'output.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Output_t {
    const uint offsetsAndSize[20];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Output_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Output_t qt_meta_stringdata_Output = {
    {
QT_MOC_LITERAL(0, 6), // "Output"
QT_MOC_LITERAL(7, 19), // "on_fileView_clicked"
QT_MOC_LITERAL(27, 0), // ""
QT_MOC_LITERAL(28, 11), // "QModelIndex"
QT_MOC_LITERAL(40, 5), // "index"
QT_MOC_LITERAL(46, 25), // "on_showFileWidget_clicked"
QT_MOC_LITERAL(72, 24), // "on_showRegWidget_clicked"
QT_MOC_LITERAL(97, 25), // "on_showHeapWidget_clicked"
QT_MOC_LITERAL(123, 30), // "on_showExceptionWidget_clicked"
QT_MOC_LITERAL(154, 24) // "on_showLogWidget_clicked"

    },
    "Output\0on_fileView_clicked\0\0QModelIndex\0"
    "index\0on_showFileWidget_clicked\0"
    "on_showRegWidget_clicked\0"
    "on_showHeapWidget_clicked\0"
    "on_showExceptionWidget_clicked\0"
    "on_showLogWidget_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Output[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x08,    1 /* Private */,
       5,    0,   53,    2, 0x08,    3 /* Private */,
       6,    0,   54,    2, 0x08,    4 /* Private */,
       7,    0,   55,    2, 0x08,    5 /* Private */,
       8,    0,   56,    2, 0x08,    6 /* Private */,
       9,    0,   57,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Output::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Output *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_fileView_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 1: _t->on_showFileWidget_clicked(); break;
        case 2: _t->on_showRegWidget_clicked(); break;
        case 3: _t->on_showHeapWidget_clicked(); break;
        case 4: _t->on_showExceptionWidget_clicked(); break;
        case 5: _t->on_showLogWidget_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Output::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Output.offsetsAndSize,
    qt_meta_data_Output,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Output_t
, QtPrivate::TypeAndForceComplete<Output, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Output::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Output::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Output.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Output::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
