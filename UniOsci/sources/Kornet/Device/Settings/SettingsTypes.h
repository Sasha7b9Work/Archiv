#pragma once


enum Range
{
    Range_2mV,
    Range_5mV,
    Range_10mV,
    Range_20mV,
    Range_50mV,
    Range_100mV,
    Range_200mV,
    Range_500mV,
    Range_1V,
    Range_2V,
    Range_5V,
    Range_10V,
    Range_20V,
    RangeSize
};

enum TBase
{
    TBase_2ns,
    TBase_5ns,
    TBase_10ns,
    TBase_20ns,
    TBase_50ns,
    TBase_100ns,
    TBase_200ns,
    TBase_500ns,
    TBase_1us,
    TBase_2us,
    TBase_5us,
    TBase_10us,
    TBase_20us,
    TBase_50us,
    TBase_100us,
    TBase_200us,
    TBase_500us,
    TBase_1ms,
    TBase_2ms,
    TBase_5ms,
    TBase_10ms,
    TBase_20ms,
    TBase_50ms,
    TBase_100ms,
    TBase_200ms,
    TBase_500ms,
    TBase_1s,
    TBase_2s,
    TBase_5,
    TBase_10s,
    TBaseSize
};

enum Couple
{
    Couple_AC,
    Couple_DC,
    Couple_GND,
    CoupleSize
};

enum TrigInput
{
    TrigInput_Full,
    TrigInput_HF,
    TrigInput_LF
};

enum Channel
{
    A,
    B,
    NumChannels
};

enum TrigPolarity
{
    Polarity_Rising,
    Polarity_Falling
};

enum Language
{
    Russian,
    English,
    NumLanguages
};

enum ModeDrawSignal
{
    Mode_Lines,
    Mode_Points
};

/// Тольщина рисуемого сигнала
enum ThicknessSignal
{
    Thickness_1,    ///< Сигнал рисуется линией толщиной одна точка
    Thickness_3     ///< Сигнал рисуется линией толщиной три точки
};

enum ModeTrig
{
    ModeTrig_Auto,
    ModeTrig_Wait,
    ModeTrig_Once
};

/// Чем будем управлять в тестер-компоненте - напряжением или током
enum TesterControl
{
    Control_Voltage,
    Control_Current
};


enum TesterPolarity
{
    Polarity_Positive,
    Polarity_Negative
};

enum TesterStepU
{
    StepU_100mV,
    StepU_500mV
};

enum TesterStepI
{
    StepI_4mA,
    StepI_20mA
};

enum EnumPoints
{
    EnumPoints_512,
    EnumPoints_1024,
    EnumPoints_2048,
    EnumPoints_4096,
    EnumPoints_8196,
    EnumPoints_Num
};

const char *NameRange(Range range);
const char *NameTBase(TBase tBase);
