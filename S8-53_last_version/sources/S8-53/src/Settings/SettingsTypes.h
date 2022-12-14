#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDisplay
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ????? ??????????? ??????????? ???????? ??? ??????????? ??????? ??????.
typedef enum
{
    Missed_Hide,    // ?? ???????? ?? ?????
    Missed_Show,    // ???????? ?? ?????
    Missed_Average  // ?????????? ? ???????? ?? ?????
} MissedSignals;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChannelX
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    A,
    B,
    A_B,
    Math
} Channel;

/// ????? ?????? ?? ?????.
typedef enum
{
    ModeCouple_DC,      ///< ???????? ????.
    ModeCouple_AC,      ///< ???????? ????.
    ModeCouple_GND      ///< ???? ????????.
} ModeCouple;

/// ????????.
typedef enum
{
    Divider_1,
    Divider_10
} Divider;

/// ??????? ?? ??????????.
typedef enum
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
} Range;

Range& operator++(Range &range);
Range& operator--(Range &range);

#define NumChannels 2

const char *RangeName(Range range);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ??????? ?? ???????
typedef enum
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
    TBase_5s,
    TBase_10s,
    TBaseSize
} TBase;

const char *TBaseName(TBase tBase);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Measures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ??????? ?? ??????? ??? ?????? ?????????.
typedef enum
{
    ModeViewSignals_AsIs,       // ?????????? ??????? ??? ????
    ModeViewSignals_Compress    // ??????? ????? ? ?????????
} ModeViewSignals;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    ScaleMath_Channel0,     // ??????? ??????? ?? ?????? 1
    ScaleMath_Channel1,     // ??????? ??????? ?? ?????? 2
    ScaleMath_Hand          // ??????? ???????? ???????.
} ScaleMath;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ???????
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ????? ???????? ?????????.
typedef enum
{
    CursCntrl_1,            ///< ??????.
    CursCntrl_2,            ///< ??????.
    CursCntrl_1_2,          ///< ??????.
    CursCntrl_Disable       ///< ???????.
} CursCntrl;


/// ???????????? ??????????? ????????.
typedef enum
{
    CursMovement_Points,    ///< ?? ??????.
    CursMovement_Percents   ///< ?? ?????????.
} CursMovement;

/// ????? ??????? ?????? ???????. ????? ???????, ?? ? ????? ???????????? ?? ???????? ????? ?????????.
typedef enum
{
    CursActive_U,
    CursActive_T,
    CursActive_None
} CursActive;

/// ????? ???????? ????????.
typedef enum
{
    CursLookMode_None,      ///< ??????? ?? ??????.
    CursLookMode_Voltage,   ///< ??????? ?????? ?? ??????????? ?????????????.
    CursLookMode_Time,      ///< ??????? ?????? ?? ???????? ?????????????.
    CursLookMode_Both       ///< ??????? ?????? ?? ???????? ? ???????????, ? ??????????? ?? ????, ????? ??????? ??????? ?????????.
} CursLookMode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Service
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ???? ????
typedef enum
{
    Russian,    // ???????
    English     // ??????????
} Language;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ??? ???????????? ??? ???????.
// ???? ? ???, ??? ?????? ??? ?? ????????? ??-?? ??????? ????????????? ( ? ), ??????? ?? ?????? ?????????????? ???????? ??? ?????? ?? ??? ??????.
typedef enum
{
    BalanceADC_Disable,     // ???????????? ?????????.
    BalanceADC_Settings,    // ???????????? ???????? ????????????, ??????? ???????? ?????????????.
    BalanceADC_Hand         // ???????????? ???????? ????????????, ???????? ???????.
} BalanceADCtype;

// ??? ???????? ???
typedef enum
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
} StretchADCtype;

// ??? ??????????????? ????????
typedef enum
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
} RShiftADCtype;
