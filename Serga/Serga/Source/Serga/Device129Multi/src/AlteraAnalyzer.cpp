//  [12/12/2009 Sasha]
#include "AlteraAnalyzer.h"
#include "DataAnalyzer.h"
#include "../common/BinaryMathematics.h"
#include "../../common/TablesOfConverting.h"

using namespace NS_TypesAltera;
using namespace NS_AlteraAddressSpace;
using namespace NS_BinaryMathematics;

extern TablesOfConverting tables;

AlteraAnalyzer::AlteraAnalyzer() : AbstractAltera()
{
}

bool AlteraAnalyzer::Start(bool _first) 
{
    data.isReady = false;
    return AbstractAltera::Start(_first);
}

bool AlteraAnalyzer::DataReady(bool &_isAnswer, bool &_isSyncro) 
{
    BYTE flags[8];
    ZeroMemory(flags, 8);
    _isAnswer = Read(READ_FL, 8, flags);
    _isSyncro = GetBit(flags[0], 1);
    return ((flags[7]&0x03) == 0x03);
}

DataAnalyzer* AlteraAnalyzer::GetData() 
{
    Read(READ_CONTR1, 512, data.data[0]);
    Read(READ_CONTR2, 512, data.data[1]);
    int rShift0 = settings.GetRShift(0);
    int rShift1 = settings.GetRShift(1);
    int range0 = settings.GetRange(0);
    int range1 = settings.GetRange(1);
    for(int pos = 0; pos < DB_LENGTH; pos++) 
    {
        data.dataAbs[0][pos] = tables.GetAbsVoltage(data.data[0][pos], range0, rShift0);
        data.dataAbs[1][pos] = tables.GetAbsVoltage(data.data[1][pos], range1, rShift1);
    }
    data.isReady = true;
    return &data;
}

void AlteraAnalyzer::SetFBase(int _fBase)
{
    SetTBase(tables.GetTBaseFromFBase(_fBase));
}

int AlteraAnalyzer::GetFBase()
{
    return tables.GetFBaseFromTBase(settings.GetTBase());
}