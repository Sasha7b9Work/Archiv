/****************************************************************************
** Meta object code from reading C++ file 'Display.h'
**
** Created: Wed Jun 27 09:43:27 2012
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
      17,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      19,    8,    8,    8, 0x0a,
      40,    8,    8,    8, 0x0a,
      55,    8,    8,    8, 0x0a,
      70,    8,    8,    8, 0x0a,
      87,    8,    8,    8, 0x0a,
     105,    8,    8,    8, 0x0a,
     123,    8,    8,    8, 0x0a,
     138,    8,    8,    8, 0x0a,
     154,    8,    8,    8, 0x0a,
     170,    8,    8,    8, 0x0a,
     189,    8,    8,    8, 0x0a,
     206,    8,    8,    8, 0x0a,
     224,    8,    8,    8, 0x0a,
     244,    8,    8,    8, 0x0a,
     262,    8,    8,    8, 0x0a,
     287,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Display[] = {
    "Display\0\0Repaint()\0SetColorBackground()\0"
    "SetColorGrid()\0SetColorTPos()\0"
    "SetColorTShift()\0SetColorSignal0()\0"
    "SetColorSignal1()\0SetColorMath()\0"
    "SetColorCurs0()\0SetColorCurs1()\0"
    "SetColorCursMath()\0SetDrawMode(int)\0"
    "SetTypeGridBase()\0SetTypeGridSimple()\0"
    "SetTypeGridNone()\0EnableAntialiasing(bool)\0"
    "ResetAccumulation()\0"
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
        case 0: Repaint(); break;
        case 1: SetColorBackground(); break;
        case 2: SetColorGrid(); break;
        case 3: SetColorTPos(); break;
        case 4: SetColorTShift(); break;
        case 5: SetColorSignal0(); break;
        case 6: SetColorSignal1(); break;
        case 7: SetColorMath(); break;
        case 8: SetColorCurs0(); break;
        case 9: SetColorCurs1(); break;
        case 10: SetColorCursMath(); break;
        case 11: SetDrawMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: SetTypeGridBase(); break;
        case 13: SetTypeGridSimple(); break;
        case 14: SetTypeGridNone(); break;
        case 15: EnableAntialiasing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: ResetAccumulation(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
