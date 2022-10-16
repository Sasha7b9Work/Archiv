// 2021/06/01 10:09:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "Display/Warnings.h"
#include "Hardware/FDrive/FDrive.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"




static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    FDrive::OpenNewFileForWrite("S8-53M.bin");

    uint8 *address = (uint8 *)0x08020000; //-V566
    uint8 *endAddress = address + 128 * 1024 * 3;

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock);
        address += sizeBlock;
    }

    FDrive::CloseFile();

    Warnings::ShowWarningGood("�������� ���������", "FIRMWARE SAVED");
}

DEF_BUTTON(bSaveFirmware, PageDebug::PageData::self,
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    IsActive_SaveFirmware, OnPress_SaveFirmware
)



static void OnPress_EraseData()
{
    ROM::Data::EraseAll();
}

DEF_BUTTON(bEraseData, PageDebug::PageData::self,
    "������� ������", "Erase data",
    "������� ������� � �������",
    "Erases data sectors",
    nullptr, OnPress_EraseData
)



static void OnPress_EraseSettings()
{
    Sector::Get(Sector::_10_SETTINGS_1).Erase();
    Sector::Get(Sector::_11_SETTINGS_2).Erase();

    set.Load();
}

DEF_BUTTON(bEraseSettings, PageDebug::PageData::self,
    "������� ���������", "Erase settings",
    "������� ���������",
    "Erase settings",
    nullptr, OnPress_EraseSettings
)



static void OnPress_EraseNRST()
{
    Sector::Get(Sector::_12_NRST_1).Erase();
    Sector::Get(Sector::_13_NRST_2).Erase();

    setNRST.Load();
}

DEF_BUTTON(bEraseNRST, PageDebug::PageData::self,
    "������� NRST", "Erase NRST",
    "������� �������������� ���������",
    "Erase non-resettable settings",
    nullptr, OnPress_EraseNRST
)



DEF_PAGE_4(pageData, PageDebug::self, NamePage::DebugInfo,
    "������", "DATA",
    "",
    "",
    bSaveFirmware,
    bEraseData,
    bEraseSettings,
    bEraseNRST,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageDebug::PageData::self = &pageData;
