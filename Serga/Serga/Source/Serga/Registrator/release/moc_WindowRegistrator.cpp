/****************************************************************************
** Meta object code from reading C++ file 'WindowRegistrator.h'
**
** Created: Wed Jun 27 09:43:04 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/WindowRegistrator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowRegistrator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowRegistrator[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   18,   18,   18, 0x08,
      45,   18,   18,   18, 0x08,
      57,   18,   18,   18, 0x08,
      83,   72,   67,   18, 0x08,
     102,   18,   18,   18, 0x08,
     117,   18,   18,   18, 0x08,
     132,   18,   18,   18, 0x08,
     153,   18,   18,   18, 0x08,
     174,   18,   18,   18, 0x08,
     185,   18,   18,   18, 0x08,
     198,   18,   18,   18, 0x08,
     215,   18,   18,   18, 0x08,
     232,   18,   18,   18, 0x08,
     253,   18,   18,   18, 0x08,
     274,   18,   18,   18, 0x08,
     297,   18,   18,   18, 0x08,
     320,   18,   18,   18, 0x08,
     343,   18,   18,   18, 0x08,
     366,   18,   18,   18, 0x08,
     374,   18,   18,   18, 0x08,
     395,   18,   18,   18, 0x08,
     410,   18,   18,   18, 0x08,
     429,  427,   18,   18, 0x08,
     462,  427,   18,   18, 0x08,
     495,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowRegistrator[] = {
    "WindowRegistrator\0\0terminate()\0"
    "UpdatePorts()\0Start(bool)\0OnTimer()\0"
    "bool\0_indexPort\0SetNumberPort(int)\0"
    "SetRange0(int)\0SetRange1(int)\0"
    "SB0valueChanged(int)\0SB1valueChanged(int)\0"
    "Open(bool)\0SetDir(bool)\0MoveSignal0(int)\0"
    "MoveSignal1(int)\0EnableUCursCh0(bool)\0"
    "EnableUCursCh1(bool)\0ChangePosUCur0Ch0(int)\0"
    "ChangePosUCur1Ch0(int)\0ChangePosUCur0Ch1(int)\0"
    "ChangePosUCur1Ch1(int)\0close()\0"
    "SetColorBackground()\0SetColorGrid()\0"
    "SetColorSignal()\0,\0ChangePosUCurFromMouse0(int,int)\0"
    "ChangePosUCurFromMouse1(int,int)\0"
    "Calibration()\0"
};

const QMetaObject WindowRegistrator::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WindowRegistrator,
      qt_meta_data_WindowRegistrator, 0 }
};

const QMetaObject *WindowRegistrator::metaObject() const
{
    return &staticMetaObject;
}

void *WindowRegistrator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowRegistrator))
        return static_cast<void*>(const_cast< WindowRegistrator*>(this));
    if (!strcmp(_clname, "Ui::WindowRegistrator"))
        return static_cast< Ui::WindowRegistrator*>(const_cast< WindowRegistrator*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WindowRegistrator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: terminate(); break;
        case 1: UpdatePorts(); break;
        case 2: Start((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: OnTimer(); break;
        case 4: { bool _r = SetNumberPort((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: SetRange0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: SetRange1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: SB0valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: SB1valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: Open((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: SetDir((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: MoveSignal0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: MoveSignal1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: EnableUCursCh0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: EnableUCursCh1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: ChangePosUCur0Ch0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: ChangePosUCur1Ch0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: ChangePosUCur0Ch1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: ChangePosUCur1Ch1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: close(); break;
        case 20: SetColorBackground(); break;
        case 21: SetColorGrid(); break;
        case 22: SetColorSignal(); break;
        case 23: ChangePosUCurFromMouse0((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: ChangePosUCurFromMouse1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: Calibration(); break;
        default: ;
        }
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void WindowRegistrator::terminate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
