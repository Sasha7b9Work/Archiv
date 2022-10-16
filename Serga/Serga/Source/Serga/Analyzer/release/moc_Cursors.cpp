/****************************************************************************
** Meta object code from reading C++ file 'Cursors.h'
**
** Created: Wed Jun 27 09:42:50 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Cursors.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Cursors.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Cursor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      18,    7,    7,    7, 0x08,
      36,    7,    7,    7, 0x08,
      54,    7,    7,    7, 0x08,
      79,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Cursor[] = {
    "Cursor\0\0Repaint()\0OnChangePos0(int)\0"
    "OnChangePos1(int)\0OnChangeCBFrequency(int)\0"
    "OnChangeCBRange(int)\0"
};

const QMetaObject Cursor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cursor,
      qt_meta_data_Cursor, 0 }
};

const QMetaObject *Cursor::metaObject() const
{
    return &staticMetaObject;
}

void *Cursor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Cursor))
        return static_cast<void*>(const_cast< Cursor*>(this));
    return QObject::qt_metacast(_clname);
}

int Cursor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Repaint(); break;
        case 1: OnChangePos0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnChangePos1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: OnChangeCBFrequency((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: OnChangeCBRange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Cursor::Repaint()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
