// 2021/03/17 14:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"


// Поиск сигнала
namespace AutoFinderFPGA
{
    void Find();

    bool FindWave(const Chan &ch);

    Range::E AccurateFindRange(const Chan &ch);

    TBase::E FindTBase(const Chan &ch);

    TBase::E AccurateFindTBase(const Chan &ch);

    extern bool auto_find_in_progress;
};
