#include "defines.h"
#include "PageCursors.h"
#include "Menu/Pages/Definition.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Hardware/Controls.h"
#include "Display/Colors.h"
#include "Display/Grid.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Hardware/Sound.h"
#include "Log.h"
#include "Display/Painter.h"
#include <math.h>
#include <string.h>


/** @addtogroup Menu
 *  @{
 *  @addtogroup PageCursors
 *  @{
 */

extern const PageBase mainPage;
extern const PageBase pCursors;
extern const PageBase mspSet;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void MoveCursUonPercentsOrPoints(int delta);
static void MoveCursTonPercentsOrPoints(int delta);
static void SetShiftCursPosU(Channel chan, int numCur, float delta);    ///< �������� �������� ������� ������� ���������� �� delta �����
static void SetShiftCursPosT(Channel chan, int numCurs, float delta);   ///< �������� �������� ������� ������� ������� �� delta �����
static void SetCursPos100(Channel chan);                                ///< ��������� ������� ��������, ��������������� 100%.
static void SetCursSource(Channel chann);                               ///< ���������� �������� ��������� ���������.
static void IncCursCntrlU(Channel chan);                                ///< ������� ��������� ������.
static void IncCursCntrlT(Channel chan);                                ///< ������� ��������� ������.
static void SetCursPosU(Channel chan, int numCur, float pos);           ///< ���������� ������� ������� ����������.
static void SetCursPosT(Channel chan, int numCur, float pos);           ///< ���������� �������� ������� �� �������.


#define MAX_POS_U   200.0f
#define MAX_POS_T   280.0f


