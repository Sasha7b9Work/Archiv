// 2021/05/12 10:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "common/Hardware/Memory/Sector_.h"
#include "Display/Display.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


static const SettingsNRST defaultNRST =
{
    0,                              // size
    0,                              // crc32
    0,                              // number
    false,                          // show_stats
    // SettingsRandomizer
    {
        6,                          // num_ave
        1,                          // num_smooth
        1000,                       // num_measures_for_gates
        0,                          // shift_T0
        false,                      // show_info
        0,                          // gate_min
        0                           // game_max
    },
    // SettingsADC
    {
        true,                       // type_stretch
        0,                          // first_byte
        RShiftADCtype::Enabled      // type_rshift
    },
    // SettingsChannel
    {
        {
            0,                                                          // balance_ADC
            { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
              {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},  // rshift
            1.0f                                                        // stretch_auto
        },
        {
            0,                                                          // balance_ADC
            { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
              {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},  // rshift
            1.0f                                                        // stretch_auto
        }
    },
    {
        10,                         // num_srings
        0,                          // size_font
        0                           // in_pause
    },
    {
        false,                  // ���������� ����/����� ������
        false, false, false, false, false, false, false, false, false, false, false
    },
    {
        CalibrationMode::x1, CalibrationMode::x1
    },
    0
};


SettingsNRST setNRST = defaultNRST;


int SettingsNRST::SettingsConsole::GetSizeFontForConsole()
{
    return setNRST.console.size_font ? 8 : 5;
}


void SettingsNRST::Load()
{
    SettingsNRST *saved = ROM::Settings::Load<SettingsNRST>();

    if (!saved ||                   // ���� ��� ���������� ��������
        saved->size == (uint)(-1))  // ��� ��������� ��� ������ �� ����
    {
        *this = defaultNRST;
    }
    else if (saved->size != sizeof(*this))
    {
        *this = defaultNRST;
        std::memcpy(this, saved, saved->size);
        size = sizeof(*this);
    }
    else
    {
        *this = *saved;
    }
}


void SettingsNRST::Save()
{
    SettingsNRST *saved = ROM::Settings::Load<SettingsNRST>();

    size = sizeof(*this);

    if (!saved ||               // ���� ��� ���������� ��������
        saved->size != size ||  // ��� ������ ��������� �� ���������
        *saved != *this)        // ��� ���������� � ������� ��������� �� ���������
    {
        ROM::Settings::Save<SettingsNRST>(this);
    }
}


void SettingsNRST::ExecuteOnChanged()
{
    setNRST.Save();
}


void SettingsNRST::ExecuteOnChanged(bool)
{
    ExecuteOnChanged();
}


bool SettingsNRST::operator!=(const SettingsNRST &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0;
}


void SettingsNRST::Test()
{
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();

    for (uint i = 0; i < 0xffffffff; i++)
    {
        ROM::Settings::Save<SettingsNRST>(this);

        SettingsNRST *saved = ROM::Settings::Load<SettingsNRST>();

        LOG_WRITE("��������� �� ������ %X %d", saved, i);

        Display::Update();
    }
}


int16 SettingsNRST::SettingsChannel::AddRShift(Range::E range, int mode)
{
    if (setNRST.adc.type_rshift == RShiftADCtype::Disabled)
    {
        return 0;
    }

    if (mode > 1)
    {
        return 0;
    }

    return rshift[range][mode];
}
