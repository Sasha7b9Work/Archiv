#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Display/DisplayTypes.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Device.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/Pages/Include/PageTime.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase page;


PageBase *Menu::pageMain = (PageBase *)&page;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_10(page,                                                                                                                       //--- ���� ---
    "����", "MENU",
    "", "",
    PageDisplay::pointer,      // �������
    PageChannelA::pointer,     // ����� 1
    PageChannelB::pointer,     // ����� 2
    PageTrig::pointer,         // �����
    PageTime::pointer,         // ���������
    PageFunction::pointer,     // �������
    PageMemory::pointer,       // ������
    PageMeasures::pointer,     // ���������
    PageService::pointer,      // ������
    //pHelp,                    // ������
    PageDebug::pointer,        // �������
    Page::Name::Main, 0, FuncActive, EmptyPressPage
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const void *Menu::PageForButton(Key button)
{
    static const void *pages[Key::Number] =
    {  
        0,                              // None
        (void *)PageFunction::pointer,  // Function  1
        (void *)PageMeasures::pointer,  // Measures  2
        (void *)PageMemory::pointer,    // Memory    3
        (void *)PageService::pointer,   // Service   4
        (void *)PageChannelA::pointer,  // ChannelA  5
        (void *)PageChannelB::pointer,  // ChannelB  6
        (void *)PageTime::pointer,      // Time      7
        0,                              // Start     
        (void *)PageTrig::pointer,      // Trig      9
        (void *)PageDisplay::pointer    // Display  10
    };

    return pages[button];
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsMainPage(const void *item)
{
    return item == pageMain;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawSB_Exit(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x2e');
    Painter::SetFont(Font::Type::_8);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressSB_Exit()
{
    Display::RemoveAddDrawFunction();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateYforCurs(int y, bool top)
{
    return top ? y + Menu::Item::HEIGHT / 2 + 4 : y + Menu::Item::HEIGHT - 2;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateXforCurs(int x, bool left)
{
    return left ? x + Menu::Item::WIDTH - 20 : x + Menu::Item::WIDTH - 5;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateXY(int *x0, int *x1, int *y0, int *y1)
{
    *x0 = CalculateXforCurs(*x0, true);
    *x1 = CalculateXforCurs(*x1, false);
    *y0 = CalculateYforCurs(*y0, true);
    *y1 = CalculateYforCurs(*y1, false);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 65;
    y -= 21;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for (int i = 0; i < (top ? 3 : 1); i++)
    {
        Painter::DrawHLine(y0 + i, x0, x1);
    }
    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
        Painter::DrawHLine(y1 - i, x0, x1);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateConditions(int16 pos0, int16 pos1, Cursors::Control cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == Cursors::Control::_1_2 || (cursCntrl == Cursors::Control::_1 && zeroLessFirst) || (cursCntrl == Cursors::Control::_2 && !zeroLessFirst);
    *cond1 = cursCntrl == Cursors::Control::_1_2 || (cursCntrl == Cursors::Control::_1 && !zeroLessFirst) || (cursCntrl == Cursors::Control::_2 && zeroLessFirst);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x -= 58;
    y -= 16;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);
    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        Painter::DrawVLine(x0 + i, y0, y1);
    }
    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        Painter::DrawVLine(x1 - i, y0, y1);
    }
}
