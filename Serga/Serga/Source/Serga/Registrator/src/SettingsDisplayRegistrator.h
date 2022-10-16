//  [2/19/2010 Sasha]
#pragma once
#include "../../common/AbstractSettingsDisplay.h"
#include <Windows.h>

struct SettingsDisplayRegistrator : public AbstractSettingsDisplay {
    SettingsDisplayRegistrator() : AbstractSettingsDisplay(), enableUCurs(false)
    {
        ZeroMemory(posUCur, 2 * sizeof(int));
    };
    bool enableUCurs;
    int posUCur[2];
};