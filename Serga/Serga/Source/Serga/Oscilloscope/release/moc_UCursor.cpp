/****************************************************************************
** Meta object code from reading C++ file 'UCursor.h'
**
** Created: Wed Jun 27 09:43:26 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/UCursor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UCursor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UCursor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      19,    8,    8,    8, 0x08,
      41,    8,    8,    8, 0x08,
      69,    8,    8,    8, 0x08,
      97,    8,    8,    8, 0x08,
     117,    8,    8,    8, 0x08,
     140,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UCursor[] = {
    "UCursor\0\0Repaint()\0OnCheckBoxEnable(int)\0"
    "OnChangeSpinBoxCursor0(int)\0"
    "OnChangeSpinBoxCursor1(int)\0"
    "OnChangeSource(int)\0OnChangeTogether(bool)\0"
    "OnPressPBPercentage()\0"
};

const QMetaObject UCursor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UCursor,
      qt_meta_data_UCursor, 0 }
};

const QMetaObject *UCursor::metaObject() const
{
    return &staticMetaObject;
}

void *UCursor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UCursor))
        return static_cast<void*>(const_cast< UCursor*>(this));
    return QObject::qt_metacast(_clname);
}

int UCursor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Repaint(); break;
        case 1: OnCheckBoxEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnChangeSpinBoxCursor0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: OnChangeSpinBoxCursor1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: OnChangeSource((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: OnChangeTogether((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: OnPressPBPercentage(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void UCursor::Repaint()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
