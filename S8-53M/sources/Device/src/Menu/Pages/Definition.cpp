// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Display/Display.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Panel/Panel.h"


void CalculateConditions(int16 pos0, int16 pos1, CursCntrl::E cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && zeroLessFirst) || (cursCntrl == CursCntrl::_2 && !zeroLessFirst); //-V537
    *cond1 = cursCntrl == CursCntrl::_1_2 || (cursCntrl == CursCntrl::_1 && !zeroLessFirst) || (cursCntrl == CursCntrl::_2 && zeroLessFirst);
}


void DrawSB_Exit(int x, int y)
{
    Char('\x2e').Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}


#include "PageHelp.cpp"


static const Page *nullPage = (const Page *)0x00000000;


DEF_PAGE_11(mainPage, &Page::empty, NamePage::MainPage,
    "Ã≈Õﬁ", "MENU",
    "",
    "",
    *PageDisplay::self,
    *PageChannel::A::self,
    *PageChannel::B::self,
    *PageTrig::self,
    *PageTime::self,
    *PageCursors::self,
    *PageMemory::self,
    *PageMeasures::self,
    *PageService::self,
    *PageHelp::self,
    *nullPage,
    nullptr, nullptr, nullptr, nullptr
)


void PageMain::EnablePageDebug()
{
    const DataPage* data = PageMain::self->OwnData();

    data->items[10] = (const Item *)&*PageDebug::self;

    Warnings::ShowWarningGood("Ã≈Õﬁ Œ“À¿ƒ ¿ ¬ Àﬁ◊≈ÕŒ", "DEBUGGING MENU ON");
}


const Page *PageForButton(Key::E key)
{
    static const void *pages[Key::Count] = {  
        (void *)nullptr,                // Key::None
        (void *)nullptr,                // Key::F1
        (void *)nullptr,                // Key::F2
        (void *)nullptr,                // Key::F3
        (void *)nullptr,                // Key::F4
        (void *)nullptr,                // Key::F5
        (void *)PageChannel::A::self,   // Key::ChannelA
        (void *)PageChannel::B::self,   // Key::ChannelB
        (void *)PageTime::self,         // Key::Time
        (void *)PageTrig::self,         // Key::Trig
        (void *)PageCursors::self,      // Key::Cursors
        (void *)PageMeasures::self,     // Key::Measures
        (void *)PageDisplay::self,      // Key::Display
        (void *)nullptr,                // Key::Help
        (void *)nullptr,                // Key::Start
        (void *)PageMemory::self,       // Key::Memory
        (void *)PageService::self,      // Key::Service
        (void *)nullptr,                // Key::Menu
        (void *)nullptr,                // Key::Power
        (void *)nullptr,                // Key::RangeA
        (void *)nullptr,                // Key::RangeB
        (void *)nullptr,                // Key::RShiftA
        (void *)nullptr,                // Key::RShiftB
        (void *)nullptr,                // Key::TBase
        (void *)nullptr,                // Key::TShift
        (void *)nullptr,                // Key::TrigLev
        (void *)nullptr                 // Key::Setting
    };

    return reinterpret_cast<const Page *>(pages[key]);
}


const Page *PageMain::self = &mainPage;


bool IsMainPage(const void *item)
{
    return (item == PageMain::self);
}
