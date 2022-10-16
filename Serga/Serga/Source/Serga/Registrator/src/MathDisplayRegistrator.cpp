#include "MathDisplayRegistrator.h"
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)
#include <cmath>
#include "../../common/TablesOfConverting.h"
#include "../../common/GlobalFunctions.h"

using namespace NS_TypesAltera;
using namespace NS_GlobalFunctions;

extern TablesOfConverting tables;

MathDisplayRegistrator::MathDisplayRegistrator()
{

}

MathDisplayRegistrator::~MathDisplayRegistrator() 
{

}

double MathDisplayRegistrator::GetMaxVoltage(int _range) 
{
    return tables.GetAbsVoltage(MAX_VALUE, _range, RShiftZero);
}

double MathDisplayRegistrator::GetMaxTime(int) 
{
    double retValue = 0.0;
    return retValue;
}

QString MathDisplayRegistrator::GetRShiftAbs(const int rShift, const int range) 
{
    return VoltageToString(tables.GetRShiftAbs(rShift, range));
}

int MathDisplayRegistrator::GetRShiftRel(const int rShiftPrev, const int prevRange, const int currentRange) 
{
    double rShiftAbs = tables.GetRShiftAbs(rShiftPrev, prevRange);
    int rShiftCurrent = tables.GetRShiftRel(rShiftAbs, currentRange);
    return Limitation(rShiftCurrent, RShiftMin, RShiftMax);
}

QString MathDisplayRegistrator::GetTShiftAbs(const int tShift, const int tBase) 
{
    return TimeToString(tables.GetTShiftAbs(tShift, tBase));
}

int MathDisplayRegistrator::GetTShiftRel(const int tShiftPrev, const int prevTBase, const int currentTBase) 
{
    double tShiftAbs = tables.GetTShiftAbs(tShiftPrev, prevTBase);
    int tShiftCurrent = tables.GetTShiftRel(tShiftAbs, currentTBase);
    return Limitation(tShiftCurrent, TShiftMin, TShiftMax);
}

QString MathDisplayRegistrator::GetPosUCur(int pos, int range) 
{
    return VoltageToString(GetPosUCurAbs(pos, range));
}

double MathDisplayRegistrator::GetPosUCurAbs(int pos, int range) 
{
    double retValue = tables.GetRShiftAbs(pos * 4 + RShiftZero, range);
    return retValue;
}

int MathDisplayRegistrator::GetPosUCurRel(double pos, int range) 
{
    return (tables.GetRShiftRel(pos, range) - RShiftZero) / 4;
}

QString MathDisplayRegistrator::GetDeltaU(int pos1, int pos2, int range) 
{
    return GetPosUCur(pos2 - pos1, range);
}

int MathDisplayRegistrator::GetPosUCurNew(int prevPos, int prevRange, int currentRange) 
{
    double absPos = GetPosUCurAbs(prevPos, prevRange);
    int relPos = GetPosUCurRel(absPos, currentRange);
    return relPos;
}

double MathDisplayRegistrator::GetPosTCurAbs(int pos, int tBase) {
    return tables.GetTShiftAbs(pos + TShiftZero, tBase);
}

int MathDisplayRegistrator::GetPosTCurRel(double pos, int tBase) 
{
    return tables.GetTShiftRel(pos, tBase);
}

int MathDisplayRegistrator::GetPosTCurNew(int prevPos, int prevTBase, int currentTBase) 
{
    double absPos = GetPosTCurAbs(prevPos, prevTBase);
    int relPos = GetPosTCurRel(absPos, currentTBase);
    return relPos - 500;
}
