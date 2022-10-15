// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "DataStorage.h"
#include "FPGA_Types.h"
#include "FPGA.h"
#include "Menu/Pages/PageMemory.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Log.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Hardware/RTC.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_POOL   (30 * 1024)                     // ���������� ��������� ��� ��������� ������.


uint8 pool[SIZE_POOL] = {0};                        // ����� �������� ������.
static uint8* beginPool = &(pool[0]);               // ����� ������ ������ ��� ��������
static uint8* endPool = &(pool[SIZE_POOL - 1]);     // ����� ���������� ����� ������ ��� ��������

static uint  sum[NumChannels][FPGA_MAX_POINTS];        // ����� �������� ����� ��������� ����� �������
static uint8 limitUp[NumChannels][FPGA_MAX_POINTS];    // ������������ �������� �������
static uint8 limitDown[NumChannels][FPGA_MAX_POINTS];  // ����������� �������� �������

static DataSettings* firstElem;                     // ��������� �� ������ ���������� ������
static DataSettings* lastElem;                      // ��������� �� ��������� ���������� ������

static int allData = 0;                             // ����� ������ ���������

static float aveData0[FPGA_MAX_POINTS] = {0.0f};    // � ���� �������� �������� ���������� ��������, ������������ �� ���������������� ���������.
static float aveData1[FPGA_MAX_POINTS] = {0.0f};

static bool newSumCalculated[NumChannels] = {true, true};                  // ���� true, �� ����� ����� ����������, � ����� ��������� ������ ��������

