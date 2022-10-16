#include "TablesOfConverting.h"

using namespace NS_TypesAltera;

//  [9/17/2009 Sasha]
TablesOfConverting::TablesOfConverting() {

    // Таблица масштабов - секунд на клетку
    static double tableScalesTBase[TBaseSize] = 
        {10e-9, 20e-9, 50e-9, 100e-9, 200e-9, 500e-9,
        1e-6, 2e-6, 5e-6, 10e-6, 20e-6, 50e-6, 100e-6, 200e-6, 500e-6,
        1e-3, 2e-3, 5e-3, 10e-3, 20e-3, 50e-3, 100e-3, 200e-3, 500e-3,
        1.0, 2.0};

    // Таблица масштабов - вольт на клетку
    static double tableScalesRange[RangeSize] = {5e-3, 10e-3, 20e-3, 50e-3, 100e-3, 200e-3, 500e-3, 1.0, 2.0, 5.0};

    for(int i = 0; i < TBaseSize; i++) {
        tableTShift[i].tBase = (TBase)i;
        tableTShift[i].absStep = tableScalesTBase[i] / 50.0;
        tableTShift[i].absMinTShift = -tableTShift[i].absStep * TShiftZero;
        tableTShift[i].absMaxTShift = tableTShift[i].absStep * TShiftMax;
    }

    for(int i = 0; i < RangeSize; i++) {
        double max = 0.0;
        double min = 0.0;
        tableRShift[i].range = (Range)i;

        tableRShift[i].absMinVoltageMeas = min = - tableScalesRange[i] * 5.0;
        tableRShift[i].absMaxVoltageMeas = max = - min;
        tableRShift[i].absStepVoltageMeas = (max - min) / (MAX_VALUE - MIN_VALUE);

        tableRShift[i].absMinRShift = min = - tableScalesRange[i] * 10.0;
        tableRShift[i].absMaxRShift = max = - min;
        tableRShift[i].absStepRShift = (max - min) / (RShiftMax - RShiftMin);
    }

    // Заполняем таблицу шагов для сетки частоты
    for(int i = 0; i < TBaseSize; i++) {
        double step = 1.0 / GetTShiftAbs(1000, i);
        tablesStepFreq.push_back(step);
    }

    // Заполняем массив для анализатора спектра
    rangeFromIndexFrequency.push_back(TBase_100ms);
    rangeFromIndexFrequency.push_back(TBase_20ms);
    rangeFromIndexFrequency.push_back(TBase_10ms);
    rangeFromIndexFrequency.push_back(TBase_2ms);
    rangeFromIndexFrequency.push_back(TBase_1ms);
    rangeFromIndexFrequency.push_back(TBase_200us);
    rangeFromIndexFrequency.push_back(TBase_100us);
    rangeFromIndexFrequency.push_back(TBase_20us);
    rangeFromIndexFrequency.push_back(TBase_10us);
    rangeFromIndexFrequency.push_back(TBase_2us);
    rangeFromIndexFrequency.push_back(TBase_1us);
}


double TablesOfConverting::GetStepFrequency(int _tBase) 
{
    return tablesStepFreq[_tBase];
}

double TablesOfConverting::GetStepTime(int _tBase)
{
    return tableTShift[_tBase].absStep;
}

int TablesOfConverting::GetTBaseFromFBase(int _fBase) 
{
    return rangeFromIndexFrequency[_fBase];
}

int TablesOfConverting::GetFBaseFromTBase(int _tBase)
{
    for(size_t i = 0; i < rangeFromIndexFrequency.size(); i++)
    {
        int tBase = rangeFromIndexFrequency[i];
        if(_tBase == tBase)
            return static_cast<FBase>(i);
    }
    return -1;
}

double TablesOfConverting::GetAbsVoltageDraw(DWORD relValue, int range) 
{
    /*
    В функцию передаётся относительное значение измереннной величины, полученное от осциллографа
    */
    if(relValue < MIN_VALUE) 
        relValue = MIN_VALUE - 1;
    else if(relValue > MAX_VALUE) 
        relValue = MAX_VALUE + 1;
    
    if(range < 0 || range >= RangeSize)
    {
        return 0.0;
    }
        
    double retValue = (tableRShift[range].absMinVoltageMeas + ((int)relValue - (int)MIN_VALUE) * tableRShift[range].absStepVoltageMeas);
    return retValue;
}

//  [9/17/2009 Sasha]
double TablesOfConverting::GetAbsVoltage(DWORD relValue, int range, int rShift) {
    if(relValue < MIN_VALUE) {
        relValue = MIN_VALUE - 1;
    }
    if(relValue > MAX_VALUE) {
        relValue = MAX_VALUE + 1;
    }
    double absVoltsDraw = GetAbsVoltageDraw(relValue, range);
    double altShift = (rShift - RShiftZero) * tableRShift[range].absStepRShift;
    double retValue = absVoltsDraw - altShift;
    return retValue;
}

//  [9/25/2009 Sasha]
int TablesOfConverting::GetRelVoltage(double absValue, int range) {
    return int(absValue / tableRShift[range].absStepVoltageMeas);
}

//  [9/21/2009 Sasha]
double TablesOfConverting::GetRShiftAbs(int rShift, int range) 
{
    return (rShift - RShiftZero) * tableRShift[range].absStepRShift;
}

//  [9/21/2009 Sasha]
int TablesOfConverting::GetRShiftRel(double rShiftAbs, int range) {
    return (int)(RShiftZero + rShiftAbs / tableRShift[range].absStepRShift);
}

//  [9/22/2009 Sasha]
double TablesOfConverting::GetTShiftAbs(int tShift, int tBase) {
    return tableTShift[tBase].absStep * (tShift - TShiftZero);
}

//  [9/22/2009 Sasha]
int TablesOfConverting::GetTShiftRel(double tShiftAbs, int tBase) {
    return (int)(tShiftAbs / tableTShift[tBase].absStep + TShiftZero);
}