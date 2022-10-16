// 2021/05/12 15:50:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void OnPress_Exit()
{
}


DEF_SMALL_BUTTON(cExit, PageService::PageMath::PageFFT::PageCursors::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, OnPress_Exit, DrawSB_Exit, nullptr
)


static void OnPress_Source()
{
    FFT_NUM_CURSOR = (uint8)((FFT_NUM_CURSOR + 1) % 2);
}


static void Draw_Source(int x, int y)
{
    Text((FFT_NUM_CURSOR == 0) ? "1" : "2").Draw(x + 7, y + 5);
}


DEF_SMALL_BUTTON(cSource, PageService::PageMath::PageFFT::PageCursors::self,
    "Источник", "Source",
    "Выбор источника для расчёта спектра",
    "Source choice for calculation of a range",
    nullptr, OnPress_Source, Draw_Source, nullptr
)


static void OnRegSet(int angle)
{
    FFT_POS_CUR(FFT_NUM_CURSOR) += (uint8)angle;
    Sound::RegulatorShiftRotate();
}


static bool IsActive()
{
    return FFT_IS_ENABLED;
}


DEF_PAGE_SB(pageCursorsFFT, PageService::PageMath::PageFFT::self, NamePage::ServiceMathFFTCursors_SB,
    "КУРСОРЫ", "CURSORS",
    "Включает курсоры для измерения параметров спектра",
    "Includes cursors to measure the parameters of the spectrum",
    cExit,
    cSource,
    Item::empty,
    Item::empty,
    Item::empty,
    Item::empty,
    IsActive, nullptr, nullptr, OnRegSet
)

const Page *PageService::PageMath::PageFFT::PageCursors::self = &pageCursorsFFT;
