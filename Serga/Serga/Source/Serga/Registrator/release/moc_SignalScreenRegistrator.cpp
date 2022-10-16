/****************************************************************************
** Meta object code from reading C++ file 'SignalScreenRegistrator.h'
**
** Created: Wed Jun 27 09:43:04 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SignalScreenRegistrator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SignalScreenRegistrator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SignalScreenRegistrator[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      55,   41,   24,   24, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SignalScreenRegistrator[] = {
    "SignalScreenRegistrator\0\0MoveSignal(int)\0"
    "numCur,deltaY\0ChangePosUCur(int,int)\0"
};

const QMetaObject SignalScreenRegistrator::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SignalScreenRegistrator,
      qt_meta_data_SignalScreenRegistrator, 0 }
};

const QMetaObject *SignalScreenRegistrator::metaObject() const
{
    return &staticMetaObject;
}

void *SignalScreenRegistrator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SignalScreenRegistrator))
        return static_cast<void*>(const_cast< SignalScreenRegistrator*>(this));
    return QFrame::qt_metacast(_clname);
}

int SignalScreenRegistrator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MoveSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: ChangePosUCur((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SignalScreenRegistrator::MoveSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SignalScreenRegistrator::ChangePosUCur(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
