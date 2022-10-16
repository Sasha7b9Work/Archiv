/****************************************************************************
** Meta object code from reading C++ file 'SignalScreen.h'
**
** Created: Wed Jun 27 09:43:26 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SignalScreen.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SignalScreen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SignalScreen[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      36,   14,   13,   13, 0x05,
      85,   63,   13,   13, 0x05,
     112,   13,   13,   13, 0x05,
     131,   13,   13,   13, 0x05,
     150,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SignalScreen[] = {
    "SignalScreen\0\0channel,numCur,deltaY\0"
    "ChangePosUCur(int,int,int)\0"
    "channel,numCur,deltaX\0ChangePosTCur(int,int,int)\0"
    "ChangeRShift0(int)\0ChangeRShift1(int)\0"
    "ChangeTShift(int)\0"
};

const QMetaObject SignalScreen::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SignalScreen,
      qt_meta_data_SignalScreen, 0 }
};

const QMetaObject *SignalScreen::metaObject() const
{
    return &staticMetaObject;
}

void *SignalScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SignalScreen))
        return static_cast<void*>(const_cast< SignalScreen*>(this));
    return QFrame::qt_metacast(_clname);
}

int SignalScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ChangePosUCur((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: ChangePosTCur((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: ChangeRShift0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: ChangeRShift1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: ChangeTShift((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SignalScreen::ChangePosUCur(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SignalScreen::ChangePosTCur(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SignalScreen::ChangeRShift0(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SignalScreen::ChangeRShift1(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SignalScreen::ChangeTShift(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
