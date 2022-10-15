#include <stm32f4xx_hal.h>
#include "FLASH.h"
#include "Hardware.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include <limits.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FlashMemory flash;

/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
/// ������ ������ ��� ���������� ��������. ��� ��� ���������� �������� �������������� ������ 1.
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
        �������� ���������� ������� ���������� �����
        1. ������� ����� = (uint*)ADDR_SECTOR_SETTINGS_1.
        2. �������, ��� �������� �� �������� ������
        2. ���� ��� 0xffffffff, �� ��� ������ ��������� ����� - �������.
        3. ���������� � �������� ������ �������� *((uint*)ADDR_SECTOR_SETTINGS_1).
        4. ��������� � 2
    */
    /*
        1. ���������� � Settings.size �������� sizeof(Settings)
        2. ������� ����� ������� ���������� ����� (�������� ����)
        3. ������� ������_���������_������ = ADDR_SECTOR_SETTINGS_1 + (128 * 1024) - �����_�������_����������_�����
        4. ���� ������_���������_������ > ������_���������_��������, �� ��������� �� ���������� ������ ��������� Settings � �������.
        5. ����� ������� ������ �������� � ���������� � ��� ������ ��������� Settings
    */

    /// \todo ������ ������� ��� �������� �������� ������������, ������ ��� ����� �������� ����� ����������� ������� � ����� �������� ��� ���������

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

        if (value == UINT_MAX)              // ��� ������� ��������, ��� �� ����� ������ ��� ������ �� ������������, ����� ����� ��� �� �������
        {                                   // ������ ��������� (Settings), ���� ������ ������ �������� �� ��� �����
            return address;
        }

        address += value;                   // ��������� �� ������ ��������� ���� �� ����������

    } while(address < (ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128));
    
    
    return UINT_MAX;        // ������-�� �� ���� ����� ����� ����� �� �����. ���� �� ��������� �����, ��������� ������
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
