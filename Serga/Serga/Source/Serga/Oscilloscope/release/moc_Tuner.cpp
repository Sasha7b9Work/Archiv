/****************************************************************************
** Meta object code from reading C++ file 'Tuner.h'
**
** Created: Wed Jun 27 09:43:26 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Tuner.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Tuner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tuner[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      22,    6,    6,    6, 0x0a,
      38,    6,    6,    6, 0x0a,
      53,    6,    6,    6, 0x0a,
      68,    6,    6,    6, 0x0a,
      84,    6,    6,    6, 0x0a,
      99,    6,    6,    6, 0x0a,
     115,    6,    6,    6, 0x0a,
     130,    6,    6,    6, 0x0a,
     144,    6,    6,    6, 0x0a,
     159,    6,    6,    6, 0x0a,
     173,    6,    6,    6, 0x0a,
     189,    6,    6,    6, 0x0a,
     213,    6,    6,    6, 0x0a,
     234,    6,    6,    6, 0x0a,
     252,    6,    6,    6, 0x0a,
     261,    6,    6,    6, 0x0a,
     270,    6,    6,    6, 0x0a,
     279,    6,    6,    6, 0x0a,
     288,    6,    6,    6, 0x0a,
     301,    6,    6,    6, 0x0a,
     314,    6,    6,    6, 0x0a,
     329,    6,    6,    6, 0x0a,
     347,    6,    6,    6, 0x0a,
     372,    6,    6,    6, 0x0a,
     388,    6,    6,    6, 0x0a,
     404,    6,    6,    6, 0x0a,
     421,    6,    6,    6, 0x0a,
     438,    6,    6,    6, 0x0a,
     454,    6,    6,    6, 0x0a,
     470,    6,    6,    6, 0x0a,
     486,    6,    6,    6, 0x0a,
     510,    6,    6,    6, 0x0a,
     534,    6,    6,    6, 0x0a,
     560,    6,    6,    6, 0x0a,
     590,    6,    6,    6, 0x0a,
     609,    6,    6,    6, 0x0a,
     628,    6,    6,    6, 0x0a,
     647,    6,    6,    6, 0x0a,
     667,    6,    6,    6, 0x0a,
     689,    6,    6,    6, 0x0a,
     714,    6,    6,    6, 0x0a,
     740,    6,    6,    6, 0x0a,
     769,  766,    6,    6, 0x0a,
     796,  766,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Tuner[] = {
    "Tuner\0\0SetRange0(int)\0SetRShift0(int)\0"
    "ResetRShift0()\0SetRange1(int)\0"
    "SetRShift1(int)\0ResetRShift1()\0"
    "SetTrigLev(int)\0ResetTrigLev()\0"
    "SetTBase(int)\0SetTShift(int)\0ResetTShift()\0"
    "SetTShift0(int)\0SetSinchroChannel(bool)\0"
    "SetSinchroType(bool)\0SetStartMode(int)\0"
    "SetAC0()\0SetAC1()\0SetDC0()\0SetDC1()\0"
    "SetGround0()\0SetGround1()\0SetFiltr(bool)\0"
    "EnableSpectr(int)\0ChangeChannelSpectr(int)\0"
    "EnableCh0(bool)\0EnableCh1(bool)\0"
    "DisableCh0(bool)\0DisableCh1(bool)\0"
    "InvertCh0(bool)\0InvertCh1(bool)\0"
    "EnableMath(int)\0ChangeMathFunction(int)\0"
    "EnableAccumulation(int)\0"
    "SetCountAccumulation(int)\0"
    "DisableAccumulationLimit(int)\0"
    "SetModePalm0(bool)\0SetModePalm1(bool)\0"
    "SetPointsPalm(int)\0SetModeAverage(int)\0"
    "SetNumberAverage(int)\0ChangeTShiftDisplay(int)\0"
    "ChangeRShift0Display(int)\0"
    "ChangeRShift1Display(int)\0,,\0"
    "ChangePosUCur(int,int,int)\0"
    "ChangePosTCur(int,int,int)\0"
};

const QMetaObject Tuner::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Tuner,
      qt_meta_data_Tuner, 0 }
};

const QMetaObject *Tuner::metaObject() const
{
    return &staticMetaObject;
}

void *Tuner::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tuner))
        return static_cast<void*>(const_cast< Tuner*>(this));
    return QObject::qt_metacast(_clname);
}

int Tuner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SetRange0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: SetRShift0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: ResetRShift0(); break;
        case 3: SetRange1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: SetRShift1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: ResetRShift1(); break;
        case 6: SetTrigLev((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: ResetTrigLev(); break;
        case 8: SetTBase((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: SetTShift((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: ResetTShift(); break;
        case 11: SetTShift0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: SetSinchroChannel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: SetSinchroType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: SetStartMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: SetAC0(); break;
        case 16: SetAC1(); break;
        case 17: SetDC0(); break;
        case 18: SetDC1(); break;
        case 19: SetGround0(); break;
        case 20: SetGround1(); break;
        case 21: SetFiltr((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: EnableSpectr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: ChangeChannelSpectr((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: EnableCh0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: EnableCh1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: DisableCh0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: DisableCh1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: InvertCh0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: InvertCh1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: EnableMath((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: ChangeMathFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: EnableAccumulation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: SetCountAccumulation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: DisableAccumulationLimit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: SetModePalm0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: SetModePalm1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: SetPointsPalm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: SetModeAverage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: SetNumberAverage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: ChangeTShiftDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: ChangeRShift0Display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: ChangeRShift1Display((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: ChangePosUCur((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 44: ChangePosTCur((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 45;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