static int MemoryFree(void);                                                // ���������� ���������� ��������� ������ � ������
static int SizeElem(DataSettings *dp);                                      // ���������, ������� ������ ���������, ����� ��������� ��������� � ����������� dp
static void RemoveFirstElement(void);                                       // ������� ������ (����� ������) ���������
static void PushData(DataSettings *dp, uint8 *data0, uint8 *data1);         // ��������� ������
static DataSettings* NextElem(DataSettings *elem);                          // ���������� ��������� �� ���������, ��������� �� elem
static DataSettings* FromEnd(int indexFromEnd);                             // ���������� ��������� �� ������, ��������� �� indexFromEnd o� ���������� �����������
static bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1);        // ���������� true, ���� ��������� ��������� � ��������� elemFromEnd0 � elemFromEnd1 ���������, � false � ���� ������.
static bool SettingsIsEquals(DataSettings *dp0, DataSettings *dp1);         // ��������� true, ���� ��������� � ����� ���������� ���������
static void ClearLimitsAndSums(void);                                       // ������� �������� ���, ���� � ����
static void CalculateLimits(uint8 *data0, uint8 *data1, DataSettings *dss);
static void CalculateSums(void);
static DataSettings* GetSettingsDataFromEnd(int fromEnd);
static bool CopyData(DataSettings *ds, Channel chan, uint8 datatImportRel[NumChannels][FPGA_MAX_POINTS]);  // �������� ������ ������ chan ��, ������������ ds, � ���� �� ���� ����� ������� dataImportRel. ������������ �������� false ��������, ��� ������ ����� ��������.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintElement(DataSettings *dp)
{
    if(dp == 0)
    {
        LOG_WRITE("������� �������");
    }
    else
    {
        LOG_WRITE("addr:%x, addrNext:%x, addrPrev:%x, size:%d", dp, dp->addrNext, dp->addrPrev, SizeElem(dp));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DS_Clear(void)
{
    firstElem = 0;
    lastElem = (DataSettings*)beginPool;
    lastElem->addrNext = lastElem->addrPrev = 0;
    ClearLimitsAndSums();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ClearLimitsAndSums(void)
{
    memset(limitUp[0], 0, FPGA_MAX_POINTS);
    memset(limitUp[1], 0, FPGA_MAX_POINTS);
    memset(limitDown[0], 0xff, FPGA_MAX_POINTS);
    memset(limitDown[1], 0xff, FPGA_MAX_POINTS);
    memset(&(sum[0][0]), 0, 2 * FPGA_MAX_POINTS * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateAroundAverage(uint8 *data0, uint8 *data1, DataSettings *dss)
{
    int numAveData = DS_NumElementsWithCurrentSettings();
    int size = dss->length1channel * (dss->peakDet == PeackDet_Disable ? 1 : 2);
    if (numAveData == 1)
    {
        for (int i = 0; i < size; i++)
        {
            aveData0[i] = data0[i];
            aveData1[i] = data1[i];
        }
    }
    else
    {
        if (numAveData > sDisplay_NumAverage())
        {
            numAveData = sDisplay_NumAverage();
        }
        float numAveDataF = numAveData;
        float numAveDataFless = numAveDataF - 1.0f;
        float numAveDataInv = 1.0f / numAveDataF;
        float* aData0 = &aveData0[0];
        float* aData1 = &aveData1[0];
        uint8* d0 = &data0[0];
        uint8* d1 = &data1[0];
        float* endData = &aveData0[size];

        do 
        {
            *aData0++ = ((*aData0) * numAveDataFless + (float)(*d0++)) * numAveDataInv;
            *aData1++ = ((*aData1) * numAveDataFless + (float)(*d1++)) * numAveDataInv;
        } while (aData0 != endData);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DS_AddData(uint8 *data0, uint8 *data1, DataSettings dss)
{
    uint time = gTimerTics;
    dss.time = RTC_GetPackedTime();

    if(dss.enableCh0 == 0 && dss.enableCh1 == 0)
    {
        return;
    }
    CalculateLimits(data0, data1, &dss);

    PushData(&dss, data0, data1);

    CalculateSums();

    CalculateAroundAverage(data0, data1, &dss);
    allData++;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DS_AllDatas(void)
{
    return allData;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateLimits(uint8 *data0, uint8 *data1, DataSettings *dss)
{
    int numElements = dss->length1channel * (dss->peakDet == PeackDet_Disable ? 1 : 2);

    if(DS_NumElementsInStorage() == 0 || NUM_MIN_MAX == 1 || (!SettingsIsEquals(dss, GetSettingsDataFromEnd(0))))
    {
        for(int i = 0; i < numElements; i++)
        {
            limitDown[0][i] = limitUp[0][i] = data0[i];
            limitDown[1][i] = limitUp[1][i] = data1[i];
        }
    }
    else
    {
        int allDatas = DS_NumElementsWithSameSettings();
        LIMITATION(allDatas, allDatas, 1, NUM_MIN_MAX);
        
        if(DS_NumElementsWithSameSettings() >= NUM_MIN_MAX)
        {
            for(int i = 0; i < numElements; i++)
            {
                limitDown[0][i] = limitUp[0][i] = data0[i];
                limitDown[1][i] = limitUp[1][i] = data1[i];
            }
            allDatas--;
        }
     
        for(int numData = 0; numData < allDatas; numData++)
        {
            const uint8 *dataA = DS_GetData(A, numData);
            const uint8 *dataB = DS_GetData(B, numData);
            for(int i = 0; i < numElements; i++)
            {
                if(dataA[i] < limitDown[0][i])  limitDown[0][i] = dataA[i];
                if(dataA[i] > limitUp[0][i])    limitUp[0][i] = dataA[i];
                if(dataB[i] < limitDown[1][i])  limitDown[1][i] = dataB[i];
                if(dataB[i] > limitUp[1][i])    limitUp[1][i] = dataB[i];
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateSums(void)
{
    DataSettings *ds = 0;
    uint8 *data0 = 0;
    uint8 *data1 = 0;

    DS_GetDataFromEnd(0, &ds, &data0, &data1);
    
    int numPoints = ds->length1channel * (ds->peakDet == PeackDet_Disable ? 1 : 2);

    int numAveragings = 0;

    if (sTime_RandomizeModeEnabled())
    {
        numAveragings = NUM_AVE_FOR_RAND;
    }

    if (sDisplay_NumAverage() > numAveragings)
    {
        numAveragings = sDisplay_NumAverage();
    }

    for(int i = 0; i < numPoints; i++)
    {
        sum[0][i] = data0[i];
        sum[1][i] = data1[i];
    }
    if(numAveragings > 1)
    {
        int numSameSettins = DS_NumElementsWithSameSettings();
        if(numSameSettins < numAveragings)
        {
            numAveragings = numSameSettins;
        }
        for(int i = 1; i < numAveragings; i++)
        {
            DS_GetDataFromEnd(i, &ds, &data0, &data1);
            for(int point = 0; point < numPoints; point++)
            {
                sum[0][point] += data0[point];
                sum[1][point] += data1[point];
            }
        }
    }
    newSumCalculated[0] = newSumCalculated[1] = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DS_NumElementsWithSameSettings(void)
{
    int retValue = 0;
    int numElements = DS_NumElementsInStorage();
    for(retValue = 1; retValue < numElements; retValue++)
    {
        if(!SettingsIsIdentical(retValue, retValue - 1))
        {
            break;
        }
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DS_NumElementsWithCurrentSettings(void)
{
    DataSettings dp;
    FPGA_FillDataPointer(&dp);
    int retValue = 0;
    int numElements = DS_NumElementsInStorage();
    for(retValue = 0; retValue < numElements; retValue++)
    {
        if(!SettingsIsEquals(&dp, FromEnd(retValue)))
        {
            break;
        }
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DS_NumElementsInStorage(void)
{
    int retValue = 0;
    DataSettings *elem = firstElem;
    if(firstElem != 0)
    {
        if(firstElem == lastElem)
        {
            retValue = 1;
        }
        else
        {
            retValue++;
            while((elem = NextElem(elem)) != 0)
            {
                retValue++;
            }
        }
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings* GetSettingsDataFromEnd(int fromEnd)
{
    return FromEnd(fromEnd);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool DS_GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1)
{
    static uint8 dataImportRel[2][FPGA_MAX_POINTS];

    DataSettings* dp = FromEnd(fromEnd);
    if(dp == 0)
    {
        return false;
    }

    if(data0 != 0)
    {
        *data0 = CopyData(dp, A, dataImportRel) ?  &dataImportRel[0][0] : 0;
    }
    if(data1 != 0)
    {
        *data1 = CopyData(dp, B, dataImportRel) ? &dataImportRel[1][0] : 0;
    }
    *ds = dp;
    
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8* DS_GetData(Channel chan, int fromEnd)
{
    static uint8 dataImportRel[2][FPGA_MAX_POINTS];
    DataSettings* dp = FromEnd(fromEnd);
    if(dp == 0)
    {
        return 0;
    }

    return CopyData(dp, chan, dataImportRel) ? &dataImportRel[chan][0] : 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CopyData(DataSettings *ds, Channel chan, uint8 datatImportRel[2][FPGA_MAX_POINTS])
{
    if((chan == A && ds->enableCh0 == 0) || (chan == B && ds->enableCh1 == 0))
    {
        return false;
    }
    uint8* pointer = (chan == A) ? (&datatImportRel[0][0]) : (&datatImportRel[1][0]);

    uint8* address = ((uint8*)ds + sizeof(DataSettings));

    int length = ds->length1channel * (ds->peakDet == PeackDet_Disable ? 1 : 2);

    if(chan == B && ds->enableCh0 == 1)
    {
        address += length;
    }

    memcpy(pointer, address, length);

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8* DS_GetAverageData(Channel chan)
{
    static uint8 data[NumChannels][FPGA_MAX_POINTS];
    
    if (newSumCalculated[chan] == false)
    {
        return &data[chan][0];
    }

    newSumCalculated[chan] = false;

    DataSettings *ds = 0;
    uint8 *d0, *d1;
    DS_GetDataFromEnd(0, &ds, &d0, &d1);

    if (ds == 0)
    {
        return 0;
    }

    int numPoints = ds->length1channel * (ds->peakDet == PeackDet_Disable ? 1 : 2);

    if (sDisplay_ModeAveraging() == Averaging_Around)
    {
        float *floatAveData = (chan == A) ? aveData0 : aveData1;
        
        for (int i = 0; i < numPoints; i++)
        {
            data[chan][i] = (uint8)(floatAveData[i]);
        }
        return &data[chan][0];
    }

    int numAveraging = sDisplay_NumAverage();

    LIMIT_ABOVE(numAveraging, DS_NumElementsWithSameSettings());

    for(int i = 0; i < numPoints; i++)
    {
        data[chan][i] = sum[chan][i] / numAveraging;
    }

    return &data[chan][0];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8* DS_GetLimitation(Channel chan, int direction)
{
    uint8 *retValue = 0;

    if(direction == 0)
    {
        retValue = &(limitDown[chan][0]);
    }
    else if(direction == 1)
    {
        retValue = &(limitUp[chan][0]);
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int DS_NumberAvailableEntries(void)
{
    if(firstElem == 0)
    {
        return 0;
    }
    return SIZE_POOL / SizeElem(lastElem);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VerifyAddress(uint8 *dst, uint size)
{
    if(dst < beginPool || dst > endPool || (dst + size) < beginPool || (dst + size) > endPool)
    {
        LOG_ERROR("!!!������ ������ � ������");
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define COPY_AND_INCREASE(address, data, length)    \
    memcpy(address, data, length);                  \
    address += length;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PushData(DataSettings *dp, uint8 *data0, uint8 *data1)
{
    int required = SizeElem(dp);
    while(MemoryFree() < required)
    {
        RemoveFirstElement();
    }

    uint8* addrRecord = 0;
    if(firstElem == 0)
    {
        firstElem = (DataSettings*)beginPool;
        addrRecord = beginPool;
        dp->addrPrev = 0;
        dp->addrNext = 0;
    }
    else
    {
        addrRecord = (uint8*)lastElem + SizeElem(lastElem);
        if(addrRecord + SizeElem(dp) > endPool)
        {
            addrRecord = beginPool;
        }
        dp->addrPrev = lastElem;
        lastElem->addrNext = addrRecord;
        dp->addrNext = 0;
    }

    lastElem = (DataSettings*)addrRecord;

    COPY_AND_INCREASE(addrRecord, dp, sizeof(DataSettings));

    uint length = dp->length1channel;

    if(dp->enableCh0 == 1)
    {
        COPY_AND_INCREASE(addrRecord, data0, length);
        if (dp->peakDet != PeackDet_Disable)
        {
            COPY_AND_INCREASE(addrRecord, data0 + 512, length);
        }
    }
    if(dp->enableCh1 == 1)
    {
        COPY_AND_INCREASE(addrRecord, data1, length);
        if (dp->peakDet != PeackDet_Disable)
        {
            COPY_AND_INCREASE(addrRecord, data1 + 512, length);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryFree(void)
{
    if (firstElem == 0)
    {
        return SIZE_POOL;
    }
    else if (firstElem == lastElem)
    {
        return (endPool - (uint8*)firstElem - (int)SizeElem(firstElem));
    }
    else if (firstElem < lastElem)
    {
        if ((uint8*)firstElem == beginPool)
        {
            return (endPool - (uint8*)lastElem - SizeElem(lastElem));
        }
        else
        {
            return (uint8*)firstElem - beginPool;
        }
    }
    else if (lastElem < firstElem)
    {
        return (uint8*)firstElem - (uint8*)lastElem - SizeElem(lastElem);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int SizeElem(DataSettings *dp)
{
    int retValue = sizeof(DataSettings);
    if(dp->enableCh0 == 1)
    {
        retValue += dp->length1channel;
        if (dp->peakDet != PeackDet_Disable)
        {
            retValue += dp->length1channel;
        }
    }
    if(dp->enableCh1 == 1)
    {
        retValue += dp->length1channel;
        if (dp->peakDet != PeackDet_Disable)
        {
            retValue += dp->length1channel;
        }
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void RemoveFirstElement(void)
{
    firstElem = NextElem(firstElem);
    firstElem->addrPrev = 0;
    allData--;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings* NextElem(DataSettings *elem)
{
    return (DataSettings*)elem->addrNext;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings* FromEnd(int indexFromEnd)
{
    if(firstElem == 0)
    {
        return 0;
    }
    int index = indexFromEnd;
    DataSettings *retValue = lastElem;
    while(index != 0 && ((retValue = retValue->addrPrev) != 0))
    {
        index--;
    }
    if(index != 0)
    {
        LOG_ERROR("������������ ������ %d, ����� ������ %d", indexFromEnd, DS_AllDatas());
        return 0;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SettingsIsIdentical(int elemFromEnd0, int elemFromEnd1)
{
    DataSettings* dp0 = FromEnd(elemFromEnd0);
    DataSettings* dp1 = FromEnd(elemFromEnd1);
    return SettingsIsEquals(dp0, dp1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool SettingsIsEquals(DataSettings *dp0, DataSettings *dp1)
{
    bool retValue = (dp0->enableCh0  == dp1->enableCh0) &&
        (dp0->enableCh1     == dp1->enableCh1) &&
        (dp0->inverseCh0    == dp1->inverseCh0) &&
        (dp0->inverseCh1    == dp1->inverseCh1) &&
        (dp0->range[0]      == dp1->range[0]) &&
        (dp0->range[1]      == dp1->range[1]) &&
        (dp0->rShiftCh0     == dp1->rShiftCh0) &&
        (dp0->rShiftCh1     == dp1->rShiftCh1) &&
        (dp0->tBase         == dp1->tBase) &&
        (dp0->tShift        == dp1->tShift) &&
        (dp0->modeCouple0   == dp1->modeCouple0) &&
        (dp0->modeCouple1   == dp1->modeCouple1) &&
        (dp0->trigLevCh0    == dp1->trigLevCh0) &&
        (dp0->trigLevCh1    == dp1->trigLevCh1) &&
        (dp0->multiplier0   == dp1->multiplier0) &&
        (dp0->multiplier1   == dp1->multiplier1) &&
        (dp0->peakDet       == dp1->peakDet); 
    return retValue;
}
