//  [6/24/2009 Sasha]
#include "MathDisplayOscilloscope.h"
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)
#include <cmath>
#include "GlobalFunctions.h"
#include "TypesAltera.h"
#include "TablesOfConverting.h"

using NS_GlobalFunctions::VoltageToString;
using NS_GlobalFunctions::Limitation;
using NS_GlobalFunctions::TimeToString;
using namespace NS_TypesAltera;

#define tr QObject::tr

extern TablesOfConverting tables;
MathDisplayOscilloscope mathDisplay;

MathDisplayOscilloscope::MathDisplayOscilloscope()
{
    mapRangesString[Range_5mv] = QString(tr("5mV/del"));
    mapRangesString[Range_10mv] = QString(tr("10mV/del"));
    mapRangesString[Range_20mv] = QString(tr("20mV/del"));
    mapRangesString[Range_50mv] = QString(tr("50mV/del"));
    mapRangesString[Range_100mv] = QString(tr("100mV/del"));
    mapRangesString[Range_200mv] = QString(tr("200mV/del"));
    mapRangesString[Range_500mv] = QString(tr("500mV/del"));
    mapRangesString[Range_1v] = QString(tr("1V/del"));
    mapRangesString[Range_2v] = QString(tr("2V/del"));
    mapRangesString[Range_5v] = QString(tr("5V/del"));

    mapTBaseString[TBase_10ns] = QString(tr("10ns/del"));
    mapTBaseString[TBase_20ns] = QString(tr("20ns/del"));
    mapTBaseString[TBase_50ns] = QString(tr("50ns/del"));
    mapTBaseString[TBase_100ns] = QString(tr("100ns/del"));
    mapTBaseString[TBase_200ns] = QString(tr("200ns/del"));
    mapTBaseString[TBase_500ns] = QString(tr("500ns/del"));
    mapTBaseString[TBase_1us] = QString(tr("1us/del"));
    mapTBaseString[TBase_2us] = QString(tr("2us/del"));
    mapTBaseString[TBase_5us] = QString(tr("5us/del"));
    mapTBaseString[TBase_10us] = QString(tr("10us/del"));
    mapTBaseString[TBase_20us] = QString(tr("20us/del"));
    mapTBaseString[TBase_50us] = QString(tr("50us/del"));
    mapTBaseString[TBase_100us] = QString(tr("100us/del"));
    mapTBaseString[TBase_200us] = QString(tr("200us/del"));
    mapTBaseString[TBase_500us] = QString(tr("500us/del"));
    mapTBaseString[TBase_1ms] = QString(tr("1ms/del"));
    mapTBaseString[TBase_2ms] = QString(tr("2ms/del"));
    mapTBaseString[TBase_5ms] = QString(tr("5ms/del"));
    mapTBaseString[TBase_10ms] = QString(tr("10ms/del"));
    mapTBaseString[TBase_20ms] = QString(tr("20ms/del"));
    mapTBaseString[TBase_50ms] = QString(tr("50ms/del"));
    mapTBaseString[TBase_100ms] = QString(tr("100ms/del"));
    mapTBaseString[TBase_200ms] = QString(tr("200ms/del"));
    mapTBaseString[TBase_500ms] = QString(tr("500ms/del"));
    mapTBaseString[TBase_1s] = QString(tr("1s/del"));
    mapTBaseString[TBase_2s] = QString(tr("2s/del"));
}

MathDisplayOscilloscope::~MathDisplayOscilloscope() { }

//  [2/5/2010 Sasha]
QString MathDisplayOscilloscope::GetRangeString(int _range) {
    return mapRangesString[_range];
}

QString MathDisplayOscilloscope::GetTBaseString(int _tBase) 
{
    return mapTBaseString[_tBase];
}

double MathDisplayOscilloscope::GetMaxVoltage(int _range) 
{
    return tables.GetAbsVoltage(MAX_VALUE, _range, RShiftZero);
}

double MathDisplayOscilloscope::GetMaxTime(int) 
{
    double retValue = 0.0;
    return retValue;
}

QString MathDisplayOscilloscope::GetRShiftAbs(const int rShift, const int range) 
{
    return VoltageToString(tables.GetRShiftAbs(rShift, range));
}

int MathDisplayOscilloscope::GetRShiftRel(const int rShiftPrev, const int prevRange, const int currentRange) 
{
    double rShiftAbs = tables.GetRShiftAbs(rShiftPrev, prevRange);
    int rShiftCurrent = tables.GetRShiftRel(rShiftAbs, currentRange);
    return Limitation(rShiftCurrent, RShiftMin, RShiftMax);
}

QString MathDisplayOscilloscope::GetTShiftAbs(const int tShift, const int tBase) 
{
    return TimeToString(tables.GetTShiftAbs(tShift, tBase));
}

int MathDisplayOscilloscope::GetTShiftRel(const int tShiftPrev, const int prevTBase, const int currentTBase) 
{
    double tShiftAbs = tables.GetTShiftAbs(tShiftPrev, prevTBase);
    int tShiftCurrent = tables.GetTShiftRel(tShiftAbs, currentTBase);
    return Limitation(tShiftCurrent, TShiftMin, TShiftMax);
}

QString MathDisplayOscilloscope::GetPosUCur(int pos, int range) 
{
    return VoltageToString(GetPosUCurAbs(pos, range));
}

double MathDisplayOscilloscope::GetPosUCurAbs(int pos, int range) 
{
    double retValue = tables.GetRShiftAbs(pos * 4 + RShiftZero, range);
    return retValue;
}

int MathDisplayOscilloscope::GetPosUCurRel(double pos, int range) 
{
    return (tables.GetRShiftRel(pos, range) - RShiftZero) / 4;
}

QString MathDisplayOscilloscope::GetDeltaU(int pos1, int pos2, int range) 
{
    return GetPosUCur(pos2 - pos1, range);
}

int MathDisplayOscilloscope::GetPosUCurNew(int prevPos, int prevRange, int currentRange) 
{
    double absPos = GetPosUCurAbs(prevPos, prevRange);
    int relPos = GetPosUCurRel(absPos, currentRange);
    return relPos;
}

double MathDisplayOscilloscope::GetPosTCurAbs(int pos, int tBase) 
{
    return tables.GetTShiftAbs(pos + TShiftZero, tBase);
}

int MathDisplayOscilloscope::GetPosTCurRel(double pos, int tBase) 
{
    return tables.GetTShiftRel(pos, tBase);
}

int MathDisplayOscilloscope::GetPosTCurNew(int prevPos, int prevTBase, int currentTBase) 
{
    double absPos = GetPosTCurAbs(prevPos, prevTBase);
    int relPos = GetPosTCurRel(absPos, currentTBase);
    return relPos - 500;
}

#undef tr