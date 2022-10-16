// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/Screen/Grid.h"
#include "Hardware/FDrive/FDrive.h"
#include "Menu/FileManager.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


int16       PageMemory::PageLatest::currentSignal = 0;
int8        PageMemory::PageInternal::currentSignal = 0;
bool        PageMemory::PageInternal::showAlways = false;
ReturnTo::E ReturnTo::fromSetName = ReturnTo::MainMenu;
bool        PageMemory::needForSaveToFlashDrive = false;

bool PageMemory::exitFromIntToLast = false;


void PageMemory::OnChanged_NumPoints(bool)
{
    int points_in_channel = EnumPointsFPGA::PointsInChannel();

    int16 &shift = SHIFT_IN_MEMORY_IN_POINTS;

    if(points_in_channel == 281)
    {
        shift = 0;
    }
    else
    {
        if(TPOS_IS_LEFT)
        {
            shift = 0;
        }
        else if(TPOS_IS_CENTER)
        {
            shift = (int16)(points_in_channel / 2 - Grid::Width() / 2);
        }
        else if(TPOS_IS_RIGHT)
        {
            shift = (int16)(points_in_channel - Grid::Width() - 2);
        }
    }

    TShift::Set(TSHIFT);
}


//static void DrawSB_MemLastSelect(int x, int y)
//{
//    Char(set_.memory.str_memory_last.isActiveModeSelect ? '\x2a' : '\x28').Draw4SymbolsInRect(x + 3, y + 2, TypeFont::UGO2);
//}


//static void PressSB_MemLastSelect()
//{
//    set_.memory.str_memory_last.isActiveModeSelect = !set_.memory.str_memory_last.isActiveModeSelect;
//}


//static void DrawSB_MemExtNewFolder(int x, int y)
//{
//    Char('\x46').Draw4SymbolsInRect(x + 1, y, TypeFont::UGO2);
//}


static bool FuncActiveMemoryNumPoinst()
{
    return !PEAK_DET_IS_ENABLED;
}


DEF_CHOICE_3(mcNumPoints, PageMemory::self,
    "Точки", "Points"
    ,
    "Выбор количества отсчётов для сохраняемых сигналов. "
    "При увеличении количества отсчётов уменьшается количество сохранённых в памяти сигналов."
    ,
    "Choice of number of counting for the saved signals. "
    "At increase in number of counting the quantity of the signals kept in memory decreases."
    ,
    "281", "281",
    "512", "512",
    "1k",  "1k",
    ENUM_POINTS_FPGA, FuncActiveMemoryNumPoinst, PageMemory::OnChanged_NumPoints, nullptr
)


void PageMemory::PageExternal::SaveSignalToFlashDrive()
{
    if (FDrive::IsConnected())
    {
        if (FILE_NAMING_MODE_IS_HAND)
        {
            PageMemory::PageSetName::self->OpenAndSetItCurrent();
        }
        else
        {
            PageMemory::needForSaveToFlashDrive = true;
        }
    }
    else
    {
        ReturnTo::fromSetName = ReturnTo::MainMenu;
    }
}


void PageMemory::PageExternal::OnPress_FileManager()
{
    if(FDrive::IsConnected())
    {
        PageMemory::PageFileManager::self->OpenAndSetItCurrent();
        Display::SetDrawMode(Display::DrawMode::Timer, FM::Draw);
        FM::needRedrawFileManager = 1;
    }
}


DEF_PAGE_4(pageMemory, PageMain::self, NamePage::Memory,
    "ПАМЯТЬ", "MEMORY",
    "Работа с внешней и внутренней памятью.",
    "Working with external and internal memory.",
    mcNumPoints,
    *PageMemory::PageLatest::self,
    *PageMemory::PageInternal::self,
    *PageMemory::PageExternal::self ,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageMemory::self = &pageMemory;
