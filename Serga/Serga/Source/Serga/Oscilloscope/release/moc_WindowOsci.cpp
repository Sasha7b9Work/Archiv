/****************************************************************************
** Meta object code from reading C++ file 'WindowOsci.h'
**
** Created: Wed Jun 27 09:43:26 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/WindowOsci.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowOsci.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowOsci[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      20,   11,   11,   11, 0x08,
      34,   11,   11,   11, 0x08,
      53,   11,   11,   11, 0x08,
      66,   11,   11,   11, 0x08,
      76,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     143,   11,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,
     177,   11,   11,   11, 0x08,
     195,   11,   11,   11, 0x08,
     216,   11,   11,   11, 0x08,
     235,   11,   11,   11, 0x08,
     253,   11,   11,   11, 0x08,
     270,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowOsci[] = {
    "WindowOsci\0\0close()\0UpdatePorts()\0"
    "SetNumberPort(int)\0SaveScreen()\0"
    "OnTimer()\0Start(bool)\0SetModeCalibrator_4V_0Hz()\0"
    "SetModeCalibrator_4V_5kHz()\0Calibrate()\0"
    "LoadDefaultSettings()\0AutoSearchEnter()\0"
    "AutoSearchExit(bool)\0NewOutMeasure(int)\0"
    "OnCalibrateZero()\0OnChangeTab(int)\0"
    "OnChangeMeasure(int)\0"
};

const QMetaObject WindowOsci::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WindowOsci,
      qt_meta_data_WindowOsci, 0 }
};

const QMetaObject *WindowOsci::metaObject() const
{
    return &staticMetaObject;
}

void *WindowOsci::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowOsci))
        return static_cast<void*>(const_cast< WindowOsci*>(this));
    if (!strcmp(_clname, "Ui::WindowOsci"))
        return static_cast< Ui::WindowOsci*>(const_cast< WindowOsci*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WindowOsci::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: close(); break;
        case 1: UpdatePorts(); break;
        case 2: SetNumberPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: SaveScreen(); break;
        case 4: OnTimer(); break;
        case 5: Start((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: SetModeCalibrator_4V_0Hz(); break;
        case 7: SetModeCalibrator_4V_5kHz(); break;
        case 8: Calibrate(); break;
        case 9: LoadDefaultSettings(); break;
        case 10: AutoSearchEnter(); break;
        case 11: AutoSearchExit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: NewOutMeasure((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: OnCalibrateZero(); break;
        case 14: OnChangeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: OnChangeMeasure((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
