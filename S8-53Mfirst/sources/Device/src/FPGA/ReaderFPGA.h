// 2021/03/17 8:40:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Utils/Mutex_.h"


namespace ReaderFPGA
{
    // ������ ����� � ������� ������
    void ReadData();

    // ������ ����� ����������� ������
    void ReadPointsP2P();

    // ������ ������ ����� �� first �� last
    void ReadPoints(const Chan &ch, uint8 *first, const uint8 *last);

    extern Mutex mutex_read;                    // ������� �� ������ ������
};
