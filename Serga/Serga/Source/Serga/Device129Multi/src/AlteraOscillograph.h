//  [12/12/2009 Sasha]
#pragma once
#include "AbstractAltera.h"
#include "DataOscillograph.h"
#include "../../common/TablesOfConverting.h"

class AlteraOscillograph : public AbstractAltera 
{
    friend class ThreadAutoSearch;
    friend class MathMeasures;

public:
    AlteraOscillograph();
    virtual bool Start(bool first = true);
    bool DataReady(BYTE &flag);
    DataOscillograph* GetData();
    void ResetAddrMinMax()
    { 
        settings.addr_Min = 200;
        addrMin = -1;
        settings.addr_Max = 400;
        addrMax = -1; 
    };

private:
    DataOscillograph data;
    int j;
    int addr0;
    double addrMin;
    double addrMax;
    bool ReadRealTime();
    void ReadRandomize();
    void ReadNPoints();
    void SetAbsoluteData();
};