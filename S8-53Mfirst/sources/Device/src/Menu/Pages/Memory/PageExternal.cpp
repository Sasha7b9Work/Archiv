// 2021/05/12 14:11:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(mcName, PageMemory::PageExternal::self,
    "��� �����", "File name"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "Sets the mode name when saving files to an external drive:\n"
    "\"By mask\" - files are named automatically advance to the entered mask(seq.Menu),\n"
    "\"Manual\" - each time the file name must be specified manually"
    ,
    "�� �����", "Mask",
    "�������", "Manually",
    FILE_NAMING_MODE, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcModeSave, PageMemory::PageExternal::self,
    "��������� ���", "Save as"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� "
    "BMP\n ���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � "
    "���������� TXT"
    ,
    "If you select \"Image\", the signal will be stored in the current directory in graphic file with the extension "
    "BMP\nIf you select \"Text\", the signal will be stored in the current directory as a text file with the extension "
    "TXT"
    ,
    "�����������", "Image",
    "�����", "Text",
    MODE_SAVE_SIGNAL, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcModeBtnMemory, PageMemory::PageExternal::self,
    "��� �� ������", "Mode btn MEMORY",
    "",
    "",
    "����", "Menu",
    "����������", "Save",
    MODE_BUTTON_MEMORY, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(mcAutoConnectFlash, PageMemory::PageExternal::self,
    "���������.", "AutoConnect",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    "If \"Enable\", when you connect an external drive is automatically transferred to the page MEMORY - Ext.Storage",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FLASH_AUTO_CONNECT, nullptr, nullptr, nullptr
)


DEF_PAGE_6(pageExternal, PageMemory::self, NamePage::MemoryExt,
    "����� ��", "EXT STORAGE",
    "������ � ������� ������������ �����������.",
    "Work with external storage device.",
    *PageMemory::PageFileManager::self,
    mcName,
    *PageMemory::PageSetMask::self,
    mcModeSave,
    mcModeBtnMemory,
    mcAutoConnectFlash,
    nullptr, nullptr, nullptr, nullptr
);


const Page *PageMemory::PageExternal::self = &pageExternal;
