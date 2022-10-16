// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"


namespace ReaderFPGA
{
    // „итает точки в обычном режиме
    void ReadData();

    // „итает точки поточечного режима
    void ReadPointsP2P();

    // „итать массив точек от first до last
    void ReadPoints(const Chan &ch, uint8 *first, const uint8 *last);

    extern Mutex mutex_read;                    // ћьютекс на чтение данных
};
