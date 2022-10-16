/****************************************************************************
** Meta object code from reading C++ file 'WindowAnalyzer.h'
**
** Created: Wed Jun 27 09:42:49 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/WindowAnalyzer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowAnalyzer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowAnalyzer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   15,   15,   15, 0x08,
      40,   15,   15,   15, 0x08,
      50,   15,   15,   15, 0x08,
      69,   15,   15,   15, 0x08,
      83,   15,   15,   15, 0x08,
      96,   15,   15,   15, 0x08,
     106,   15,   15,   15, 0x08,
     127,   15,   15,   15, 0x08,
     142,   15,   15,   15, 0x08,
     161,   15,   15,   15, 0x08,
     175,   15,   15,   15, 0x08,
     196,   15,   15,   15, 0x08,
     211,   15,   15,   15, 0x08,
     244,  228,   15,   15, 0x08,
     265,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowAnalyzer[] = {
    "WindowAnalyzer\0\0terminate()\0Start(bool)\0"
    "OnTimer()\0SetNumberPort(int)\0Calibration()\0"
    "SetFreq(int)\0SetU(int)\0SetActiveSpectr(int)\0"
    "SetWindow(int)\0EnableCursors(int)\0"
    "UpdatePorts()\0SetColorBackground()\0"
    "SetColorGrid()\0SetColorSignal()\0"
    "cursor,position\0MovedCursor(int,int)\0"
    "close()\0"
};

const QMetaObject WindowAnalyzer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_WindowAnalyzer,
      qt_meta_data_WindowAnalyzer, 0 }
};

const QMetaObject *WindowAnalyzer::metaObject() const
{
    return &staticMetaObject;
}

void *WindowAnalyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowAnalyzer))
        return static_cast<void*>(const_cast< WindowAnalyzer*>(this));
    if (!strcmp(_clname, "Ui::WindowAnalyzer"))
        return static_cast< Ui::WindowAnalyzer*>(const_cast< WindowAnalyzer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int WindowAnalyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: terminate(); break;
        case 1: Start((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: OnTimer(); break;
        case 3: SetNumberPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: Calibration(); break;
        case 5: SetFreq((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: SetU((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: SetActiveSpectr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: SetWindow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: EnableCursors((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: UpdatePorts(); break;
        case 11: SetColorBackground(); break;
        case 12: SetColorGrid(); break;
        case 13: SetColorSignal(); break;
        case 14: MovedCursor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: close(); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void WindowAnalyzer::terminate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
