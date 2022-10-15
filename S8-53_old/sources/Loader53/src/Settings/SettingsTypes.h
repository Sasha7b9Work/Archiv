#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDisplay
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    ColorScheme_WhiteLetters,       // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
    ColorScheme_BlackLetters        // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
} ColorScheme;

// ���������� ����� ��� ������� �����������.
typedef enum
{
    DisplaySmooth_Disable,
    DisplaySmooth_2points = 1,
    DisplaySmooth_3points = 2,
    DisplaySmooth_4points = 3,
    DisplaySmooth_5points = 4,
    DisplaySmooth_6points = 5,
    DisplaySmooth_7points = 6,
    DisplaySmooth_8points = 7,
    DisplaySmooth_9points = 8,
    DisplaySmooth_10points = 9
} DisplaySmoothing;

// ����� ����������� ����������� �������� ��� ����������� ������� ������.
typedef enum
{
    Missed_Hide,    // �� �������� �� �����
    Missed_Show,    // �������� �� �����
    Missed_Average  // ���������� � �������� �� �����
} MissedSignals;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ChannelX
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    ChanA,
    ChanB,
    ChanA_B,
    ChanMath
} Channel;

#define NumChannels 2

// ����� ������ �� �����
typedef enum
{
    ModeCouple_DC,      // �������� ����
    ModeCouple_AC,      // �������� ����
    ModeCouple_GND      // ���� �������
} ModeCouple;

// ��������
typedef enum
{
    Multiplier_1,
    Multiplier_10
} Multiplier;

// ������� �� ����������
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

// ����� �������
typedef enum
{
    StartMode_Auto,     // ��������������
    StartMode_Wait,     // ������ 
    StartMode_Single    // �����������
} StartMode;

// �������� �������������
typedef enum
{
    TrigSource_Channel0,    // ����� 1
    TrigSource_Channel1,    // ����� 2
    TrigSource_Ext          // �������
} TrigSource;

// ��� ������������
typedef enum
{
    TrigPolarity_Front,     // �� ������
    TrigPolarity_Back       // �� �����
} TrigPolarity;

// ���� �������������
typedef enum
{
    TrigInput_Full,         // ������ ������
    TrigInput_AC,           // ����������
    TrigInput_LPF,          // ���
    TrigInput_HPF           // ���
} TrigInput;

typedef enum
{
    TrigModeFind_Hand,          // ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������
    TrigModeFind_Auto           // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
} TrigModeFind;

// ����� ����������� ������� ������ �����
typedef enum
{
    ModeLongPRessTrig_Level0,
    ModeLongPressTrig_Auto
} ModeLongPressTrig;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������� ��/���
typedef enum
{
    FunctionTime_Time,
    FunctionTime_ShiftInMemory
} FunctionTime;

// ������� �� �������
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

// ��������� ����� ������������� �� �������
typedef enum
{
    TPos_Left,      // �������� � ������ ����
    TPos_Center,    // �������� � ������
    TPos_Right      // �������� � ������� ����
} TPos;

// ��� ������� ��� ������ �������������.
typedef enum
{
    SampleType_Real,   // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������
    SampleType_Equal   // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������
} SampleType;

typedef enum
{
    PeakDet_Disabled,
    PeakDet_Enabled,
    PeakDet_Average
} PeakDetMode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cursors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� �������� ���������
typedef enum
{
    CursCntrl_1,        // ������
    CursCntrl_2,        // ������
    CursCntrl_1_2,      // ������
    CursCntrl_Disable   // �������
} CursCntrl;

// ������������ ����������� ��������
typedef enum
{
    CursMovement_Points,    // �� ������
    CursMovement_Percents   // �� ���������
} CursMovement;

// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
typedef enum
{
    CursActive_U,
    CursActive_T,
    CursActive_None
} CursActive;

// ����� �������� ��������
typedef enum
{
    CursLookMode_None,          // ������� �� ������
    CursLookMode_Voltage,       // ������� ������ �� ����������� �������������
    CursLookMode_Time,          // ������� ������ �� �������� �������������
    CursLookMode_Both           // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
} CursLookMode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����� ����� �������, � ������� ��� ������.
typedef enum
{
    FNP_281,
    FNP_512,
    FNP_1024
} FPGA_NUM_POINTS;

