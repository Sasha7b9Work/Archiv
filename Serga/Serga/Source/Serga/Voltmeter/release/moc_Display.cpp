/****************************************************************************
** Meta object code from reading C++ file 'Display.h'
**
** Created: Wed Jun 27 09:43:17 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Display.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Display.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Display[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      36,    8,    8,    8, 0x0a,
      57,    8,    8,    8, 0x0a,
      78,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Display[] = {
    "Display\0\0SetColorBackground(QColor)\0"
    "SetColorGrid(QColor)\0SetColorFont(QColor)\0"
    "SetColorSignal(QColor)\0"
};

const QMetaObject Display::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Display,
      qt_meta_data_Display, 0 }
};

const QMetaObject *Display::metaObject() const
{
    return &staticMetaObject;
}

void *Display::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Display))
        return static_cast<void*>(const_cast< Display*>(this));
    return QFrame::qt_metacast(_clname);
}

int Display::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SetColorBackground((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: SetColorGrid((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: SetColorFont((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: SetColorSignal((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