//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetCursSource(Channel chan)
{
    CURS_SOURCE = chan;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void IncCursCntrlU(Channel chan)
{
    CircleIncrease<int8>((int8 *)&CURsU_CNTRL_CH(chan), 0, 3);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void IncCursCntrlT(Channel chan)
{
    CircleIncrease<int8>((int8 *)&CURS_CNTRL_T(chan), 0, 3);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors_Update()
{
    Channel source = CURS_SOURCE;
    CursLookMode lookMode0 = CURS_LOOKMODE_A;
    CursLookMode lookMode1 = CURS_LOOKMODE_B;

    float posT0 = 0.0f, posT1 = 0.0f;

    if((lookMode0 == CursLookMode_Voltage || lookMode0 == CursLookMode_Both) && CURS_ACTIVE_IS_T)
    {
        float posU0 = Processing::GetCursU(source, CURS_POS_T0(source));
        SetCursPosU(source, 0, posU0);
    }
    if((lookMode1 == CursLookMode_Voltage || lookMode1 == CursLookMode_Both)  && CURS_ACTIVE_IS_T)
    {
        float posU1 = Processing::GetCursU(source, CURS_POS_T1(source));
        SetCursPosU(source, 1, posU1);
    }
    if((lookMode0 == CursLookMode_Time || lookMode0 == CursLookMode_Both) && CURS_ACTIVE_IS_U)
    {
        float posU0 = CURS_POS_U0(source);
        posT0 = Processing::GetCursT(source, posU0, 0);
        SetCursPosT(source, 0, posT0);
    }
    if((lookMode1 == CursLookMode_Time || lookMode1 == CursLookMode_Both) && CURS_ACTIVE_IS_U)
    {
        float posU1 = CURS_POS_U1(source);
        posT1 = Processing::GetCursT(source, posU1, 1);
        SetCursPosT(source, 1, posT1);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetCursPosU(Channel chan, int numCur, float pos)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURS_POS_U(chan, numCur) = LimitationRet(pos, 0.0f, MAX_POS_U);
    }
    else
    {
        CURS_POS_U(chan, numCur) = LimitationRet(pos, 0.0f, MAX_POS_U);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetCursPosT(Channel chan, int numCur, float pos)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURS_POS_T(chan, numCur) = LimitationRet(pos, 0.0f, (float)MAX_POS_T);
    }
    else
    {
        CURS_POS_T(chan, numCur) = LimitationRet(pos, 0.0f, (float)MAX_POS_T);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  sbSetExit,                                                                              //--- ������� - ���������� - ����� ---
    mspSet, FuncActive, OnPressSB_Exit, DrawSB_Exit
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_Cursors_Source()
{
    Channel source = CURS_SOURCE_A ? B : A;
    SetCursSource(source);
}

static void DrawSB_Cursors_SourceA(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "1");
}

static void DrawSB_Cursors_SourceB(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "2");
}

static void DrawSB_Cursors_Source(int x, int y)
{
    CURS_SOURCE_A ? DrawSB_Cursors_SourceA(x, y) : DrawSB_Cursors_SourceB(x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   sbSetSource,                                                                     //--- ������� - ���������� - �������� ---
    "��������", "Source",
    "����� ������ ��� ��������� ���������",
    "Channel choice for measurements",
    mspSet, FuncActive, PressSB_Cursors_Source, DrawSB_Cursors_Source,
    DrawSB_Cursors_SourceA, {"����� 1", "channel 1"},
    DrawSB_Cursors_SourceB, {"����� 2", "channel 2"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_Cursors_U()
{
    if (CURS_ACTIVE_IS_U || CURS_CNTRL_U_IS_DISABLE(CURS_SOURCE))
    {
        IncCursCntrlU(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive_U;
}

static void DrawSB_Cursors_U_Disable(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "U");
}

static void DrawSB_Cursors_U_Upper(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, false);
}

static void DrawSB_Cursors_U_Lower(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, true);
}

static void DrawSB_Cursors_U_Both_Enable(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, true, true);
}

static void DrawSB_Cursors_U_Both_Disable(int x, int y)
{
    DrawMenuCursVoltage(x + 7, y + 5, false, false);
}

static void DrawSB_Cursors_U(int x, int y)
{
    CursCntrl cursCntrl = CURsU_CNTRL;
    if (cursCntrl == CursCntrl_Disable)
    {
        DrawSB_Cursors_U_Disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_IS_U)
        {
            DrawSB_Cursors_U_Both_Disable(x, y);
        }
        else
        {
            Channel source = CURS_SOURCE;
            bool condTop = false, condDown = false;
            CalculateConditions((int16)sCursors_GetCursPosU(source, 0), (int16)sCursors_GetCursPosU(source, 1), cursCntrl, &condTop, &condDown);
            if (condTop && condDown)
            {
                DrawSB_Cursors_U_Both_Enable(x, y);
            }
            else if (condTop)
            {
                DrawSB_Cursors_U_Upper(x, y);
            }
            else
            {
                DrawSB_Cursors_U_Lower(x, y);
            }
        }
    }
}

DEF_SMALL_BUTTON_HINTS_5(   sbSetU,                                                                         //--- ������� - ���������� - ������� U ---
    "������� U", "Cursors U",
    "����� �������� ���������� ��� ��������� � ����������",
    "Choice of cursors of voltage for indication and management",
    mspSet, FuncActive, PressSB_Cursors_U, DrawSB_Cursors_U,
    DrawSB_Cursors_U_Disable,       {"������� ���������� ���������",
                                    "cursors of tension are switched off"},
    DrawSB_Cursors_U_Both_Disable,  {"������� ���������� ��������",
                                    "cursors of tension are switched on"},
    DrawSB_Cursors_U_Upper,         {"������� ���������� ��������, ���������� ������� ��������",
                                    "cursors of tension are switched on, control of the top cursor"},
    DrawSB_Cursors_U_Lower,         {"������� ���������� ��������, ���������� ������ ��������",
                                    "cursors of tension are switched on, control of the lower cursor"},
    DrawSB_Cursors_U_Both_Enable,   {"������� ���������� ��������, ���������� ������ ���������",
                                    "cursors of tension are switched on, control of both cursors"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_Cursors_T()
{
    if (CURS_ACTIVE_IS_T || CURS_CNTRL_T_IS_DISABLE(CURS_SOURCE))
    {
        IncCursCntrlT(CURS_SOURCE);
    }
    CURS_ACTIVE = CursActive_T;
}

static void DrawSB_Cursors_T_Disable(int x, int y)
{
    Painter::DrawText(x + 7, y + 5, "T");
}

static void DrawSB_Cursors_T_Both_Disable(int x, int y)
{
    DrawMenuCursTime(x, y, false, false);
}

static void DrawSB_Cursors_T_Left(int x, int y)
{
    DrawMenuCursTime(x, y, true, false);
}

static void DrawSB_Cursors_T_Right(int x, int y)
{
    DrawMenuCursTime(x, y, false, true);
}

static void DrawSB_Cursors_T_Both_Enable(int x, int y)
{
    DrawMenuCursTime(x, y, true, true);
}

static void DrawSB_Cursors_T(int x, int y)
{
    Channel source = CURS_SOURCE;
    CursCntrl cursCntrl = CURS_CNTRL_T(source);
    if (cursCntrl == CursCntrl_Disable)
    {
        DrawSB_Cursors_T_Disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_IS_T)
        {
            DrawSB_Cursors_T_Both_Disable(x, y);
        }
        else
        {
            bool condLeft = false, condDown = false;
            CalculateConditions((int16)CURS_POS_T0(source), (int16)CURS_POS_T1(source), cursCntrl, &condLeft, &condDown);
            if (condLeft && condDown)
            {
                DrawSB_Cursors_T_Both_Enable(x, y);
            }
            else if (condLeft)
            {
                DrawSB_Cursors_T_Left(x, y);
            }
            else
            {
                DrawSB_Cursors_T_Right(x, y);
            }
        }
    }
}

DEF_SMALL_BUTTON_HINTS_5(   sbSetT,                                                                         //--- ������� - ���������� - ������� T ---
    "������� T", "Cursors T",
    "����� �������� ������� ��� ��������� � ����������",
    "Choice of cursors of time for indication and management",
    mspSet, FuncActive, PressSB_Cursors_T,  DrawSB_Cursors_T,
    DrawSB_Cursors_T_Disable,       {"������� ������� ���������",
                                    "cursors of time are switched off"},
    DrawSB_Cursors_T_Both_Disable,  {"������� ������� ��������",
                                    "cursors of time are switched on"},
    DrawSB_Cursors_T_Left,          {"������� ������� ��������, ���������� ����� ��������",
                                    "cursors of time are switched on, control of the left cursor"},
    DrawSB_Cursors_T_Right,         {"������� ������� ��������, ���������� ������ ��������",
                                    "cursors of time are switched on, control of the right cursor"},
    DrawSB_Cursors_T_Both_Enable,   {"������� ������� ��������, ���������� ������ ���������",
                                    "cursors of time are switched on, control of both cursors"}
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SetCursPos100(Channel chan)
{
    DELTA_U100(chan) = fabsf(CURS_POS_U0(chan) - CURS_POS_U1(chan));
    DELTA_T100(chan) = fabsf(CURS_POS_T0(chan) - CURS_POS_T1(chan));
}

static void PressSB_Cursors_100()
{
    SetCursPos100(CURS_SOURCE);
}

static void DrawSB_Cursors_100(int x, int y)
{
    Painter::SetFont(TypeFont_5);
    Painter::DrawText(x + 2, y + 3, "100%");
    Painter::SetFont(TypeFont_8);
}

DEF_SMALL_BUTTON(   sbSet100,                                                                                    //--- ������� - ���������� - 100% ---
    "100%", "100%",
    "������������ ��� ���������� ���������. ������� �������� ���������� ����� ��������� ��������� ��� 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    mspSet, FuncActive, PressSB_Cursors_100, DrawSB_Cursors_100
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void PressSB_Cursors_PointsPercents()
{
    CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}

static void DrawSB_Cursors_PointsPercents_Percents(int x, int y)
{
    Painter::DrawText(x + 6, y + 5, "\x83");
}

static void DrawSB_Cursors_PointsPercents_Points(int x, int y)
{
    Painter::SetFont(TypeFont_5);
    Painter::DrawText(x + 4, y + 3, "���");
    Painter::SetFont(TypeFont_8);
}

static void DrawSB_Cursors_PointsPercents(int x, int y)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        DrawSB_Cursors_PointsPercents_Percents(x, y);
    }
    else
    {
        DrawSB_Cursors_PointsPercents_Points(x, y);
    }
}

DEF_SMALL_BUTTON_HINTS_2(   sbSetPointsPercents,                                                          //--- ������� - ���������� - ����������� ---
    "�����������", "Movement",
    "����� ���� ����������� �������� - �������� ��� �����",
    "Choice of a step of movement of cursors - percent or points",
    mspSet, FuncActive, PressSB_Cursors_PointsPercents, DrawSB_Cursors_PointsPercents,
    DrawSB_Cursors_PointsPercents_Percents, {"��� ����������� �������� ������ ������ ��������",
                                            "the step of movement of cursors is multiple to one percent"},
    DrawSB_Cursors_PointsPercents_Points,   {"��� ����������� ������� ������ ������ �������",
                                            "the step of movement of the cursor is multiple to one pixel"}
)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnRotate_RegSet_Set(int angle)
{
    if (CURS_ACTIVE_IS_U)
    {
        MoveCursUonPercentsOrPoints(angle);
    }
    else
    {
        MoveCursTonPercentsOrPoints(angle);
    }
    Sound::RegulatorShiftRotate();
}

DEF_PAGE_SB(        mspSet,                                                                                                // ������� - ���������� ///
    "����������", "SET",
    "������� � ����� ��������� ���������",
    "Switch to cursor measures",
    &sbSetExit,                 // ������� - ���������� - �����
    &sbSetSource,               // ������� - ���������� - ��������
    &sbSetU,                    // ������� - ���������� - ������� U
    &sbSetT,                    // ������� - ���������� - ������� T
    &sbSet100,                  // ������� - ���������� - 100%
    &sbSetPointsPercents,       // ������� - ���������� - �����������
    PageSB_Cursors_Set, &pCursors, FuncActive, FuncPress, FuncDrawPage, OnRotate_RegSet_Set
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcShow,                                                                                             //--- ������� - ���������� ---
    "����������", "Shown",
    "��������/��������� �������.",
    "Enable/disable cursors.",
    "���", "No",
    "��",  "Yes",
    CURS_SHOW, pCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       mcTrackingT1U1,                                                                                  //--- ������� - ������� T1,U1 ---
    "�������� \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    "Sets the mode tracking for second cursor:\n"                   // WARN �������
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors.",
    DISABLE_RU,       DISABLE_EN,
    "����������",     "Voltage",
    "�����",          "Time",
    "������ � �����", "Volt and time",
    CURS_LOOKMODE_A, pCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       mcTrackingT2U2,                                                                                  //--- ������� - ������� T2,U2 ---
    "�������� \x8f, \x9f", "Tracking \x8f, \x9f",
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
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors.",
    DISABLE_RU,        DISABLE_EN,
    "����������",      "Voltage",
    "�����",           "Time",
    "������. � �����", "Volt. and time",
    CURS_LOOKMODE_B, pCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//----------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       mcShowFreq,                                                                                               //--- ������� - 1/dT ---
    "1/dT", "1/dT"
    ,
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ "
    "�������� ����� ���������� ����� ���������� ���������."
    ,
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - frequency signal, a period "
    "equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    CURSORS_SHOW_FREQ, pCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pCursors,                                                                                                           // ������� ///
    "�������", "CURSORS",
    "��������� ���������.",
    "Cursor measurements.",
    mcShow,             // ������� - ����������
    mcTrackingT1U1,     // ������� - ������� T1,U1
    mcTrackingT2U2,     // ������� - ������� T2,U2
    mcShowFreq,         // ������� - 1/dT
    mspSet,             // ������� - ����������
    Page_Cursors, &mainPage, FuncActive, EmptyPressPage
)

static void MoveCursUonPercentsOrPoints(int delta)
{
    CursCntrl cursCntrl = CURsU_CNTRL;

    float value = (float)delta;

    Channel source = CURS_SOURCE;

    if(CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= DELTA_U100(source) / 100.0f;
    }

    if(cursCntrl == CursCntrl_1 || cursCntrl == CursCntrl_1_2)
    {
        SetShiftCursPosU(source, 0, value);
    }
    if(cursCntrl == CursCntrl_2 || cursCntrl == CursCntrl_1_2)
    {
        SetShiftCursPosU(source, 1, value);
    }
    Cursors_Update();
}

static void SetShiftCursPosU(Channel chan, int numCur, float delta)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURS_POS_U(chan, numCur) = LimitationRet(CURS_POS_U(chan, numCur) - delta, 0.0f, MAX_POS_U);
    }
    else
    {
        CURS_POS_U(chan, numCur) = LimitationRet(CURS_POS_U(chan, numCur) - delta, 0.0f, MAX_POS_U);
    }
}

static void MoveCursTonPercentsOrPoints(int delta)
{
    Channel source = CURS_SOURCE;
    CursCntrl cursCntrl = CURS_CNTRL_T(source);

    float value = (float)delta;

    if(CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= DELTA_T100(source) / 100.0f;
    }

    if(cursCntrl == CursCntrl_1 || cursCntrl == CursCntrl_1_2)
    {
        SetShiftCursPosT(source, 0, value);
    }
    if(cursCntrl == CursCntrl_2 || cursCntrl == CursCntrl_1_2)
    {
        SetShiftCursPosT(source, 1, value);
    }
    Cursors_Update();
}

static void SetShiftCursPosT(Channel chan, int numCur, float delta)
{
    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        CURS_POS_T(chan, numCur) = LimitationRet(CURS_POS_T(chan, numCur) + delta, 0.0f, MAX_POS_T);
    }
    else
    {
        CURS_POS_T(chan, numCur) = LimitationRet(CURS_POS_T(chan, numCur) + delta, 0.0f, MAX_POS_T);
    }
}


/** @}  @}
 */
