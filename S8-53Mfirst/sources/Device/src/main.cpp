// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "common/Hardware/Sound_.h"
#include "Settings/Settings.h"


int main()
{
    Device::ResetAllSettings();

    Device::Init();

    Sound::WarnBeepGood();

    while(1)
    {
        Device::Update();

        Settings::Save();

        setNRST.Save();
    }
}
