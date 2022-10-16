// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


#define SOUND_ENABLED       (set.service.soundEnabled)          // SettingsService.soundEnabled


// ��������� �����������
struct SettingsDisplay
{
    int16               timeShowLevels;             // �����, � ������� �������� ����� ���������� ������ ��������
    int16               shiftInMemory;              // ���������� �������� ������ ���� ����� ������������ �������� ����� ������. ����� ��� ����������� ����������� ������� � ����.
    int16               timeMessages;               // ����� � ��������, �� ������� ��������� ������� �� ������
    int16               brightness;                 // ������� ������� (������ ��� ��������)
    int16               brightnessGrid;             // ������� ����� �� 0 �� 100
    uint                colors[16];
    ModeDrawSignal::E   modeDrawSignal;             // ����� ��������� �������
    TypeGrid::E         typeGrid;                   // ��� �����
    ENumAccumulation::E enumAccumulation;           // ����� ���������� ������� �� ������
    ENumAveraging::E    enumAve;                    // ����� ���������� �������
    ModeAveraging::E    modeAve;                    // ��� ���������� �� ����������
    ENumMinMax::E       enumMinMax;                 // ����� ��������� ��� ����������� ��������� � ����������
    Smoothing::E        smoothing;                  // ����� ����� ��� ����������� �������
    ENumSignalsInSec::E enumSignalsInSec;           // ����� ���������� ������� � �������
    Chan::E             lastAffectedChannel;        // ����� �������� ����� ���������� ������, ������� ��������� �������. ����� ��� ����, ����� �����, ����� ������ �������� �������.
    ModeAccumulation::E modeAccumulation;           // ����� ����� ���������� ��������
    AltMarkers::E       altMarkers;                 // ����� ����������� �������������� ������� �������� ��������
    MenuAutoHide::E     menuAutoHide;               // ����� ������� ������� ����� ���������� ������� ������� ������� ���� 
    bool                showFullMemoryWindow;       // ���������� �� ���� ������ ������ ������.
    bool                showStringNavigation;       // ���������� �� ������ �������� ��������� ����.
};


// ��������� �������������
struct SettingsTrig
{
    StartMode::E         startMode;          // ����� �������.
    TrigSource::E        source;             // ��������.
    TrigPolarity::E      polarity;           // ��� �������������.
    TrigInput::E         input;              // ���� �������������.
    ModeLongPressTrig::E modeLongPressTrig;  // ����� ������ ����������� ������� ������ �����.
    TrigModeFind::E      modeFind;           // ����� ������������� - ������� ��� �������������.
    int16                levelRel[3];        // ������� ������������� ��� ��� ����������.
};


// ��������� ���������
struct SettingsTime
{
    bool                selfRecorder;   // ������� �� ����� ���������.
    int16               tShiftRel;      // �������� �� �������
    TBase::E            tBase;          // ������� �� �������.
    FunctionTime::E     timeDivXPos;
    TPos::E             tPos;
    SampleType::E       sampleType;
    PeakDetMode::E     peakDet;
    EnumPointsFPGA::E oldNumPoints;     // ����� ������������� � ����� �������� ���������, ������������� ����������
                                        // ����� � 1024, � ���� ���������� ��, ��� ����, ����� ����� ������������
};


// ��������� ��������
struct SettingsCursors
{
    CursCntrl::E        cntrlU[Chan::Count];     // �������� ������� ����������.
    CursCntrl::E        cntrlT[Chan::Count];     // �������� ������� ����������.
    Chan::E          source;                     // �������� - � ������ ������ ��������� �������.
    float               posCurU[Chan::Count][2]; // ������� ������� �������� ���������� ����� �������.
    float               posCurT[Chan::Count][2]; // ������� ������� �������� ������� ����� �������.
    float               deltaU100percents[2];       // ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float               deltaT100percents[2];       // ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement::E     movement;                   // ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive::E       active;                     // ����� ������� ������ �������.
    CursLookMode::E     lookMode[2];                // ������ �������� �� ��������� ��� ���� ��� ��������.
    bool                showFreq;                   // ������������� � true ��������, ��� ����� ���������� �� ������
                                                    // �������� 1/dT ����� ���������.
    bool                showCursors;                // ���������� �� �������
};

struct StructMemoryLast
{
    bool isActiveModeSelect;
};