// ����� ������.
typedef enum
{
    ModeWork_Dir,        // �������� �����.
    ModeWork_RAM,        // � ���� ������ ����� ����������� ��������� ���������� ���������
    ModeWork_ROM         // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������
} ModeWork;

// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������
typedef enum
{
    ModeShowIntMem_Direct,  // ���������� ������ ��������� �������.
    ModeShowIntMem_Saved,   // ���������� ���������� ������
    ModeShowIntMem_Both     // ���������� ������ ��������� ������� � ���������� ������
} ModeShowIntMem;

// ��� ������ ��� ������� ������ ������
typedef enum
{
    ModeBtnMemory_Menu,     // ����� ������������ ��������������� �������� ����
    ModeBtnMemory_Save      // ���������� ����������� ������ �� ������
} ModeBtnMemory;

// ����� ������������ ������
typedef enum
{
    FileNamingMode_Mask,        // ��������� �� �����
    FileNamingMode_Manually     // ��������� �������
} FileNamingMode;

// ��� ��������� ������ �� ������
typedef enum
{
    ModeSaveSignal_BMP,
    ModeSaveSignal_TXT
} ModeSaveSignal;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Measures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ������� �� ������� ��� ������ ���������.
typedef enum
{
    ModeViewSignals_AsIs,       // ���������� ������� ��� ����
    ModeViewSignals_Compress    // ������� ����� � ���������
} ModeViewSignals;

typedef enum
{
    MN_1,                       // 1 ��������� ����� �����
    MN_2,                       // 2 ��������� ����� �����
    MN_1_5,                     // 1 ������ � 5 �����������
    MN_2_5,                     // 2 ������ �� 5 ���������
    MN_3_5,                     // 3 ������ �� 5 ���������
    MN_6_1,                     // 6 ����� �� 1 ���������
    MN_6_2                      // 6 ����� �� 2 ���������
} MeasuresNumber;

// ����, �� ������� ��������� ��������
typedef enum
{
    MeasuresField_Screen,       // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    MeasuresField_AllMemory,    // ��������� ����� ������������� �� ����� �������.
    MeasuresField_Hand          // ��������� ����� ������������� �� ����, ����������� �������������.
} MeasuresField;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Math
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    Function_Sum,
    Function_Mul
} Function;

typedef enum
{
    ScaleMath_Channel0,     // ������� ������ �� ������ 1
    ScaleMath_Channel1,     // ������� ������ �� ������ 2
    ScaleMath_Hand          // ������� ������� �������.
} ScaleMath;

typedef enum
{
    ScaleFFT_Log,           // ��� �������� �������� ��������������� ����� ������ �������
    ScaleFFT_Linear         // ��� �������� �������� �������� ����� ������ �������
} ScaleFFT;

typedef enum
{
    SourceFFT_Channel0,
    SourceFFT_Channel1,
    SourceFFT_Both
} SourceFFT;

typedef enum
{
    WindowFFT_Rectangle,
    WindowFFT_Hamming,
    WindowFFT_Blackman,
    WindowFFT_Hann
} WindowFFT;

typedef enum
{
    FFTmaxDB_40,
    FFTmaxDB_60,
    FFTmaxDB_80
} FFTmaxDB;

typedef enum
{
    FuncModeDraw_Disable,
    FuncModeDraw_Separate,
    FuncModeDraw_Together
} FuncModeDraw;

typedef enum
{
    ModeRegSet_Range,
    ModeRegSet_RShift
} ModeRegSet;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Service
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ���� ����
typedef enum
{
    Russian,    // �������
    English     // ����������
} Language;

typedef enum
{
    Calibrator_Freq,
    Calibrator_DC,
    Calibrator_GND
} CalibratorMode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ��� ������������ ��� �������.
// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
typedef enum
{
    BalanceADC_Disable,     // ������������ ���������.
    BalanceADC_Settings,    // ������������ �������� ������������, ������� �������� �������������.
    BalanceADC_Hand         // ������������ �������� ������������, �������� �������.
} BalanceADCtype;

// ��� �������� ���
typedef enum
{
    StretchADC_Disable,
    StretchADC_Settings,
    StretchADC_Hand
} StretchADCtype;

// ��� ��������������� ��������
typedef enum
{
    RShiftADC_Disable,
    RShiftADC_Settings,
    RShiftADC_Hand
} RShiftADCtype;
