/****************************************************************************
** Meta object code from reading C++ file 'SignalScreen.h'
**
** Created: Wed Jun 27 09:42:49 2012
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
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      27,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SignalScreen[] = {
    "SignalScreen\0\0cursor,delta\0"
    "MovedCursor(int,int)\0Repaint()\0"
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
        case 0: MovedCursor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: Repaint(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SignalScreen::MovedCursor(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
