#include "defines.h"
#include "Hardware/CPU.h"
#include "Log.h"
#include "Display/Display.h"
#include "Menu/Menu.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VALUE_FOR_RTC 0x644


#define RTC_CLOCK_SOURCE_LSE
// #define RTC_CLOCK_SOURCE_LSI


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x0130
#endif


#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x00ff
#endif


static const RTC_HandleTypeDef rtcHandle =
{
    RTC,
    {
        RTC_HOURFORMAT_24,          // HourFormat
        RTC_ASYNCH_PREDIV,          // AsynchPrediv
        RTC_SYNCH_PREDIV,           // SynchPrediv
        RTC_OUTPUT_DISABLE,         // OutPut
        RTC_OUTPUT_POLARITY_HIGH,   // OutPutPolarity
        RTC_OUTPUT_TYPE_OPENDRAIN   // OutPutType
    },
    HAL_UNLOCKED, HAL_RTC_STATE_RESET
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::RTC_::Init()
{
    if (HAL_RTC_Init((RTC_HandleTypeDef*)(&rtcHandle)) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_RTCEx_BKUPRead((RTC_HandleTypeDef*)&rtcHandle, RTC_BKP_DR0) != VALUE_FOR_RTC)
    {
        if(SetTimeAndData(11, 11, 11, 11, 11, 11))
        {
            HAL_RTCEx_BKUPWrite((RTC_HandleTypeDef*)&rtcHandle, RTC_BKP_DR0, VALUE_FOR_RTC);
        }
        gBF.tuneTime = 1;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PackedTime CPU::RTC_::GetPackedTime()
{
    PackedTime time;

    RTC_TimeTypeDef isTime;
    HAL_RTC_GetTime((RTC_HandleTypeDef*)&rtcHandle, &isTime, FORMAT_BIN);

    time.hours = isTime.Hours;
    time.minutes = isTime.Minutes;
    time.seconds = isTime.Seconds;

    RTC_DateTypeDef isDate;
    HAL_RTC_GetDate((RTC_HandleTypeDef*)&rtcHandle, &isDate, FORMAT_BIN);

    time.year = isDate.Year;
    time.month = isDate.Month;
    time.day = isDate.Date;

    return time;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::RTC_::SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds)
{
    RTC_DateTypeDef dateStruct =
    {
        RTC_WEEKDAY_MONDAY,
        (uint8)month,
        (uint8)day,
        (uint8)year
    };

    if(HAL_RTC_SetDate((RTC_HandleTypeDef*)&rtcHandle, &dateStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };

    RTC_TimeTypeDef timeStruct =
    {
        (uint8)hours,
        (uint8)minutes,
        (uint8)seconds,
        RTC_HOURFORMAT_24,
        RTC_DAYLIGHTSAVING_NONE,
        RTC_STOREOPERATION_SET
    };

    if(HAL_RTC_SetTime((RTC_HandleTypeDef*)&rtcHandle, &timeStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };
    
    return true;
}
