#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFFT;
extern const PageBase pppFFT_Cursors;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cFFT_View,                                                                                       //--- ������� - ������ - ����������� ---
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FFT_ENABLED, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cFFT_Scale,                                                                                            //--- ������� - ������ - ����� ---
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    SCALE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cFFT_Source,                                                                                        //--- ������� - ������ - �������� ---
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1", "Chan 1",
    "����� 2", "Chan 2",
    "����� 1 + 2", "Chan 1 + 2",
    SOURCE_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(cFFT_Window,                                                                                            //--- ������� - ������ - ���� ---
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������", "Hamming",
    "��������", "Blackman",
    "�����", "Hann",
    WINDOW_FFT, ppFFT, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_FFT_Range()
{
    return SCALE_FFT_IS_LOG;
}

DEF_CHOICE_3(cFFT_Range,                                                                                         //--- ������� - ������ - �������� ---
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    MAX_DB_FFT, ppFFT, IsActive_FFT_Range, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
DEF_SMALL_BUTTON_EXIT(bFFT_Cursors_Exit,                                                                  //--- ������� - ������ - ������� - ����� ---
    pppFFT_Cursors, FuncActive, OnPressSB_Exit, DrawSB_Exit
)
*/

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_FFT_Cursors_Source()
{
    MATH_CURRENT_CUR = (uint8)((MATH_CURRENT_CUR + 1) % 2);
}

static void Draw_FFT_Cursors_Source(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, MATH_CURRENT_CUR_IS_0 ? "1" : "2");
}

DEF_SMALL_BUTTON(bFFT_Cursors_Source,                                                                  //--- ������� - ������ - ������� - �������� ---
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Source choice for calculation of a range",
    pppFFT_Cursors, FuncActive, OnPress_FFT_Cursors_Source, Draw_FFT_Cursors_Source
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_FFT_Cursors()
{
    return FFT_ENABLED;
}

static bool OnRegSet_FFT_Cursors(int angle)
{
    FFT_POS_CURSOR(MATH_CURRENT_CUR) += (uint8)angle;
    Sound::RegulatorShiftRotate();

    return true;
}

/*
DEF_PAGE_SB(pppFFT_Cursors,                                                                                       //--- ������� - ������ - ������� ---
    "�������", "CURSORS",
    "�������� ������� ��� ��������� ���������� �������",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Exit,     ///< ������ - ������ - ������� - �����
    &bFFT_Cursors_Source,   ///< ������ - ������ - ������� - ��������
    0,
    0,
    0,
    0,
    Page::Name::SB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors, EmptyPressPage, FuncDrawPage, OnRegSet_FFT_Cursors
)
*/

DEF_PAGE_1(pppFFT_Cursors,                                                                                       //--- ������� - ������ - ������� ---
    "�������", "CURSORS",
    "�������� ������� ��� ��������� ���������� �������",
    "Includes cursors to measure the parameters of the spectrum",
    &bFFT_Cursors_Source,   ///< ������ - ������ - ������� - ��������
    Page::Name::SB_Service_FFT_Cursors, &ppFFT, IsActive_FFT_Cursors, EmptyPressPage, FuncDrawPage, OnRegSet_FFT_Cursors
)



static bool IsActive_FFT()
{
    return !FUNC_MODE_DRAW_IS_ENABLED;
}

static void OnPress_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning(Warning::ImpossibleEnableFFT);
    }
}

const PageBase *PageFunction::PageFFT::pointer = &ppFFT;


DEF_PAGE_6(ppFFT,                                                                                                            //--- ������ - ������ ---
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    &cFFT_View,         ///< ������ - ������ - �����������
    &cFFT_Scale,        ///< ������ - ������ - �����
    &cFFT_Source,       ///< ������ - ������ - ��������
    &cFFT_Window,       ///< ������ - ������ - ���� 
    &pppFFT_Cursors,    ///< ������ - ������ - �������
    &cFFT_Range,        ///< ������ - ������ - ��������
    Page::Name::Function_FFT, PageFunction::pointer, IsActive_FFT, OnPress_FFT
)
