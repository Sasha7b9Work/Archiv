#include "defines.h"
#include "PageCursors.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTypes.h"
#include "Panel/Controls.h"
#include "Utils/ProcessingSignal.h"
#include "Log.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include <math.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pCursors;
extern const PageBase ppSet;

static void MoveCursUonPercentsOrPoints(int delta);
static void MoveCursTonPercentsOrPoints(int delta);

static void SetShiftCursPosU(Channel ch, int numCur, float delta);      ///< �������� �������� ������� ������� ���������� �� delta �����.
static void SetShiftCursPosT(Channel ch, int numCurs, float delta);     ///< �������� �������� ������� ������� ������� �� delta �����.
static void SetCursPos100(Channel ch);                                  ///< ��������� ������� ��������, ��������������� 100%.
static void SetCursSource(Channel ch);                                  ///< ���������� �������� ��������� ���������.
static void IncCursCntrlU(Channel ch);                                  ///< ������� ��������� ������.
static void IncCursCntrlT(Channel ch);                                  ///< ������� ��������� ������.
static void SetCursPosU(Channel ch, int numCur, float pos);             ///< ���������� ������� ������� ����������.
//static void SetCursPosT(Channel ch, int numCur, float pos);             ///< ���������� �������� ������� �� �������.

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cShow,                                                                                              //--- ������� - ���������� ---
    "����������", "Shown",
    "��������/��������� �������.",
    "Enable/disable cursors.",
    "���", "No",
    "��",  "Yes",
    CURS_SHOW, pCursors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cLookModeChanA,                                                                               //--- ������� - �������� ����� 1 ---
    "�������� \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    /// \todo �������
    "Sets the mode tracking for second cursor:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "����������",     "Voltage",
    "�����",          "Time",
    "������ � �����", "Volt and time",
    CURS_LOOK_MODE(A), pCursors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cLookModeChanB,                                                                               //--- ������� - �������� ����� 2 ---
    "�������� \x8f, \x9f", "Tracking \x8f, \x9f"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    "Sets the mode tracking cursors channel 2:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU,        DISABLE_EN,
    "����������",      "Voltage",
    "�����",           "Time",
    "������. � �����", "Volt. and time",
    CURS_LOOK_MODE(B), pCursors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cShowFreq,                                                                                                //--- ������� - 1/dT ---
    "1/dT", "1/dT",
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ "
    "�������� ����� ���������� ����� ���������� ���������.",
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - frequency signal, a period "
    "equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    CURSORS_SHOW_FREQ, pCursors, FuncActive, FuncChangedChoice, FuncDraw
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bSet_Exit,                                                                              //--- ������� - ���������� - ����� ---
    ppSet, FuncActive, OnPressSB_Exit, DrawSB_Exit
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_ChannelA(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "1");
}

static void Draw_Set_ChannelB(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "2");
}

static void OnPress_Set_Channel()
{
    Channel source = CURS_SOURCE_A ? B : A;
    SetCursSource(source);
}

