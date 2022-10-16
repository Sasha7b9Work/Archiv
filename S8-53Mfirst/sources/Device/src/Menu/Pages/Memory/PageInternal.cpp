// 2021/05/12 13:51:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/Memory/ROM_.h"
#include "common/Utils/Math_.h"
#include "Hardware/FDrive/FDrive.h"
#include "Menu/Menu.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void PressSB_Exit()
{
    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    if (PageMemory::exitFromIntToLast)
    {
        PageMemory::PageLatest::self->OpenAndSetItCurrent();
        set.memory.mode_work = ModeWork::Latest;
        PageMemory::exitFromIntToLast = false;
        Menu::needClosePageSB = false;
    }
    else
    {
        set.memory.mode_work = ModeWork::Direct;
    }
}


DEF_SMALL_BUTTON(sbExit, PageMemory::PageInternal::self,    // ������ ��� ������ �� ������ ����� ������.
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void PressSB_ShowSignalAlways()
{
    PageMemory::PageInternal::showAlways = !PageMemory::PageInternal::showAlways;
}


static void DrawSB_ShowSignalAllways_No(int x, int y)
{
    Char('\x68').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


static void DrawSB_ShowSignalAllways_Yes(int x, int y)
{
    Char('\x66').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


static void DrawSB_ShowSignalAlways(int x, int y)
{
    if (!PageMemory::PageInternal::showAlways)
    {
        DrawSB_ShowSignalAllways_No(x, y);
    }
    else
    {
        DrawSB_ShowSignalAllways_Yes(x, y);
    }
}


static const arrayHints hintsShowSignalAlways =
{
    {DrawSB_ShowSignalAllways_Yes, "���������� ��������� ������ �� ���������� ������ ������ ��������",
                                   "to show the chosen signal from internal memory over the current"},
    {DrawSB_ShowSignalAllways_No,  "������ �� ���������� ������ ����� ������ � ������ ������ � ���������� ������������ "
                                   "�����������"
                                   ,
                                   "the signal from internal memory is visible only in an operating mode with an "
                                   "internal memory"}
};


DEF_SMALL_BUTTON(sbShowSignalAlways, PageMemory::PageInternal::self,
    "���������� ������", "To show always",
    "��������� ������ ���������� ��������� ���������� ������ ������ ��������",
    "Allows to show always the chosen kept signal over the current",
    nullptr, PressSB_ShowSignalAlways, DrawSB_ShowSignalAlways, &hintsShowSignalAlways
)


static void PressSB_ModeShow()
{
    Math::CircleIncrease<int8>((int8 *)&MODE_SHOW_INT_MEM, 0, 2);
}


static void DrawSB_ModeShow_Direct(int x, int y)
{
    Char(Symbol::UGO2::MEM_INT_SHOW_DIRECT).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


static void DrawSB_ModeShow_Saved(int x, int y)
{
    Char(Symbol::UGO2::MEM_INT_SHOW_SAVED).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


static void DrawSB_ModeShow_Both(int x, int y)
{
    ++y;

    Rectangle(4, 7).Draw(x + 2, y + 5);

    Rectangle(4, 4).Draw(x + 8, y + 8);
    HLine().Draw(x + 8, y + 8, y + 5);
    HLine().Draw(y + 5, x + 8, x + 12);

    HLine().Draw(x + 14, y + 6, y + 12);
    HLine().Draw(x + 17, y + 5, y + 12);
    HLine().Draw(y + 9, x + 14, x + 17);
    HLine().Draw(y + 5, x + 15, x + 17);
}


static void DrawSB_ModeShow(int x, int y)
{
    switch (MODE_SHOW_INT_MEM)
    {
    case ModeShowIntMem::Direct:    DrawSB_ModeShow_Direct(x, y);       break;
    case ModeShowIntMem::Saved:     DrawSB_ModeShow_Saved(x, y);        break;
    case ModeShowIntMem::Both:      DrawSB_ModeShow_Both(x, y);         break;
    }
}


static const arrayHints hintsModeShow =
{
    { DrawSB_ModeShow_Direct, "�� ������� ������� ������",     "on the display current signal" },
    { DrawSB_ModeShow_Saved,  "�� ������� ���������� ������", "on the display the kept signal" },
    { DrawSB_ModeShow_Both,   "�� ������� ��� �������",        "on the display the both signals" }
};


DEF_SMALL_BUTTON(sbModeShow, PageMemory::PageInternal::self,
    "��� �������", "Type of a signal",
    "���������� ���������� ��� ������� ������ � ������ ����� ��",
    "Show recorded or current signal in mode Internal Memory",
    nullptr, PressSB_ModeShow, DrawSB_ModeShow, &hintsModeShow
)


static void PressSB_Delete()
{
    ROM::Data::Erase((uint)PageMemory::PageInternal::currentSignal);
}


static void DrawSB_Delete(int x, int y)
{
    Char(Symbol::UGO2::DEL).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbDelete, PageMemory::PageInternal::self,
    "������� ������", "Delete signal",
    "������� ������",
    "Delete signal",
    nullptr, PressSB_Delete, DrawSB_Delete, nullptr
)


static void SaveSignal()
{
    if (PageMemory::exitFromIntToLast)          // ���� ������� �� ����� �� �� ���������
    {
        //        if  (Storage::dsLast != 0)
        //        {                               // �� ��������� ������ �� ���������
        //            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::dsLast, Storage::dataLastA, Storage::dataLastB);
        //            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
        //            Warnings::ShowWarningGood(Warning::SignalIsSaved);
        //        }
    }
    else                                // ����� ��������� ������� ������
    {
        //        if (Storage::set != 0)
        //        {
        //            EPROM::SaveData(PageMemory::PageInternal::currentSignal, Storage::set, Storage::dataA, Storage::dataB);
        //            EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::set, &Storage::dataIntA, &Storage::dataIntB);
        //            Warnings::ShowWarningGood(Warning::SignalIsSaved);
        //        }
    }
}


void PressSB_Save()
{
    SaveSignal();
}


void DrawSB_Save(int x, int y)
{
    Char(Symbol::UGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


DEF_SMALL_BUTTON(sbSave, PageMemory::PageInternal::self,
    "���������", "Save",
    "��������� ������ �� ���������� ������������ ����������",
    "To keep a signal in an internal memory",
    nullptr, PressSB_Save, DrawSB_Save, nullptr
)


void PressSB_SaveToFlashDrive()
{
    ReturnTo::fromSetName = ReturnTo::IntMemory;
    PageMemory::PageExternal::SaveSignalToFlashDrive();
}


void DrawSB_SaveToFlashDrive(int x, int y)
{
    if (FDrive::IsConnected())
    {
        Char('\x42').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
    }
}


DEF_SMALL_BUTTON(sbSaveToFlash, PageMemory::PageInternal::self,
    "���������", "Save",
    "��������� ������ �� ������",
    "Save signal to flash drive",
    nullptr, PressSB_SaveToFlashDrive, DrawSB_SaveToFlashDrive, nullptr
)


void OnPress()
{
    PageMemory::PageInternal::self->OpenAndSetItCurrent();
    set.memory.mode_work = ModeWork::MemInt;

    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);
}


static void DrawMemoryWave(int num, bool exist)
{
    int x = Grid::Left() + 2 + num * 12;
    int y = Grid::FullBottom() - 10;
    int width = 12;
    Region(width, 10).Fill(x, y, (num == PageMemory::PageInternal::currentSignal) ? Color::BLACK_WHITE_0 : Color::BACK);
    Rectangle(width, 10).Draw(x, y, Color::FILL);
    ((num == PageMemory::PageInternal::currentSignal) ? Color::BLACK_WHITE_1 : Color::FILL).SetAsCurrent();
    if (exist)
    {
        Int(num + 1).ToText(false, 2).Draw(x + 2, y + 1);
    }
    else
    {
        Text("\x88").Draw(x + 3, y + 1);
    }
}


static void FuncAdditionDrawing()
{
    // ������ �������� ��������� ������

    bool exist[ROM::Data::MAX_NUM_SAVED_WAVES] = { false };

    ROM::Data::GetInfo(exist);

    for (uint i = 0; i < ROM::Data::MAX_NUM_SAVED_WAVES; i++)
    {
        DrawMemoryWave((int)i, exist[i]);
    }
}


static void OnRegSet(int delta)
{
    Holder::RotateSwitch(delta);

    if (delta < 0)
    {
        Math::CircleDecrease<int8>(&PageMemory::PageInternal::currentSignal, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1);
    }
    else if (delta > 0)
    {
        Math::CircleIncrease<int8>(&PageMemory::PageInternal::currentSignal, 0, ROM::Data::MAX_NUM_SAVED_WAVES - 1);
    }

    //    EPROM::GetData(PageMemory::PageInternal::currentSignal, &Storage::dsInt, &Storage::dataIntA, &Storage::dataIntB);

    Color::ResetFlash();
}


DEF_PAGE_SB(pageInternal, PageMemory::self, NamePage::MemoryInt_SB,
    "����� ��", "INT STORAGE",
    "������� � ����� ������ � ���������� �������",
    "Transition to an operating mode with internal memory",
    sbExit,
    sbShowSignalAlways,
    sbModeShow,
    sbDelete,
    sbSave,
    sbSaveToFlash,
    nullptr, OnPress, FuncAdditionDrawing, OnRegSet
);


const Page *PageMemory::PageInternal::self = &pageInternal;
