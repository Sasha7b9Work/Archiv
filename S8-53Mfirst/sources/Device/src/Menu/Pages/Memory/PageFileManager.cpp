// 2021/05/12 15:07:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "Hardware/FDrive/FDrive.h"
#include "Menu/FileManager.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


static void PressSB_Exit()
{
    Display::SetDrawMode(Display::DrawMode::Default);
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageFileManager::self,
    EXIT_RU, EXIT_EN,
    EXIT_ILLUSTRATION_RU,
    EXIT_ILLUSTRATION_EN,
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void DrawSB_Tab(int x, int y)
{
    Char('\x6e').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbTab, PageMemory::PageFileManager::self,
    "Tab", "Tab",
    "������� ����� ���������� � �������",
    "The transition between the directories and files",
    nullptr, FM::PressTab, DrawSB_Tab, nullptr
)


static void DrawSB_LevelUp(int x, int y)
{
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbLevelUp, PageMemory::PageFileManager::self,
    "�����", "Leave",
    "������� � ������������ �������",
    "Transition to the parental catalog",
    nullptr, FM::PressLevelUp, DrawSB_LevelUp, nullptr
)


static void DrawSB_LevelDown(int x, int y)
{
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbLevelDown, PageMemory::PageFileManager::self,
    "�����", "Enter",
    "������� � ��������� �������",
    "Transition to the chosen catalog",
    nullptr, FM::PressLevelDown, DrawSB_LevelDown, nullptr
)

static bool FuncActive()
{
    return FDrive::IsConnected();
}


DEF_PAGE_SB(pageFileManager, PageMemory::PageExternal::self, NamePage::MemoryExtFileManager_SB,
    "�������", "DIRECTORY",
    "��������� ������ � �������� ������� ������������� ����������",
    "Provides access to the file system of the connected drive",
    sbExit,
    sbTab,
    Item::empty,
    Item::empty,
    sbLevelUp,
    sbLevelDown,
    FuncActive, PageMemory::PageExternal::OnPress_FileManager, EmptyFuncVV, FM::RotateRegSet
);

const Page *PageMemory::PageFileManager::self = &pageFileManager;