static void Draw_Set_Channel(int x, int y)
{
    static const pFuncVII func[2] = {Draw_Set_ChannelA, Draw_Set_ChannelB};
    func[CURS_SOURCE](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   bSet_Channel,                                                                       //--- ������� - ���������� - ����� ---
    "�����", "Channel",
    "����� ������ ��� ��������� ���������",
    "Channel choice for measurements",
    ppSet, FuncActive, OnPress_Set_Channel, Draw_Set_Channel,
    Draw_Set_ChannelA, "����� 1", "channel 1",
    Draw_Set_ChannelB, "����� 2", "channel 2"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

static void OnPress_Set_U()
{
    if (CURS_ACTIVE_U || CURsU_DISABLED)
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive_U;
}

static void Draw_Set_U(int x, int y)
{
    Channel source = CURS_SOURCE;
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
            CalculateConditions((int16)sCursors_GetCursPosU(source, 0), (int16)sCursors_GetCursPosU(source, 1), CURsU_CNTRL, &condTop, &condDown);
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
DEF_SMALL_BUTTON_HINTS_5(   bSet_U,                                                                         //--- ������� - ���������� - ������� U ---
    "������� U", "Cursors U",
    "����� �������� ���������� ��� ��������� � ����������",
    "Choice of cursors of voltage for indication and management",
    ppSet, FuncActive, OnPress_Set_U, Draw_Set_U,
    Draw_Set_U_disable,     "������� ���������� ���������",
                            "cursors of tension are switched off",
    Draw_Set_U_disableBoth, "������� ���������� ��������",
                            "cursors of tension are switched on",
    Draw_Set_U_enableUpper, "������� ���������� ��������, ���������� ������� ��������",
                            "cursors of tension are switched on, control of the top cursor",
    Draw_Set_U_enableLower, "������� ���������� ��������, ���������� ������ ��������",
                            "cursors of tension are switched on, control of the lower cursor",
    Draw_Set_U_enableBoth,  "������� ���������� ��������, ���������� ������ ���������",
                            "cursors of tension are switched on, control of both cursors"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

static void OnPress_Set_T()
{
    if (CURS_ACTIVE_T || CURsT_DISABLED)
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive_T;
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
            Channel source = CURS_SOURCE;
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

DEF_SMALL_BUTTON_HINTS_5(   bSet_T,                                                                         //--- ������� - ���������� - ������� � ---
    "������� T", "Cursors T",
    "����� �������� ������� ��� ��������� � ����������",
    "Choice of cursors of time for indication and management",
    ppSet, FuncActive, OnPress_Set_T, Draw_Set_T,
    Draw_Set_T_disable,     "������� ������� ���������",                             "cursors of time are switched off",
    Draw_Set_T_disableBoth, "������� ������� ��������",                              "cursors of time are switched on",
    Draw_Set_T_enableLeft,  "������� ������� ��������, ���������� ����� ��������",   "cursors of time are switched on, control of the left cursor",
    Draw_Set_T_enableRight, "������� ������� ��������, ���������� ������ ��������",  "cursors of time are switched on, control of the right cursor",
    Draw_Set_T_enableBoth,  "������� ������� ��������, ���������� ������ ���������", "cursors of time are switched on, control of both cursors"
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Set_100()
{
    SetCursPos100(CURS_SOURCE);
}

static void Draw_Set_100(int x, int y)
{
    Painter::SetFont(TypeFont_5);
    Painter::DrawText(x + 2, y + 3, "100%");
    Painter::SetFont(TypeFont_8);
}

// ��������� 100 ��������� � ������� ����� ��������.
DEF_SMALL_BUTTON(   bSet_100,                                                                                    //--- ������� - ���������� - 100% ---
    "100%", "100%",
    "������������ ��� ���������� ���������. ������� �������� ���������� ����� ��������� ��������� ��� 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    ppSet, FuncActive, OnPress_Set_100, Draw_Set_100
);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Set_Movement_Percents(int x, int y)
{
    Painter::DrawText(x + 6, y + 5, "\x83");
}

static void Draw_Set_Movement_Points(int x, int y)
{
    Painter::SetFont(TypeFont_5);
    Painter::DrawText(x + 4, y + 3, "���");
    Painter::SetFont(TypeFont_8);
}

static void OnPress_Set_Movement()
{
    CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void Draw_Set_Movement(int x, int y)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        Draw_Set_Movement_Percents(x, y);
    }
    else
    {
        Draw_Set_Movement_Points(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(   bSet_Movement,                                                                //--- ������� - ���������� - ����������� ---
    "�����������", "Movement",
    "����� ���� ����������� �������� - �������� ��� �����",
    "Choice of a step of movement of cursors - percent or points",
    ppSet, FuncActive, OnPress_Set_Movement, Draw_Set_Movement,
    Draw_Set_Movement_Percents, "��� ����������� �������� ������ ������ ��������", "the step of movement of cursors is multiple to one percent",
    Draw_Set_Movement_Points,   "��� ����������� ������� ������ ������ �������",   "the step of movement of the cursor is multiple to one pixel"
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnRegSet_Set(int angle)
{
    if (CURS_ACTIVE_U)
    {
        MoveCursUonPercentsOrPoints(angle);
    }
    else
    {
        MoveCursTonPercentsOrPoints(angle);
    }
    Sound::RegulatorShiftRotate();
}

DEF_PAGE_SB(        ppSet,                                                                                                 // ������� - ���������� ///
    "����������", "SET",
    "������� � ����� ��������� ���������",
    "Switch to cursor measures",
    &bSet_Exit,             // ������� - ���������� - �����
    &bSet_Channel,          // ������� - ���������� - �����
    &bSet_U,                // ������� - ���������� - ������� U
    &bSet_T,                // ������� - ���������� - ������� �
    &bSet_100,              // ������� - ���������� - 100%
    &bSet_Movement,         // ������� - ���������� - �����������
    PageSB_Cursors_Set, &pCursors, FuncActive, FuncPress, FuncDrawPage, OnRegSet_Set
);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pCursors,                                                                                                           // ������� ///
    "�������", "CURSORS",
    "��������� ���������.",
    "Cursor measurements.",
    cShow,              // ������� - ����������
    cLookModeChanA,     // ������� - �������� ����� 1
    cLookModeChanB,     // ������� - �������� ����� 2
    cShowFreq,          // ������� - 1/dT
    ppSet,              // ������� - ����������
    Page_Cursors, &mainPage, FuncActive, EmptyPressPage
);

const PageBase * pointerPageCursors = &pCursors;

static void CalculateConditions(int16 pos0, int16 pos1, CursCntrl cursCntrl, bool *cond0, bool *cond1)
{
    bool zeroLessFirst = pos0 < pos1;
    *cond0 = cursCntrl == CursCntrl_1_2 || (cursCntrl == CursCntrl_1 && zeroLessFirst) || (cursCntrl == CursCntrl_2 && !zeroLessFirst);
    *cond1 = cursCntrl == CursCntrl_1_2 || (cursCntrl == CursCntrl_1 && !zeroLessFirst) || (cursCntrl == CursCntrl_2 && zeroLessFirst);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_POS_U   200.0f
#define MAX_POS_T   280.0f

static void SetShiftCursPosU(Channel ch, int numCur, float delta)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURsU_POS(ch, numCur) = LimitationRet(CURsU_POS(ch, numCur) - delta, 0.0f, MAX_POS_U);   /// \todo ���������� �����
    }
    else
    {
        CURsU_POS(ch, numCur) = LimitationRet(CURsU_POS(ch, numCur) - delta, 0.0f, MAX_POS_U);
    }
}

static void SetCursPosU(Channel ch, int numCur, float pos)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURsU_POS(ch, numCur) = LimitationRet(pos, 0.0f, MAX_POS_U);                             /// \todo ���������� �����
    }
    else
    {
        CURsU_POS(ch, numCur) = LimitationRet(pos, 0.0f, MAX_POS_U);
    }
}

void SetShiftCursPosT(Channel ch, int numCur, float delta)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);   /// \todo ���������� �����
        SetCursPosT_temp(ch, numCur, LimitationRet(CURsT_POS(ch, numCur) + delta, 0.0f, MAX_POS_T));
    }
    else
    {
        // CURsT_POS(ch, numCur) = LimitationFloat(CURsT_POS(ch, numCur) + delta, 0, MAX_POS_T);
        SetCursPosT_temp(ch, numCur, LimitationRet(CURsT_POS(ch, numCur) + delta, 0.0f, MAX_POS_T));
    }
}

void SetCursPosT(Channel ch, int numCur, float pos)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);                             /// \todo ���������� �����
        SetCursPosT_temp(ch, numCur, LimitationRet(pos, 0.0f, MAX_POS_T));
    }
    else
    {
        // CURsT_POS(ch, numCur) = LimitationFloat(pos, 0, MAX_POS_T);
        SetCursPosT_temp(ch, numCur, LimitationRet(pos, 0.0f, MAX_POS_T));
    }
}

