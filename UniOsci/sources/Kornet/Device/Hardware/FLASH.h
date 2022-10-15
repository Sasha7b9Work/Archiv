#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FlashMemory
{
public:
    void SaveSettings();
    void LoadSettings();
private:
    /// Возвращает адрес первого свободного байта в секторе настроек
    uint FirstFreeAddressForSettings();
    /// \brief Возвращает адрес сохранённых настроек или 0, если настройки не сохранялись. fromEnd указывает, какие настройки от конца
    /// нужно загружать - 0 - последние, 1 - предпоследние и так далее
    uint AddressSavedSettings(int fromEnd);
    /// Возвращает адрес первого свободного байта для записи
    uint AddressFirstEmptyByte();

    uint GetSector(uint address);

    void EraseSector(uint address);

    void WriteBytes(uint address, uint8 *data, int size);

    void ReadBytes(uint address, void *data, uint size);
};


extern FlashMemory flash;

