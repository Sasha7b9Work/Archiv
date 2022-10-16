//  [6/24/2009 Sasha]
#pragma warning(disable:4127)
#include <QObject>
#pragma warning(default:4127)
#include <cmath>
#include "MathDisplayAnalyzer.h"
#include "../../common/GlobalFunctions.h"
#include "../common/TypesAltera.h"
#include "../common/TablesOfConverting.h"

using NS_TypesAltera::RShiftZero;
using NS_GlobalFunctions::VoltageToString;
using NS_GlobalFunctions::Limitation;

extern TablesOfConverting tables;

double MathDisplayAnalyzer::GetMaxVoltage(int range) 
{
    return tables.GetAbsVoltage(NS_TypesAltera::MAX_VALUE, range, RShiftZero);
}

double MathDisplayAnalyzer::GetMaxTime(int) 
{
    double retValue = 0.0;
    return retValue;
}

QString MathDisplayAnalyzer::GetRShiftAbs(const int rShift, const int range) 
{
    return VoltageToString(tables.GetRShiftAbs(rShift, range));
}

int MathDisplayAnalyzer::GetRShiftRel(const int rShiftPrev, const int prevRange, const int currentRange) 
{
    double rShiftAbs = tables.GetRShiftAbs(rShiftPrev, prevRange);
    int rShiftCurrent = tables.GetRShiftRel(rShiftAbs, currentRange);
    return Limitation(rShiftCurrent, NS_TypesAltera::RShiftMin, NS_TypesAltera::RShiftMax);
}

QString MathDisplayAnalyzer::GetTShiftAbs(const int tShift, const int tBase) 
{
    return NS_GlobalFunctions::TimeToString(tables.GetTShiftAbs(tShift, tBase));
}

int MathDisplayAnalyzer::GetTShiftRel(const int tShiftPrev, const int prevTBase, const int currentTBase) 
{
    double tShiftAbs = tables.GetTShiftAbs(tShiftPrev, prevTBase);
    int tShiftCurrent = tables.GetTShiftRel(tShiftAbs, currentTBase);
    return Limitation(tShiftCurrent, NS_TypesAltera::TShiftMin, NS_TypesAltera::TShiftMax);
}

QString MathDisplayAnalyzer::GetPosUCur(int pos, int range) 
{
    return VoltageToString(GetPosUCurAbs(pos, range));
}

double MathDisplayAnalyzer::GetPosUCurAbs(int pos, int range) 
{
    double retValue = tables.GetRShiftAbs(pos * 4 + RShiftZero, range);
    return retValue;
}

int MathDisplayAnalyzer::GetPosUCurRel(double pos, int range) 
{
    return (tables.GetRShiftRel(pos, range) - RShiftZero) / 4;
}

QString MathDisplayAnalyzer::GetDeltaU(int pos1, int pos2, int range) 
{
    return GetPosUCur(pos2 - pos1, range);
}

//  [8/18/2009 Sasha]
int MathDisplayAnalyzer::GetPosUCurNew(int prevPos, int prevRange, int currentRange) {
    double absPos = GetPosUCurAbs(prevPos, prevRange);
    int relPos = GetPosUCurRel(absPos, currentRange);
    return relPos;
}

double MathDisplayAnalyzer::GetPosTCurAbs(int pos, int tBase) 
{
    return tables.GetTShiftAbs(pos + NS_TypesAltera::TShiftZero, tBase);
}

int MathDisplayAnalyzer::GetPosTCurRel(double pos, int tBase) 
{
    return tables.GetTShiftRel(pos, tBase);
}

int MathDisplayAnalyzer::GetPosTCurNew(int prevPos, int prevTBase, int currentTBase) 
{
    double absPos = GetPosTCurAbs(prevPos, prevTBase);
    int relPos = GetPosTCurRel(absPos, currentTBase);
    return relPos - 500;
}