static void SetCursPos100(Channel ch)
{
    dUperc(ch) = (float)fabsf(CURsU_POS(ch, 0) - CURsU_POS(ch, 1));
    dTperc(ch) = (float)fabsf(CURsT_POS(ch, 0) - CURsT_POS(ch, 1));
}

static void SetCursSource(Channel ch)
{
    CURS_SOURCE = ch;
}

static void IncCursCntrlU(Channel ch)
{
    CircleIncrease<int8>((int8 *)&CURsU_CNTRL_CH(ch), 0, 3);
}

static void IncCursCntrlT(Channel ch)
{
    CircleIncrease<int8>((int8 *)&CURsT_CNTRL_CH(ch), 0, 3);
}

void CursorsUpdate()
{
    Channel source = CURS_SOURCE;

    float posT0 = 0.0f, posT1 = 0.0f;

    if(CURS_ACTIVE_T && (CURS_LOOK_U(0) || CURS_LOOK_BOTH(0)))
    {
        float posU0 = processing.GetCursU(source, CURsT_POS(source, 0));
        SetCursPosU(source, 0, posU0);
    }
    if(CURS_ACTIVE_T && (CURS_LOOK_U(1) || CURS_LOOK_BOTH(1)))
    {
        float posU1 = processing.GetCursU(source, CURsT_POS(source, 1));
        SetCursPosU(source, 1, posU1);
    }
    if(CURS_ACTIVE_U && (CURS_LOOK_T(0) || CURS_LOOK_BOTH(0)))
    {
        float posU0 = CURsU_POS(source, 0);
        posT0 = processing.GetCursT(source, posU0, 0);
        SetCursPosT(source, 0, posT0);
    }
    if(CURS_ACTIVE_U && (CURS_LOOK_T(1) || CURS_LOOK_BOTH(1)))
    {
        float posU1 = CURsU_POS(source, 1);
        posT1 = processing.GetCursT(source, posU1, 1);
        SetCursPosT(source, 1, posT1);
    }
}

static void MoveCursUonPercentsOrPoints(int delta)
{
    float value = (float)delta;

    if (CURS_MOVEMENT_IS_PERCENTS)
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
    CursorsUpdate();
}

static void MoveCursTonPercentsOrPoints(int delta)
{
    float value = (float)delta;

    if (CURS_MOVEMENT_IS_PERCENTS)
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
    CursorsUpdate();
}

bool IsRegSetActiveOnCursors()
{
    return ((Menu::GetNameOpenedPage() == PageSB_Cursors_Set) &&
        ((CURS_ACTIVE_U && CURsU_ENABLED) || (CURS_ACTIVE_T && CURsT_ENABLED)));
}
