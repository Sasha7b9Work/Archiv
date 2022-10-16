//  [12/13/2009 Sasha]
#include "AlteraRegistrator.h"
#include "../../common/BinaryMathematics.h"
#include "../../common/TablesOfConverting.h"
#include <algorithm>

using namespace std;

using namespace NS_AlteraAddressSpace;
using namespace NS_BinaryMathematics;

extern TablesOfConverting tables;

//  [12/13/2009 Sasha]
AlteraRegistrator::AlteraRegistrator() : AbstractAltera() {
}

//  [12/13/2009 Sasha]
bool AlteraRegistrator::DataReady() {
    if(!running) {
        return false;
    }
    BYTE flag[8];
    bool readed = Read(READ_FL, 8, flag);
    static const int NUM_P = 512;
    if(readed && (GetBit(flag[0], 0))) 
    {
        BYTE data0[NUM_P];

        while(!Read(READ_CONTR1, NUM_P, data0))
        {
        }
        data[0] = tables.GetAbsVoltage(Average(data0, NUM_P), settings.GetRange(0), settings.GetRShift(0));

        BYTE data1[NUM_P];

        while(!Read(READ_CONTR2, NUM_P, data1))
        {
        }
        data[1] = tables.GetAbsVoltage(Average(data1, NUM_P), settings.GetRange(1), settings.GetRShift(1));

        return true;
    }
    else 
    {
        return false;
    }
}

//  [12/13/2009 Sasha]
double AlteraRegistrator::GetValue(int _channel) {
    return data[_channel];
}