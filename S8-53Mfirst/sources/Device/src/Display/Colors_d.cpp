// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "Settings/Settings.h"


Color Color::Trig()
{
    if (TRIG_SOURCE_IS_A || TRIG_SOURCE_IS_B)
    {
        return Chan((Chan::E)TRIG_SOURCE).GetColor();
    }

    return Color::FILL;
}
