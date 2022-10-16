// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"


struct DataSettings
{
    DataSettings() : is_clean(1) { enabled[0] = enabled[1] = false; } //-V730

    bool               enabled[Chan::Count];    // ������� �� ����� 1
    bool               enabled_peak_det : 2;    // ������� �� ������� ��������
    uint               r_shift_a        : 10;   // �������� �� ����������
    uint               r_shift_b        : 10;   
    uint               trig_lev_a       : 10;   // ������� �������������
    uint               trig_lev_b       : 10;   
    uint               couple_a         : 2;    // ����� ������ �� �����
    uint               couple_b         : 2;    
    uint               tbase            : 5;    // ������� �� �������
    Divider::E         multiplier_a     : 1;    
    Divider::E         multiplier_b     : 1;    
    EnumPointsFPGA::E  enum_points      : 4;    // enum ����� � ������
    uint               is_clean         : 1;    // ������������� � 1 �������� ��������, ��� ������ ��� (��������� ������
                                                // ��� �������)
    PackedTime         time;
    int                tshift;                  // �������� �� �������
    Range::E           range[2];                // ������� �� ���������� ����� �������.
    uint               num_in_rom;


    void Fill();

    TBase::E GetTBase() const
    {
        return (TBase::E)tbase;
    }

    // ���������� ���������� ����, ���������� ����� �������
    int BytesInChannel() const
    {
        int result = PointsInChannel();

        if (enabled_peak_det)
        {
            result *= 2;
        }

        return result;
    }

    // ���������� ���������� ����, ���������� ������ �������� (� ������ �� ���������)
    int BytesInData() const;

    // ������� ����� � ����� ������
    int PointsInChannel() const
    {
        return EnumPointsFPGA::ToPoints((EnumPointsFPGA::E)enum_points);
    }
};
