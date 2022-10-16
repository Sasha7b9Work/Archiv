// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    void Init();
    void Update();
    void ResetAllSettings();
    extern bool needResetSettings;  // Если 1, то необходим сброс настроек
};