// ��������� ����->������
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    EnumPointsFPGA::E fpgaNumPoints;          // ����� �����.
    ModeWork::E         modeWork;               // ����� ������.
    FileNamingMode::E   fileNamingMode;         // ����� ���������� ������.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // ����� ����� ��� ��������������� ���������� ������\n
         // ������� ����������.\n
         // %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������\n
         // %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007\n
         // �������\n
         // name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32\n
         // ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� ����� ������ ����� 0001, �.�. ��� ����������� ������ ��������������� ������ ������� �� ����.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     // ��� ����� ��� ������ ������� �������
    int8                indexCurSymbolNameMask; // ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    StructMemoryLast    strMemoryLast;
    ModeShowIntMem::E   modeShowIntMem;         // ����� ������� ���������� � ������ ����������� ��
    bool                flashAutoConnect;       // ���� true, ��� ���������� ���� ������������� ��������� NC (������ ���������)
    ModeBtnMemory::E    modeBtnMemory;     
    ModeSaveSignal::E   modeSaveSignal;         // � ����� ���� ��������� ������.
};
                                    
struct SettingsMath
{
    ScaleFFT::E     scaleFFT;
    SourceFFT::E    sourceFFT;
    WindowFFT::E    windowFFT;
    MaxDbFFT::E     fftMaxDB;
    Function::E     func;
    uint8           currentCursor;          // ����������, ����� �������� ������� ��������� ����� ���������
    uint8           posCur[2];              // ������� ������� �������. ���������� 0...256.
    int8            koeff1add;              // ����������� ��� ������ ��������� ��� ��������.
    int8            koeff2add;
    int8            koeff1mul;
    int8            koeff2mul;
    bool            enableFFT;
    ModeDrawMath::E modeDraw;               // ���������� ��� ����� ������� � ������ �������������� �������
    ModeRegSet::E   modeRegSet;             // ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������
    Range::E        range;
    Divider::E      multiplier;
    int16           rShift;
};

// ��� ��������� �������� ����� ���� -> ������
struct SettingsService
{
    bool              screenWelcomeEnable;    // ����� �� ������������ ����� ����������� ��� ��������� �������.
    bool              sound_enabled;           // �������� �� �����.
    int8              IPaddress;              // IP-����� (��������)
    CalibratorMode::E calibrator;             // ����� ������ �����������.
    ColorScheme::E    colorScheme;            //
};

struct SettingsEthernet
{
    uint8 mac0;
    uint8 mac1;
    uint8 mac2;
    uint8 mac3;
    uint8 mac4;
    uint8 mac5;

    uint8 ip0;
    uint8 ip1;
    uint8 ip2;
    uint8 ip3;

    uint16 port;

    uint8 mask0;
    uint8 mask1;
    uint8 mask2;
    uint8 mask3;

    uint8 gw0;
    uint8 gw1;
    uint8 gw2;
    uint8 gw3;

    bool enable;
};
                                    
struct SettingsCommon
{
    int         countEnables;               // ���������� ���������. ������������� ��� ������ ���������
    int         countErasedFlashData;       // ������� ��� �������� ������ ������ � ���������
    int         countErasedFlashSettings;   // ������� ��� �������� ������ � ����������
    int         workingTimeInSecs;          // ����� ������ � ��������
    Language::E lang;                       // ���� ����
};

struct OutputRegisters
{
    bool    all;                                // ���������� �������� ���� ���������.
    bool    flag;                               // �������� �� ���� ����������.
    bool    rShift0;
    bool    rShift1;
    bool    trigLev;
    bool    range[2];
    bool    chanParam[2];
    bool    trigParam;
    bool    tShift;
    bool    tBase;
};


// �������� ������ ��� ��������� �������.
struct Settings
{
    SettingsDisplay     display;                    // ��������� �����������          (���� �������)
    SettingsTime        time;                       // ��������� ���������            (���� ���¨����)
    SettingsCursors     cursors;                    // ��������� ��������� ���������  (���� �������)
    SettingsMemory      memory;                     // ��������� ������� ������       (���� ������)
    SettingsMath        math;                       // ��������� ������ �������������� ���������
    SettingsService     service;                    // �������������� ���������       (���� ������)
    SettingsEthernet    eth;
    SettingsCommon      common;                     // ��������� ���������
    int temp;

    static void Load();
};

extern Settings set;
