// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "common/Settings/SettingsTypes_.h"


struct DataSettings
{
    DataSettings() : is_clean(1) { enabled[0] = enabled[1] = false; } //-V730

    bool               enabled[Chan::Count];    // Включен ли канал 1
    bool               enabled_peak_det : 2;    // Включен ли пиковый детектор
    uint               r_shift_a        : 10;   // Смещение по напряжению
    uint               r_shift_b        : 10;   
    uint               trig_lev_a       : 10;   // Уровень синхронизации
    uint               trig_lev_b       : 10;   
    uint               couple_a         : 2;    // Режим канала по входу
    uint               couple_b         : 2;    
    uint               tbase            : 5;    // Масштаб по времени
    Divider::E         multiplier_a     : 1;    
    Divider::E         multiplier_b     : 1;    
    EnumPointsFPGA::E  enum_points      : 4;    // enum точек в канале
    uint               is_clean         : 1;    // установленное в 1 значение означает, что данных нет (структура только
                                                // что создана)
    PackedTime         time;
    int                tshift;                  // Смещение по времени
    Range::E           range[2];                // Масштаб по напряжению обоих каналов.
    uint               num_in_rom;


    void Fill();

    TBase::E GetTBase() const
    {
        return (TBase::E)tbase;
    }

    // Возвращает количество байт, занимаемых одним каналом
    int BytesInChannel() const
    {
        int result = PointsInChannel();

        if (enabled_peak_det)
        {
            result *= 2;
        }

        return result;
    }

    // Возвращает количество байт, занимаемых обоими каналами (с учётом их включения)
    int BytesInData() const;

    // Сколько точек в одном канале
    int PointsInChannel() const
    {
        return EnumPointsFPGA::ToPoints((EnumPointsFPGA::E)enum_points);
    }
};
