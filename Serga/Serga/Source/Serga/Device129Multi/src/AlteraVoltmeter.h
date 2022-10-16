//  [12/9/2009 Sasha]
#pragma once
#include "AbstractAltera.h"
#include "../../common/TablesOfConverting.h"
#include "../../common/BinaryMathematics.h"
#include "../../common/TablesOfConverting.h"

extern TablesOfConverting tables;

class AlteraVoltmeter : public AbstractAltera 
{
public:
    AlteraVoltmeter() : AbstractAltera() {};
    bool DataReady(bool &_isAnswer, bool &_isSynchro)     // Возвращает true, если данные готовы
                                                        // в isAnswer возвращается false, если не пришёл ответ от прибора - так можно определить, что нет связи с прибором
                                                        // в isSychro возвращается true, если прошла синхронизация, и false в обратном случае
    {
        BYTE flags[8];
        ZeroMemory(flags, 8);
        _isAnswer = Read(NS_AlteraAddressSpace::READ_FL, 8, flags);
        _isSynchro = NS_BinaryMathematics::GetBit(flags[0], 1);
        return ((flags[7]&0x03) == 0x03);
    }
    bool DataReady();
    AbstractData *GetData()
    {
        Read(NS_AlteraAddressSpace::READ_CONTR1, 512, data.data[0]);
        Read(NS_AlteraAddressSpace::READ_CONTR2, 512, data.data[1]);
        int rShift0 = settings.GetRShift(0);
        int rShift1 = settings.GetRShift(1);
        int range0 = settings.GetRange(0);
        int range1 = settings.GetRange(1);
        BYTE *dataRel0 = data.data[0];
        BYTE *dataRel1 = data.data[1];
        double *dataAbs0 = data.dataAbs[0];
        double *dataAbs1 = data.dataAbs[1];
        double *dataDraw0 = data.dataDraw[0];
        double *dataDraw1 = data.dataDraw[1];
        for(int pos = 0; pos < NS_TypesAltera::DB_LENGTH; pos++) 
        {
            dataAbs0[pos] = tables.GetAbsVoltage(dataRel0[pos], range0, rShift0);
            dataAbs1[pos] = tables.GetAbsVoltage(dataRel1[pos], range1, rShift1);
            dataDraw0[pos] = ((double)dataRel0[pos] - NS_TypesAltera::MIN_VALUE) / (NS_TypesAltera::MAX_VALUE - NS_TypesAltera::MIN_VALUE);
            dataDraw1[pos] = ((double)dataRel1[pos] - NS_TypesAltera::MIN_VALUE) / (NS_TypesAltera::MAX_VALUE - NS_TypesAltera::MIN_VALUE);
        }
        return &data;
    };
private:
    AbstractData data;
};
