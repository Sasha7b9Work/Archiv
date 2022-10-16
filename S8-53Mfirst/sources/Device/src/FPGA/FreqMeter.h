// 2021/08/10 09:52:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Hardware/HAL/HAL_.h"
#include "FPGA/TypesFPGA.h"


namespace FreqMeter
{
    void Init();

    void Update(uint16 flag);

    // Получить значение частоты.
    float GetFrequency();

    float CalculateFrequencyFromCounterFrequency();

    float CalculateFrequencyFromCounterPeriod();
}
