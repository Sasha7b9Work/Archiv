// 2021/03/30 13:42:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tables.h"


struct Chan;


namespace Warnings
{
    void Draw();

    void ClearFromWarnings();

    void ShowWarningBad(pchar textRu, pchar textEn);

    void ShowWarningGood(pchar textRu, pchar textEn);

    int BottomY();

    void ShowWarningBadLimitRange(const Chan &ch);
    void ShowWarningBadLimitRShift(const Chan &ch);
};
