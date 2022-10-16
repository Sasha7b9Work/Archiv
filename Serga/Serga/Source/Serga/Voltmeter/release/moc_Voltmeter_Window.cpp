/****************************************************************************
** Meta object code from reading C++ file 'Voltmeter_Window.h'
**
** Created: Wed Jun 27 09:43:17 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Voltmeter_Window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Voltmeter_Window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Voltmeter_Window[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      37,   17,   32,   17, 0x08,
      53,   17,   17,   17, 0x08,
      65,   17,   17,   17, 0x08,
      75,   17,   17,   17, 0x08,
      86,   17,   17,   17, 0x08,
      97,   17,   17,   17, 0x08,
     120,   17,   17,   17, 0x08,
     138,   17,   17,   17, 0x08,
     156,   17,   17,   17, 0x08,
     177,   17,   17,   17, 0x08,
     194,   17,   17,   17, 0x08,
     209,   17,   17,   17, 0x08,
     224,   17,   17,   17, 0x08,
     236,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Voltmeter_Window[] = {
    "Voltmeter_Window\0\0UpdatePorts()\0bool\0"
    "SetComPort(int)\0Start(bool)\0OnTimer()\0"
    "SetU0(int)\0SetU1(int)\0SetSinchroChannel(int)\0"
    "SetDC_Mode0(bool)\0SetDC_Mode1(bool)\0"
    "SetColorBackground()\0SetColorSignal()\0"
    "SetColorGrid()\0SetColorFont()\0Calibrate()\0"
    "OnTimerCalibrateZero()\0"
};

const QMetaObject Voltmeter_Window::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Voltmeter_Window,
      qt_meta_data_Voltmeter_Window, 0 }
};

const QMetaObject *Voltmeter_Window::metaObject() const
{
    return &staticMetaObject;
}

void *Voltmeter_Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Voltmeter_Window))
        return static_cast<void*>(const_cast< Voltmeter_Window*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Voltmeter_Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: UpdatePorts(); break;
        case 1: { bool _r = SetComPort((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: Start((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: OnTimer(); break;
        case 4: SetU0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: SetU1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: SetSinchroChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: SetDC_Mode0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: SetDC_Mode1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: SetColorBackground(); break;
        case 10: SetColorSignal(); break;
        case 11: SetColorGrid(); break;
        case 12: SetColorFont(); break;
        case 13: Calibrate(); break;
        case 14: OnTimerCalibrateZero(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
