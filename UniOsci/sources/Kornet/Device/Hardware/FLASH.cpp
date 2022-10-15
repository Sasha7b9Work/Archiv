#include <stm32f4xx_hal.h>
#include "FLASH.h"
#include "Hardware.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include <limits.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FlashMemory flash;

/// ѕервый сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
/// ¬торой сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 1.
#define ADDR_SECTOR_SETTINGS_2  ((uint)0x080E0000)

#define SIZE_SECTOR_128         (128 * 1024)

#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


#define READ_DOUBLEWORD(address)    (*((uint *)address))
#define READ_BYTE(address)          (*((uint8 *)address))


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FlashMemory::SaveSettings()
{
    /*
        јлгоритм нахождени€ первого свободного слова
        1. текущий адрес = (uint*)ADDR_SECTOR_SETTINGS_1.
        2. —мотрим, что записано по текущему адресу
        2. ≈сли там 0xffffffff, то это первое свободное слово - выходим.
        3. ѕрибавл€ем к текущему адресу значение *((uint*)ADDR_SECTOR_SETTINGS_1).
        4. ѕереходим к 2
    */
    /*
        1. «аписываем в Settings.size значение sizeof(Settings)
        2. Ќаходим адрес первого свободного байта (алгоритм выше)
        3. Ќаходим размер_свободной_пам€ти = ADDR_SECTOR_SETTINGS_1 + (128 * 1024) - адрес_первого_свободного_слова
        4. ≈сли размер_свободной_пам€ти > размер_структуры_настроек, то сохран€ем по найденному адресу структуру Settings и выходим.
        5. »наче стираем сектор настроек и записываем в его начало структуру Settings
    */

    /// \todo ќдного сектора дл€ хранени€ настроек недостаточно, потому что после стирани€ может отключитьс€ питание и тогда сотрутс€ все настройки

    set.size = sizeof(set);

    uint address = FirstFreeAddressForSettings();

    uint freeMemory = ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128 - address;

    if (freeMemory <= sizeof(Settings))
    {
        EraseSector(ADDR_SECTOR_SETTINGS_1);
        address = ADDR_SECTOR_SETTINGS_1;
    }

    set.size = sizeof(set);
    WriteBytes(address, (uint8 *)&set, sizeof(set));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FlashMemory::LoadSettings()
{
    uint address = AddressSavedSettings(0);

    if (address && READ_DOUBLEWORD(address) == sizeof(set))
    {
        ReadBytes(address, &set, READ_DOUBLEWORD(address));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FlashMemory::FirstFreeAddressForSettings()
{
    uint address = ADDR_SECTOR_SETTINGS_1;

    do
    {
        uint value = READ_DOUBLEWORD(address);

        if (value == UINT_MAX)              // Ёто условие означает, что по этому адресу ещЄ ничего не записывалось, иначе здесь был бы записан
        {                                   // размер структуры (Settings), чьим первым байтом €вл€лось бы это слово
            return address;
        }

        address += value;                   // ѕереходим на первый свободный байт за структурой

    } while(address < (ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128));
    
    
    return UINT_MAX;        // ¬ообще-то до этой точки дойти никак не может. ≈сли мы оказались здесь, произошла ошибка
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FlashMemory::AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR_SETTINGS_1;

    while (READ_DOUBLEWORD(address) != UINT_MAX)
    {
        addrPrev = address;
        address += READ_DOUBLEWORD(address);
    }

    return addrPrev;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FlashMemory::AddressFirstEmptyByte()
{
    uint address = AddressSavedSettings(0);
    return address + READ_DOUBLEWORD(address);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint FlashMemory::GetSector(uint address)
{
    if (address == ADDR_SECTOR_SETTINGS_1)
    {
        return FLASH_SECTOR_10;
    }
    else if (address == ADDR_SECTOR_SETTINGS_2)
    {
        return FLASH_SECTOR_11;
    }



    return FLASH_SECTOR_0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FlashMemory::EraseSector(uint address)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        GetSector(address),
        1,
        FLASH_VOLTAGE_RANGE_3
    };

    uint error = 0;

    if (HAL_FLASHEx_Erase(&isFLASH, &error) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_FLASH_Lock();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FlashMemory::WriteBytes(uint address, uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();
    
    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        ++address;
    }

    HAL_FLASH_Lock();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FlashMemory::ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = (uint8 *)data;

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}
