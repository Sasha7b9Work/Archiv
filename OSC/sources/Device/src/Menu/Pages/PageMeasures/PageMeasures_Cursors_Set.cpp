#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Hardware/Sound.h"
#include <math.h>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSet;

const float PageMeasures::PageCursors::PageSet::MAX_POS_U = 200.0f;
const float PageMeasures::PageCursors::PageSet::MAX_POS_T = 280.0f;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
DEF_SMALL_BUTTON_EXIT(bSet_Exit,                                                                    //--- ��������� - ������� - ���������� - ����� ---
    pageSet, FuncActive, OnPressSB_Exit, DrawSB_Exit
)
*/

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_ChannelA(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "1");
}

static void Draw_Set_ChannelB(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "2");
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_Channel()
{
    Chan source = CURS_SOURCE_A ? Chan(Chan::B) : Chan(Chan::A);
    SetCursSource(source);
}

void PageMeasures::PageCursors::PageSet::Draw_Set_Channel(int x, int y)
{
    static const pFuncVII func[2] = {Draw_Set_ChannelA, Draw_Set_ChannelB};
    func[CURS_SOURCE](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(bSet_Channel,                                                              //--- ��������� - ������� - ���������� - ����� ---
    "�����", "Chan",
    "����� ������ ��� ��������� ���������",
    "Chan choice for measurements",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_Channel, PageMeasures::PageCursors::PageSet::Draw_Set_Channel,
    Draw_Set_ChannelA, {"����� 1", "channel 1"},
    Draw_Set_ChannelB, {"����� 2", "channel 2"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_T_disable(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "T");
}

static void Draw_Set_T_disableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void Draw_Set_T_enableLeft(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void Draw_Set_T_enableRight(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void Draw_Set_T_enableBoth(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_T()
{
    if (CURS_ACTIVE_T || CURsT_DISABLED)
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::T;
}

static void Draw_Set_T(int x, int y)
{
    if (CURsT_DISABLED)
    {
        Draw_Set_T_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_T)
        {
            Draw_Set_T_disableBoth(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            Chan source = CURS_SOURCE;
            CalculateConditions((int16)CURsT_POS(source, 0), (int16)CURsT_POS(source, 1), CURsT_CNTRL, &condLeft, &condDown);
            if (condLeft && condDown)
            {
                Draw_Set_T_enableBoth(x, y);
            }
            else if (condLeft)
            {
                Draw_Set_T_enableLeft(x, y);
            }
            else
            {
                Draw_Set_T_enableRight(x, y);
            }
        }
    }
}

DEF_SMALL_BUTTON_HINTS_5(bSet_T,                                                                //--- ��������� - ������� - ���������� - ������� � ---
    "������� T", "Cursors T",
    "����� �������� ������� ��� ��������� � ����������",
    "Choice of cursors of time for indication and management",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_T, Draw_Set_T,
    Draw_Set_T_disable,     {"������� ������� ���������",                             "cursors of time are switched off"},
    Draw_Set_T_disableBoth, {"������� ������� ��������",                              "cursors of time are switched on"},
    Draw_Set_T_enableLeft,  {"������� ������� ��������, ���������� ����� ��������",   "cursors of time are switched on, control of the left cursor"},
    Draw_Set_T_enableRight, {"������� ������� ��������, ���������� ������ ��������",  "cursors of time are switched on, control of the right cursor"},
    Draw_Set_T_enableBoth,  {"������� ������� ��������, ���������� ������ ���������", "cursors of time are switched on, control of both cursors"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_U_disable(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "U");
}

static void Draw_Set_U_disableBoth(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, false);
}

static void Draw_Set_U_enableUpper(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, false);
}

static void Draw_Set_U_enableLower(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, true);
}

static void Draw_Set_U_enableBoth(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, true);
}

void PageMeasures::PageCursors::PageSet::OnPress_Set_U()
{
    if (CURS_ACTIVE_U || CURsU_DISABLED)
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = Cursors::Active::U;
}

static void Draw_Set_U(int x, int y)
{
    Chan source = CURS_SOURCE;
    if (CURsU_DISABLED)
    {
        Draw_Set_U_disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_U)
        {
            Draw_Set_U_disableBoth(x, y);
        }
        else
        {
            bool condTop = false, condDown = false;
            CalculateConditions((int16)Cursors::PosU(source, 0), (int16)Cursors::PosU(source, 1), CURsU_CNTRL, &condTop, &condDown);
            if (condTop && condDown)
            {
                Draw_Set_U_enableBoth(x, y);
            }
            else if (condTop)
            {
                Draw_Set_U_enableUpper(x, y);
            }
            else
            {
                Draw_Set_U_enableLower(x, y);
            }
        }
    }
}

// ����� ������� ���������� - ������ 1, ������ 2, ��� ������� ��� ���������.
DEF_SMALL_BUTTON_HINTS_5(bSet_U,                                                                //--- ��������� - ������� - ���������� - ������� U ---
    "������� U", "Cursors U",
    "����� �������� ���������� ��� ��������� � ����������",
    "Choice of cursors of voltage for indication and management",
    pageSet, FuncActive, PageMeasures::PageCursors::PageSet::OnPress_Set_U, Draw_Set_U,
    Draw_Set_U_disable, {"������� ���������� ���������",
                            "cursors of tension are switched off"},
    Draw_Set_U_disableBoth, {"������� ���������� ��������",
                            "cursors of tension are switched on"},
    Draw_Set_U_enableUpper, {"������� ���������� ��������, ���������� ������� ��������",
                            "cursors of tension are switched on, control of the top cursor"},
    Draw_Set_U_enableLower, {"������� ���������� ��������, ���������� ������ ��������",
                            "cursors of tension are switched on, control of the lower cursor"},
    Draw_Set_U_enableBoth, {"������� ���������� ��������, ���������� ������ ���������",
                            "cursors of tension are switched on, control of both cursors"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Set_100()
{
    PageMeasures::PageCursors::PageSet::SetCursPos100(CURS_SOURCE);
}

static void Draw_Set_100(int x, int y)
{
    Painter::SetFont(Font::Type::_5);
    Painter::DrawText(x + 2, y + 3, "100%");
    Painter::SetFont(Font::Type::_8);
}

// ��������� 100 ��������� � ������� ����� ��������.
DEF_SMALL_BUTTON(bSet_100,                                                                           //--- ��������� - ������� - ���������� - 100% ---
    "100%", "100%",
    "������������ ��� ���������� ���������. ������� �������� ���������� ����� ��������� ��������� ��� 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    pageSet, FuncActive, OnPress_Set_100, Draw_Set_100
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_Movement_Percents(int x, int y)
{
    Painter::DrawText(x + 6, y + 5, "\x83");
}

static void Draw_Set_Movement_Points(int x, int y)
{
    Painter::SetFont(Font::Type::_5);
    Painter::DrawText(x + 4, y + 3, "���");
    Painter::SetFont(Font::Type::_8);
}

static void OnPress_Set_Movement()
{
    Math::CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void Draw_Set_Movement(int x, int y)
{
    if (CURS_MOVEMENT_IN_PERCENTS)
    {
        Draw_Set_Movement_Percents(x, y);
    }
    else
    {
        Draw_Set_Movement_Points(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(bSet_Movement,                                                       //--- ��������� - ������� - ���������� - ����������� ---
    "�����������", "Movement",
    "����� ���� ����������� �������� - �������� ��� �����",
    "Choice of a step of movement of cursors - percent or points",
    pageSet, FuncActive, OnPress_Set_Movement, Draw_Set_Movement,
    Draw_Set_Movement_Percents, {"��� ����������� �������� ������ ������ ��������", "the step of movement of cursors is multiple to one percent"},
    Draw_Set_Movement_Points, {"��� ����������� ������� ������ ������ �������",   "the step of movement of the cursor is multiple to one pixel"}
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PageMeasures::PageCursors::PageSet::OnRegSet(int angle)
{
    float value = (float)angle;

    if (CURS_ACTIVE_U)
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dUperc(CURS_SOURCE) / 100.0f;
        }

        if (CURsU_CNTRL_1 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 0, value);
        }
        if (CURsU_CNTRL_2 || CURsU_CNTRL_1_2)
        {
            SetShiftCursPosU(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    else
    {
        if (CURS_MOVEMENT_IN_PERCENTS)
        {
            value *= dTperc(CURS_SOURCE) / 100.0f;
        }

        if (CURsT_CNTRL_1 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 0, value);
        }
        if (CURsT_CNTRL_2 || CURsT_CNTRL_1_2)
        {
            SetShiftCursPosT(CURS_SOURCE, 1, value);
        }
        UpdateCursorsForLook();
    }
    Sound::RegulatorShiftRotate();

    return true;
}

/*
DEF_PAGE_SB(pageSet,                                                                                        //--- ��������� - ������� - ���������� ---
    "����������", "SET",
    "������� � ����� ��������� ���������",
    "Switch to cursor measures",
    &bSet_Exit,             ///< ��������� - ������� - ���������� - �����
    &bSet_Channel,          ///< ��������� - ������� - ���������� - �����
    &bSet_U,                ///< ��������� - ������� - ���������� - ������� U
    &bSet_T,                ///< ��������� - ������� - ���������� - ������� �
    &bSet_100,              ///< ��������� - ������� - ���������� - 100%
    &bSet_Movement,         ///< ��������� - ������� - ���������� - �����������
    Page::Name::Measures_Cursors_Set, PageMeasures::PageCursors::pointer, FuncActive, EmptyPressPage, FuncDrawPage, 
            PageMeasures::PageCursors::PageSet::OnRegSet
)
*/


DEF_PAGE_5(pageSet,                                                                                        //--- ��������� - ������� - ���������� ---
    "����������", "SET",
    "������� � ����� ��������� ���������",
    "Switch to cursor measures",
    &bSet_Channel,          ///< ��������� - ������� - ���������� - �����
    &bSet_U,                ///< ��������� - ������� - ���������� - ������� U
    &bSet_T,                ///< ��������� - ������� - ���������� - ������� �
    &bSet_100,              ///< ��������� - ������� - ���������� - 100%
    &bSet_Movement,         ///< ��������� - ������� - ���������� - �����������
    Page::Name::Measures_Cursors_Set, PageMeasures::PageCursors::pointer, FuncActive, EmptyPressPage, FuncDrawPage,
    PageMeasures::PageCursors::PageSet::OnRegSet
)



const PageBase *PageMeasures::PageCursors::PageSet::pointer = &pageSet;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursSource(Chan ch)
{
    CURS_SOURCE = ch;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::IncCursCntrlU(Chan ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsU_CNTRL_CH(ch), 0, 3);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::IncCursCntrlT(Chan ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURsT_CNTRL_CH(ch), 0, 3);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursPos100(Chan ch)
{
    dUperc(ch) = (float)fabsf(CURsU_POS(ch, 0) - CURsU_POS(ch, 1));
    dTperc(ch) = (float)fabsf(CURsT_POS(ch, 0) - CURsT_POS(ch, 1));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetShiftCursPosU(Chan ch, int numCur, float delta)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(CURsU_POS(ch, numCur) - delta, 0.0f, MAX_POS_U);

    if (CURS_MOVEMENT_IN_PIXELS)                        // ���� ����������� �� ��������, �� ����� �������� � ���������� ����� ������
    {
        /// \todo
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetShiftCursPosT(Chan ch, int numCur, float delta)
{
    /// \todo ���������� �����
    // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);   
    Cursors::SetCursPosT_temp(ch, numCur, Math::LimitationRet(CURsT_POS(ch, numCur) + delta, 0.0f, MAX_POS_T));

    if (CURS_MOVEMENT_IN_PIXELS)                        // ���� ����������� �� ��������, �� ����� �������� � ���������� ����� ������
    {
        /// \todo
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::UpdateCursorsForLook()
{
    Chan source = CURS_SOURCE;

    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorU(source, 0, Measure::CalculateCursorU(source, CURsT_POS(source, 0)));
    }
    if (CURS_ACTIVE_T && (CURS_LOOK_U(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorU(source, 1, Measure::CalculateCursorU(source, CURsT_POS(source, 1)));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::A) || CURS_LOOK_BOTH(Chan::A)))
    {
        SetCursorT(source, 0, Measure::CalculateCursorT(source, CURsU_POS(source, 0), 0));
    }
    if (CURS_ACTIVE_U && (CURS_LOOK_T(Chan::B) || CURS_LOOK_BOTH(Chan::B)))
    {
        SetCursorT(source, 1, Measure::CalculateCursorT(source, CURsU_POS(source, 1), 1));
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursorU(Chan ch, int numCur, float pos)
{
    CURsU_POS(ch, numCur) = Math::LimitationRet(pos, 0.0f, MAX_POS_U);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMeasures::PageCursors::PageSet::SetCursorT(Chan ch, int numCur, float pos)
{
    // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);      /// \todo ���������� �����
    Cursors::SetCursPosT_temp(ch, numCur, Math::LimitationRet(pos, 0.0f, MAX_POS_T));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageMeasures::PageCursors::PageSet::IsRegSetActiveOnCursors()
{
    return ((Menu::GetNameOpenedPage() == Page::Name::Measures_Cursors_Set) &&
        ((CURS_ACTIVE_U && CURsU_ENABLED) || (CURS_ACTIVE_T && CURsT_ENABLED)));
}
