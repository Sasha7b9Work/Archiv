// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include <cstring>
#include <stm32f4xx_hal.h>


bool HAL_OTP::SaveSerialNumber(char *serialNumber)
{
    // ������� ������ ������ ������ ������ 16 ���� � ������� OPT, ������� � ������.
    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_BASE); //-V566

    while ((*address) != 0xff &&                // *address != 0xff ��������, ��� ������ � ��� ������ ��� �������������
           address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566
    {
        address += 16;
    }

    if (address < reinterpret_cast<uint8 *>(FLASH_OTP_END) - 16) //-V566
    {
        HAL_ROM::WriteBytes(Math::ToUINT(address), reinterpret_cast<uint8 *>(serialNumber),
            std::strlen(serialNumber) + 1);
        return true;
    }

    return false;
}


int HAL_OTP::GetSerialNumber(char buffer[17])
{
    // \todo �������� - ������ �������������� �������. ������ ������ ������ �������� ������� �����, ������� � ���
    // ��������, � �� 16, ��� ������.

    const int allShotsMAX = 512 / 16;   // ������������ ����� ������� � OPT ��������� ������.

    uint8 *address = reinterpret_cast<uint8 *>(FLASH_OTP_END) - 15; //-V566

    do
    {
        address -= 16;
    } while ((*address == 0xff) && (address > reinterpret_cast<uint8 *>(FLASH_OTP_BASE))); //-V566

    if (*address == 0xff)   // �� ����� ������ � �����������, ����� �� ���������� ������ OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, reinterpret_cast<char *>(address));

    return allShotsMAX - (address - reinterpret_cast<uint8 *>(FLASH_OTP_BASE)) / 16 - 1; //-V566
}
