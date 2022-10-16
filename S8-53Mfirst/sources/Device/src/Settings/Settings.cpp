// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "common/Utils/Math_.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Panel/Panel.h"
#include <cstring>


static const Settings defaultSettings =
{
    0,                              // size
    0,                              // crc32
    0,                              // number
    // Display
    {
        5,                          // timeShowLevels
        512 - 140,                  // shiftInMemory
        5,                          // timeMessages
        100,                        // brightness
        20,                         // brightnessGrid
        {
            Color::Make(0x00, 0x00, 0x00),   // Color::BLACK = 0x00,
            Color::Make(0xFF, 0xFF, 0xFF),   // Color::WHITE = 0x01,
            Color::Make(0x80, 0x80, 0x80),   // Color::GRID = 0x02,
            Color::Make(0x01, 0xCA, 0xFF),   // Color::DATA_A = 0x03,
            Color::Make(0x00, 0xFF, 0x00),   // COLOR_DATA_B = 0x04,
            Color::Make(0xD5, 0xDA, 0xD5),   // Color::MENU_FIELD = 0x05,
            Color::Make(0xFF, 0xB2, 0x00),   // COLOR_MENU_TITLE = 0x06,
            Color::Make(0x7B, 0x59, 0x00),   // Color::MENU_TITLE_DARK = 0x07,
            Color::Make(0xFF, 0xFF, 0x00),   // Color::MENU_TITLE_BRIGHT = 0x08,
            Color::Make(0x69, 0x89, 0x00),   // COLOR_MENU_ITEM = 0x09,
            Color::Make(0x6B, 0x45, 0x00),   // Color::MENU_ITEM_DARK = 0x0a,
            Color::Make(0xFF, 0xCE, 0x00),   // Color::MENU_ITEM_BRIGHT = 0x0b,
            Color::Make(0x00, 0x00, 0x00),   // Color::MENU_SHADOW = 0x0c,
            Color::Make(0x00, 0x00, 0x00),   // COLOR_EMPTY = 0x0d,
            Color::Make(0x08, 0xA2, 0xCE),   // COLOR_EMPTY_A = 0x0e,
            Color::Make(0x00, 0xCE, 0x00)    // COLOR_EMPTY_B = 0x0f,
        },
        ModeDrawSignal::Lines,      // modeDrawSignal
        TypeGrid::_1,               // typeGrid
        ENumAccumulation::_1,       // numAccumulation
        ENumAveraging::_1,          // numAveraging
        ModeAveraging::Accurately,  // modeAveraging
        ENumMinMax::_1,             // enumMinMax
        Smoothing::Disable,         // smoothing
        ENumSignalsInSec::_25,      // num signals in one second
        Chan::A,                 // lastAffectedChannel
        ModeAccumulation::NoReset,  // modeAccumulation
        AltMarkers::Show,           // altMarkers
        MenuAutoHide::None,         // menuAutoHide
        true,                       // showFullMemoryWindow
        false,                      // showStringNavigation
        LinkingRShift::Voltage      // linkingRShift
    },
    // channels
    {
        {
            RShift::ZERO,
            ModeCouple::AC,         // ModCouple
            Divider::_1,            // Divider
            Range::_500mV,          // range
            true,                   // enable
            false,                  // inverse
            false                   // filtr
        },
        {
            RShift::ZERO,
            ModeCouple::AC,         // ModCouple
            Divider::_1,            // Divider
            Range::_500mV,          // range
            true,                   // enable
            false,                  // inverse
            false                   // filtr
        }
        
    },
    // trig
    {
        StartMode::Auto,
        TrigSource::A,
        TrigPolarity::Front,
        TrigInput::Full,
        {TrigLev::ZERO, TrigLev::ZERO, TrigLev::ZERO},
        ModeLongPressTrig::Zero,
        TrigModeFind::Hand
    },
    // time
    {
        TBase::_1ms,
        FunctionTime::Time,
        TPos::Center,
        SampleType::Equal,
        PeakDetMode::Disable,
        EnumPointsFPGA::_1k,
        0,                          // set.time.tShiftRel
        false
    },
    // cursors
    {
        { CursCntrl::Disabled, CursCntrl::Disabled },   // CursCntrl U
        { CursCntrl::Disabled, CursCntrl::Disabled },   // CursCntrl T
        Chan::A,                                     // source
        { 60.0F,  140.0F, 60.0F, 140.0F },              // posCur U
        { 80.0F,  200.0F, 80.0F, 200.0F },              // posCur T
        { 80.0F,  80.0F },                              // ���������� ����� ��������� ���������� ��� 100%
        { 120.0F, 120.0F },                             // ���������� ����� ��������� ������� ��� 100%
        CursMovement::Points,                           // CursMovement
        CursActive::None,                               // CursActive
        { CursLookMode::None, CursLookMode::None },     // ����� �������� ��������.
        false,                                          // showFreq
        false                                           // showCursors
    },
    // memory
    {
        EnumPointsFPGA::_1k,
        ModeWork::Direct,
        FileNamingMode::Mask,
        "Signal_\x07\x03",   // \x07 - ��������, ��� ����� ������ ���� ��������� �����, \x03 - ������� �� ��� ����������
        "Signal01",
        0,
        {
            false
        },
        ModeShowIntMem::Both,       // modeShowIntMem
        false,                      // flashAutoConnect
        ModeBtnMemory::Menu,
        ModeSaveSignal::BMP
    },
    // measures
    {
        MeasuresNumber::_1,         // measures number
        Chan::A_B,               // source
        ModeViewSignals::AsIs,      // mode view signal
        {
            TypeMeasure::VoltageMax,     TypeMeasure::VoltageMin,  TypeMeasure::VoltagePic,      TypeMeasure::VoltageMaxSteady,  TypeMeasure::VoltageMinSteady,
            TypeMeasure::VoltageAverage, TypeMeasure::VoltageAmpl, TypeMeasure::VoltageRMS,      TypeMeasure::VoltageVybrosPlus, TypeMeasure::VoltageVybrosMinus,
            TypeMeasure::Period,         TypeMeasure::Freq,        TypeMeasure::TimeNarastaniya, TypeMeasure::TimeSpada,         TypeMeasure::DurationPlus
        },
        false,                      // show
        MeasuresField::Screen,      // ����� ��� ��������� ��������, ������� �� ������
        //{0, 200},                 // ��������� �������� �������� ���������� ��� ���� ���������
        //{372, 652},               // ��������� �������� �������� ������� ��� ���� ���������
        {50, 150},                  // ��������� �������� �������� ���������� ��� ���� ���������
        {422, 602},                 // ��������� �������� �������� ������� ��� ���� ���������
        CursCntrl::_1,              // ��������� ������ �������� ����������
        CursCntrl::_1,              // ��������� ������ �������� �������
        CursActive::T,              // ������� ������� �������.
        TypeMeasure::None           // marked TableAutoMeasures
    },
    // math
    {
        {
            ScaleFFT::Log,
            SourceFFT::A_,
            WindowFFT::Rectangle,
            MaxDbFFT::_60,
            0,
            {100, 256 - 100},
            false
        },
        Function::Sum,
        1,
        1,
        1,
        1,
        ModeDrawMath::Disable,       // modeDrawMath
        ModeRegSet::Range,           // modeRegSet
        Range::_50mV,
        Divider::_1,
        RShift::ZERO                  // rShift
    },
    // service
    {
        CalibratorMode::Freq,       // calibratorEnable
        ColorScheme::WhiteLetters,  // colorScheme
        true,                       // soundEnable
        0                           // IP-����� (��������)  WARN
    },
    {
        0x8b, 0x2e, 0xaf, 0x8f, 0x13, 0x00, // mac
        192, 168, 1, 200,                   // ip
        7,
        255, 255, 255, 0,                   // netmask
        192, 168, 1, 1                      // gateway
    },
    // common
    {
        0
    },
    // menu
    {
        false,                  // isShown
        NamePage::MainPage,     // currentPage
        0                       // currentSubPage
    }
};

