#include "AlteraOscillograph.h"
#include "../../common/BinaryMathematics.h"
#include "../../common/TypesAltera.h"
#include "AddressSpaceAltera.h"
#include "../../common/TablesOfConverting.h"
#include <algorithm>

using namespace NS_TypesAltera;
using namespace NS_AlteraAddressSpace;
using namespace NS_BinaryMathematics;

extern TablesOfConverting tables;

AlteraOscillograph::AlteraOscillograph() : 
    AbstractAltera()
{
    ResetAddrMinMax();
}

bool AlteraOscillograph::Start(bool _first) 
{
    if(_first)
        data.Clear();
    return AbstractAltera::Start(_first);
}

bool AlteraOscillograph::DataReady(BYTE &_flag) 
{
    return AbstractAltera::DataReady(_flag);
}

DataOscillograph* AlteraOscillograph::GetData() 
{
    bool normalReading = false;

    switch(settings.modeADC) 
    {
        case ModeADC_RealTime:
            normalReading = ReadRealTime();
            SetAbsoluteData();
            break;
        case ModeADC_Randomize:
            if(StartMode_Single == settings.startMode) 
            {
                normalReading = ReadRealTime();
                SetAbsoluteData();
            }
            else 
            {
                normalReading = true;
                ReadRandomize();
                SetAbsoluteData();
                data.AdditionData();
            }
            break;
    }
    data.isReady = normalReading;
    return &data;
}

bool AlteraOscillograph::ReadRealTime() 
{
    
    if(!Read(READ_CONTR1, 512, data.data[0]))
        return false;
    
    if(!Read(READ_CONTR2, 512, data.data[1]))
        return false;

    return true;
}

void AlteraOscillograph::ReadRandomize() 
{
    static const int MAX_Z = 2048;
    static int z = MAX_Z;
    static DWORD tableJ[TBaseSize] = {100, 50, 20, 10, 5, 2};

    BYTE rdInt1 = 0;
    BYTE rdInt2 = 0;
    unsigned int addrI = 0;
    int counter = 0;

    while(!Read(READ_INT1, 1, &rdInt1) && (counter < 5))
        counter++;

    counter = 0;

    while(!Read(READ_INT2, 1, &rdInt2) && (counter < 5))
        counter++;

    addrI = (((UINT)(rdInt2)) << 8) | (UINT)rdInt1;

    if((addrI <= 100) || (addrI >= 500)) 
    {
        Start(false);
        return;                    // »гнорируем отсчЄт
    }

    if(addrMin < 0)
    {
        addrMin = addrI;
        addrMax = addrI;
    }
    else if(addrI < addrMin)
        addrMin = addrI;

    else if(addrI > addrMax)
        addrMax = addrI;

    z--;
    if(z == 0) 
    {
        z = MAX_Z;

        settings.addr_Min = addrMin;
        settings.addr_Max = addrMax;

        Start(false);
        addrMin = -1;
        addrMax = -1;
        return;
    }
    j = tableJ[settings.GetTBase()];

    double nominator = (int)addrI - ((int)settings.addr_Min + 4);
    double denominator = (int)(settings.addr_Max - 4) - (int)(settings.addr_Min + 4);
    addr0 = (double)(nominator / denominator * j);
    ReadNPoints();
    data.isReady = true;
    return;          
}

void AlteraOscillograph::ReadNPoints() 
{
    static DWORD tableR[TBaseSize] = {5, 10, 25, 50, 100, 250};

    int tBase = settings.GetTBase();
    DWORD r = tableR[tBase];

    static DWORD tableReadBytes[6] = {8, 16, 32, 64, 128, 256};
    BYTE buffer1[512];
    if(Read(READ_CONTR1, tableReadBytes[settings.GetTBase()], buffer1)) 
    {
        Write(WRITE_STOP, 0);           // Ёто действие по€вилось потому, что точки по€вл€лись из первого канала во втором
        BYTE buffer2[512];
        if(Read(READ_CONTR2, tableReadBytes[settings.GetTBase()], buffer2)) 
        {
            for(DWORD i = 0; i <= r; i++) 
            {
                int pos = addr0 + j * i - shift;
                if(pos < DB_LENGTH && pos >= 0)
                {
                    data.data[0][pos] = buffer1[i];
                    data.data[1][pos] = buffer2[i];
                    data.readPoint[pos] = true;
                    data.drawPoints[pos] = true;
                }
            }

            double count = 0;
            for(int i = 0; i < DB_SIGNED_LENGTH; i++)
                if(data.readPoint[i] == true)
                    count++;
            data.fraction = count / DB_SIGNED_LENGTH;
        }
    }
}

void AlteraOscillograph::SetAbsoluteData() 
{
    int range0 = settings.GetRange(0);
    int range1 = settings.GetRange(1);
    int rShift0 = settings.GetRShift(0);
    int rShift1 = settings.GetRShift(1);
    double k0 = settings.GetK(0);
    double k1 = settings.GetK(1);

    for(int pos = 0; pos < DB_SIGNED_LENGTH; pos++) 
    {
        data.dataAbs[0][pos] = tables.GetAbsVoltage(data.data[0][pos], range0, rShift0) * k0;
        data.dataAbs[1][pos] = tables.GetAbsVoltage(data.data[1][pos], range1, rShift1) * k1;
    }
}