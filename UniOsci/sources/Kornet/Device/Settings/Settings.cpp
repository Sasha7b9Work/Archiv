#include "Settings.h"
#include "FPGA/FPGA.h"
#include "Hardware/FLASH.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const Settings defaultSettings =
{
    0,                              // size
    0,                              // crc32

    Mode_Lines,                     // disp_modeDrawSignal
    Thickness_1,                    // disp_thickness

    {RShiftZero, RShiftZero},       // chan_shift
    {Range_500mV, Range_500mV},     // chan_range
    {Couple_DC, Couple_DC},         // chan_couple
    {true, true},                   // chan_enable

    A,                              // trig_source
    TrigInput_Full,                 // trig_input
    Polarity_Rising,                // trig_polarity
    TrigLevZero,                    // trig_lev
    ModeTrig_Auto,                  // trig_mode

    Russian,                        // serv_lang

    0,                              // time_shift
    TBase_500us,                    // time_base

    Control_Voltage,                // test_control
    Polarity_Positive,              // test_polarity
    StepU_100mV,                    // test_stepU
    StepI_4mA,                      // test_stepI
    0,                              // test_smooth

    EnumPoints_512,                 // mem_enumPoints
        
    0,                              // menu_currentPage
    false                           // menu_show
};

Settings set;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load()
{
    Reset();
    flash.LoadSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    flash.SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Reset()
{
    set = defaultSettings;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *NameRange(Range range)
{
    static const char *names[RangeSize][NumLanguages] =
    {
        {"2мВ",     "2mV"},
        {"5мВ",     "5mV"},
        {"10мВ",    "10mV"},
        {"20мВ",    "20mV"},
        {"50мВ",    "50mV"},
        {"0.1В",    "0.1V"},
        {"0.2В",    "0.2V"},
        {"0.5В",    "0.5V"},
        {"1В",      "1V"},
        {"2В",      "2V"},
        {"5В",      "5V"},
        {"10В",     "10V"},
        {"20В",     "20V"}
    };

    return names[range][LANG];
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *NameTBase(TBase tBase)
{
    static const char *names[TBaseSize][NumLanguages] =
    {
        {"2нс",     "2ns"},
        {"5нс",     "5ns"},
        {"10нс",    "10ns"},
        {"20нс",    "20ns"},
        {"50нс",    "50ns"},
        {"0.1мкс",  "0.1us"},
        {"0.2мкс",  "0.2us"},
        {"0.5мкс",  "0.5us"},
        {"1мкс",    "1us"},
        {"2мкс",    "2us"},
        {"5мкс",    "5us"},
        {"10мкс",   "10us"},
        {"20мкс",   "20us"},
        {"50мкс",   "50us"},
        {"0.1мс",   "0.1ms"},
        {"0.2мс",   "0.2ms"},
        {"0.5мс",   "0.5ms"},
        {"1мс",     "1ms"},
        {"2мс",     "2ms"},
        {"5мс",     "5ms"},
        {"10мс",    "10ms"},
        {"20мс",    "20ms"},
        {"50мс",    "50ms"},
        {"0.1с",    "0.1s"},
        {"0.2с",    "0.2s"},
        {"0.5с",    "0.5s"},
        {"1с",      "1s"},
        {"2с",      "2s"},
        {"5с",      "5s"},
        {"10с",     "10s"}
    };

    return names[tBase][LANG];
}