Settings set;


void Settings::Load()
{
    Settings *saved = ROM::Settings::Load< ::Settings>();

    if (!saved)
    {
        *this = defaultSettings;
    }
    else if (saved->size != sizeof(*this))
    {
        *this = defaultSettings;
        std::memcpy(this, saved, saved->size);
        size = sizeof(*this);
    }
    else
    {
        *this = *saved;
    }
}


void Settings::Save()
{
    static uint timeLastSaved = 0;

    Settings *saved = ROM::Settings::Load<Settings>();

    if ((!saved || *saved != set) && (TIMER_MS - timeLastSaved > 5000))
    {
        ROM::Settings::Save<Settings>(&set);

        timeLastSaved = TIMER_MS;
    }
}


void Settings::Reset()
{
    *this = defaultSettings;
}


Range::E& operator++(Range::E &range)
{
    range = (Range::E)((int)range + 1);
    return range;

}


Range::E& operator--(Range::E &range)
{
    range = (Range::E)((int)range - 1);
    return range;
}


bool SettingsMath::IsMathEnabled()
{
    return !MODE_DRAW_MATH_IS_DISABLED || FFT_IS_ENABLED;
}


void Chan::Enable() const
{
    ENABLED(value) = true;
}


void Chan::Disable() const
{
    ENABLED(value) = false;
}


Chan Chan::Opposed() const
{
    if (value == A)
    {
        return Chan(B);
    }
    else if (value == B)
    {
        return Chan(A);
    }

    return Chan(Count);
}


uint Settings::CalcWriteCRC32()
{
    uint8 *buffer = reinterpret_cast<uint8 *>(this);

    return HAL_CRC::Calculate8bit(buffer + 2 * sizeof(uint), sizeof(*this) - 2 * sizeof(uint));
}


bool Settings::operator!=(const Settings &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0;
}
