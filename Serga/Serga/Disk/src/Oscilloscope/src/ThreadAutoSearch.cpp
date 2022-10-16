#include "ThreadAutoSearch.h"
#include "../../common/TypesAltera.h"
#include "../../common/Timer.h"
#include "../../common/BinaryMathematics.h"
#include "../../Device129Multi/src/AlteraOscillograph.h"
#include "../../Device129Multi/src/Device129Multi.h"
#include "WindowOsci.h"

using namespace NS_TypesAltera;
using namespace NS_BinaryMathematics;
using namespace NS_AlteraAddressSpace;;

extern int numDevice;

ThreadAutoSearch::ThreadAutoSearch(WindowOsci *_w):QThread(), find(false), w(*_w) 
{
}

void ThreadAutoSearch::run() 
{
    while(!find) 
    {
    }
    FindSignalInternal();
    find = false;
}

void ThreadAutoSearch::FindSignal() 
{
    find = true;
}

void ThreadAutoSearch::FindSignalInternal() 
{
    //bool autoCalibrate = D129_GetAutoCalibrate(numDevice);
    //D129_SetAutoCalibrate(numDevice, false);

    w.pbStart->setChecked(false);
    w.cbStartMode->setCurrentIndex(1);
    w.cbTPos->setCurrentIndex(0);
    w.rbAC_Channel0->click();
    w.rbAC_Channel1->click();
    w.sbRShift0->setValue(RShiftZero);
    w.sbRShift1->setValue(RShiftZero);
    w.sbTShift->setValue(TShiftZero);
    w.pbSinchro->setChecked(false);

    int tBase = TBase_5ms;
    D129_SetTBase(numDevice, tBase);

    int mainChannel = D129_GetSinchroChannel(numDevice);
    int range0 = FindRange(0);
    D129_SetRange(numDevice, 0, range0);
    int range1 = FindRange(1);
    D129_SetRange(numDevice, 1, range1);

    D129_SetSinchroChannel(numDevice, mainChannel);
    D129_SetTrigLevRel(numDevice, 0, TrigLevZero);
    D129_SetTrigLevRel(numDevice, 1, TrigLevZero);

    // Теперь находим масштаб по времени
    for(; tBase >= 0; tBase--)
    {
        if(!VerifyTwoSinchro(mainChannel, tBase, timeWaiting))   // Проверяем, есть ли две синхронизации
            break;                              // Если нет - найдено
    }
    tBase++;

    // Корректируем масштаб по напряжению для главного канала
    int range = (mainChannel == 0) ? range0 : range1;
    if(tBase >= TBase_1us) 
    {
        for(--range; range >= 0; range--) 
            if(OutFromScreen(mainChannel, range, timeWaiting)) 
            {
                ++range;
                break;
            }

        if(range < 0) 
            range = 0;
    }

    if(mainChannel == 0) 
        range0 = range;
    else 
        range1 = range;

    //D129_SetAutoCalibrate(numDevice, autoCalibrate);
    w.cbRangeChannel0->setCurrentIndex(range0);
    D129_SetRange(numDevice, 0, range0);
    w.cbRangeChannel1->setCurrentIndex(range1);
    D129_SetRange(numDevice, 1, range1);
    w.cbTBase->setCurrentIndex(tBase);
    D129_SetTBase(numDevice, tBase);
    w.cbStartMode->setCurrentIndex(0);
    D129_SetStartMode(numDevice, StartMode_Auto);
    w.sbTrigLev->setValue(TrigLevZero);
    w.pbStart->setChecked(true);
   
    emit AutoSearchExit(!VerySmallSignal(mainChannel, timeWaiting));
}

bool ThreadAutoSearch::VerifySinchro(int _channel, int _trigLev, double _timeUs) 
{
    const BYTE bitSinchronisation = 3;
    Timer timer;
    D129_SetTrigLevRel(numDevice, _channel, _trigLev);
    BYTE flag = 0;
    D129_Start(numDevice, true);
    timer.Start(_timeUs);
    do 
    {
        D129_DataReadyForRead(numDevice, flag);
    } while(timer.IsRun() && (!GetBit(flag, bitSinchronisation)));
    return GetBit(flag, bitSinchronisation);;
}

bool ThreadAutoSearch::VerifyTwoSinchro(int /*_channel*/, int _tBase, double _timeUs) 
{
    static int tablePostK[6] = {5, 10, 25, 50, 100, 250};
    const BYTE bitTwoSinchronisation = 5;
    Timer timer;
    D129_SetTBase(numDevice, _tBase);
    if(_tBase < TBase_1us) 
    {
        int post = 16384 - tablePostK[_tBase];
        D129_Write(numDevice, WRITE_POST_RG6, (BYTE)post);
        D129_Write(numDevice, WRITE_POST_RG7, (BYTE)(post>>8));
    }
    D129_Start(numDevice, true);
    timer.Start(_timeUs);
    BYTE flag = 0;
    bool dataReady = false;
    do 
    {
        dataReady = D129_DataReadyForRead(numDevice, flag);
    } while(timer.IsRun() && (!dataReady));
    bool twoSinchro = GetBit(flag, bitTwoSinchronisation);
    return (twoSinchro && dataReady);
}

bool ThreadAutoSearch::OutFromScreen(int _channel, int range, double _timeUs) 
{
    Timer timer;
    D129_SetRange(numDevice, _channel, range);
    D129_Start(numDevice, true);
    timer.Start(_timeUs);
    BYTE flag = 0;
    do 
    {
    } while(timer.IsRun() && (!D129_DataReadyForRead(numDevice, flag)));
    if(D129_DataReadyForRead(numDevice, flag)) 
    {
        D129_ReadData(numDevice);
        double* data = D129_GetPointerToDataDraw(numDevice, _channel);
        for(int i = 0; i < DB_SIGNED_LENGTH; i++) 
            if((data[i] < MIN_VALUE) || (data[i] > MAX_VALUE))
                return true;
    }
    return false;
}

bool ThreadAutoSearch::VerySmallSignal(int _channel, double _timeUs) 
{
    Timer timer;
    D129_Start(numDevice, true);
    timer.Start(_timeUs);
    BYTE flag = 0;
    do 
    {
    } while(timer.IsRun() && (!D129_DataReadyForRead(numDevice, flag)));

    if(D129_DataReadyForRead(numDevice, flag)) 
    {
        D129_ReadData(numDevice);
        int min = AVE_VALUE - (MAX_VALUE - MIN_VALUE) / 6;
        int max = AVE_VALUE + (MAX_VALUE - MIN_VALUE) / 6;
        double* data = D129_GetPointerToDataDraw(numDevice, _channel);
        for(int i = 0; i < DB_SIGNED_LENGTH; i++)
            if((data[i] < min) || (data[i] > max))
                return false;
    }
    return true;
}

int ThreadAutoSearch::FindRange(int _channel) 
{
    int range = Range_5v;
    D129_SetSinchroChannel(numDevice, _channel);
    for(; range >= 0; range--) 
    {
        D129_SetRange(numDevice, _channel, range);
        if(VerifySinchro(_channel, 3000, timeWaiting))
            break;
    }
    if(range < Range_5v) 
        range++;
   
    return range;
}