// 2021/05/12 17:22:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include <cmath>


#define MAX_POS_U   200
#define MAX_POS_T   280


static void PressSB_Exit()
{
}


DEF_SMALL_BUTTON(sbExit, PageCursors::PageSet::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, PressSB_Exit, DrawSB_Exit, nullptr
)


static void SetSource(const Chan &ch)
{
    CURS_SOURCE = ch.value;
}


static void PressSB_Source()
{
    SetSource(CURS_SOURCE == ChA ? ChB : ChA);
}


static void DrawSB_SourceA(int x, int y)
{
    Text("1").Draw(x + 7, y + 5);
}


static void DrawSB_SourceB(int x, int y)
{
    Text("2").Draw(x + 7, y + 5);
}


static void DrawSB_Source(int x, int y)
{
    (CURS_SOURCE == ChA) ? DrawSB_SourceA(x, y) : DrawSB_SourceB(x, y);
}


static const arrayHints hintsSetSource =
{
    { DrawSB_SourceA, "канал 1", "channel 1" }, { DrawSB_SourceB, "канал 2", "channel 2" }
};


DEF_SMALL_BUTTON(sbSource, PageCursors::PageSet::self,
    "Источник", "Source",
    "Выбор канала для курсорных измерений",
    "Channel choice for measurements",
    nullptr, PressSB_Source, DrawSB_Source, &hintsSetSource
)


static void IncCursCntrlU(const Chan &ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURS_U_CONTROL(ch), 0, 3);
}


static void PressSB_U()
{
    if (CURS_ACTIVE_IS_U || CURS_U_CONTROL_IS_DISABLED(CURS_SOURCE))
    {
        IncCursCntrlU(CURS_SOURCE);
    }

    CURS_ACTIVE = CursActive::U;
}


static void DrawSB_U_Disable(int x, int y)
{
    Text("U").Draw(x + 7, y + 5);
}


static void DrawSB_U_Both_Disable(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, false, false);
}


static void DrawSB_U_Both_Enable(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, true, true);
}


static void DrawSB_U_Upper(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, true, false);
}


static void DrawSB_U_Lower(int x, int y)
{
    PageCursors::DrawMenuCursorsVoltage(x + 7, y + 5, false, true);
}


static void DrawSB_Cursors_U(int x, int y)
{
    CursCntrl::E cursCntrl = CURS_U_CONTROL(CURS_SOURCE);

    if (cursCntrl == CursCntrl::Disabled)
    {
        DrawSB_U_Disable(x, y);
    }
    else
    {
        if (!CURS_ACTIVE_IS_U)
        {
            DrawSB_U_Both_Disable(x, y);
        }
        else
        {
            bool condTop = false, condDown = false;

            CalculateConditions((int16)(Cursors::GetPosU(CURS_SOURCE, 0)),
                (int16)(Cursors::GetPosU(CURS_SOURCE, 1)), cursCntrl, &condTop, &condDown);

            if (condTop && condDown)
            {
                DrawSB_U_Both_Enable(x, y);
            }
            else if (condTop)
            {
                DrawSB_U_Upper(x, y);
            }
            else
            {
                DrawSB_U_Lower(x, y);
            }
        }
    }
}


static const arrayHints hintsSetU =
{
    { DrawSB_U_Disable,     "курсоры напряжения выключены",
                                    "cursors of tension are switched off" },
    { DrawSB_U_Both_Disable,"курсоры напряжения включены",
                                    "cursors of tension are switched on" },
    { DrawSB_U_Upper,       "курсоры напряжения включены, управление верхним курсором",
                                    "cursors of tension are switched on, control of the top cursor" },
    { DrawSB_U_Lower,       "курсоры напряжения включены, управление нижним курсором",
                                    "cursors of tension are switched on, control of the lower cursor" },
    { DrawSB_U_Both_Enable, "курсоры напряжения включены, управление обоими курсорами",
                                    "cursors of tension are switched on, control of both cursors" }
};


DEF_SMALL_BUTTON(sbtU, PageCursors::PageSet::self,
    "Курсоры U", "Cursors U",
    "Выбор курсоров напряжения для индикации и управления",
    "Choice of cursors of voltage for indication and management",
    nullptr, PressSB_U, DrawSB_Cursors_U, &hintsSetU
)


void IncCursCntrlT(const Chan &ch)
{
    Math::CircleIncrease<int8>((int8 *)&CURS_T_CONTROL(ch), 0, 3);
}


static void PressSB_Cursors_T()
{
    if (CURS_ACTIVE_IS_T || CURS_T_CONTROL_IS_DISABLED(CURS_SOURCE))
    {
        IncCursCntrlT(CURS_SOURCE);
    }

    CURS_ACTIVE = CursActive::T;
}


static void DrawSB_Cursors_T_Disable(int x, int y)
{
    Text("T").Draw(x + 7, y + 5);
}


static void DrawSB_Cursors_T_Both_Disable(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, false, false);
}


static void DrawSB_Cursors_T_Both_Enable(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, true, true);
}


static void DrawSB_Cursors_T_Left(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, true, false);
}


static void DrawSB_Cursors_T_Right(int x, int y)
{
    PageCursors::DrawMenuCursorsTime(x, y, false, true);
}


static void DrawSB_Cursors_T(int x, int y)
{
    CursCntrl::E curs_cntrl = CURS_T_CONTROL(CURS_SOURCE);

    if (curs_cntrl == CursCntrl::Disabled)
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
            bool cond_left = false;
            bool cond_down = false;

            CalculateConditions((int16)(CURS_T_POS(CURS_SOURCE, 0)),
                (int16)(CURS_T_POS(CURS_SOURCE, 1)), curs_cntrl, &cond_left, &cond_down);

            if (cond_left && cond_down)
            {
                DrawSB_Cursors_T_Both_Enable(x, y);
            }
            else if (cond_left)
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


static const arrayHints hintsSetT =
{
    { DrawSB_Cursors_T_Disable,         "курсоры времени выключены",
                                        "cursors of time are switched off" },
    { DrawSB_Cursors_T_Both_Disable,    "курсоры времени включены",
                                        "cursors of time are switched on" },
    { DrawSB_Cursors_T_Left,            "курсоры времени включены, управление левым курсором",
                                        "cursors of time are switched on, control of the left cursor" },
    { DrawSB_Cursors_T_Right,           "курсоры времени включены, управление правым курсором",
                                        "cursors of time are switched on, control of the right cursor" },
    { DrawSB_Cursors_T_Both_Enable,     "курсоры времени включены, управление обоими курсорами",
                                        "cursors of time are switched on, control of both cursors" }
};


DEF_SMALL_BUTTON(sbT, PageCursors::PageSet::self,
    "Курсоры T", "Cursors T",
    "Выбор курсоров времени для индикации и управления",
    "Choice of cursors of time for indication and management",
    nullptr, PressSB_Cursors_T, DrawSB_Cursors_T, &hintsSetT
)


static void SetCursPos100(const Chan &ch)
{
    CURS_100_PERCENTS_DU(ch) = std::fabsf(CURS_U_POS(ch, 0) - CURS_U_POS(ch, 1));
    CURS_100_PERCENTS_DT(ch) = std::fabsf(CURS_T_POS(ch, 0) - CURS_T_POS(ch, 1));
}


static void PressSB_Cursors_100()
{
    SetCursPos100(CURS_SOURCE);
}


static void DrawSB_Cursors_100(int x, int y)
{
    Text("100%%%%").Draw(x + 2, y + 3, TypeFont::S5);           // \todo Костыль. Внутри конструктора съедаются лишние % в vsprintf
}


DEF_SMALL_BUTTON(sb100, PageCursors::PageSet::self,
    "100%%", "100%%",
    "Используется для процентных измерений. Нажатие помечает расстояние между активными курсорами как 100%",
    "It is used for percentage measurements. Pressing marks distance between active cursors as 100%",
    nullptr, PressSB_Cursors_100, DrawSB_Cursors_100, nullptr
)


static void PressSB_Cursors_PointsPercents()
{
    Math::CircleIncrease<int8>((int8 *)&CURS_MOVEMENT, 0, 1);
}


static void DrawSB_Cursors_PointsPercents_Percents(int x, int y)
{
    Text("\x83").Draw(x + 6, y + 5);
}


static void DrawSB_Cursors_PointsPercents_Points(int x, int y)
{
    Text("тчк").Draw(x + 4, y + 3, TypeFont::S5);
}


static void DrawSB_Cursors_PointsPercents(int x, int y)
{
    if(CURS_MOVEMENT_IS_PERCENTS)
    {
        DrawSB_Cursors_PointsPercents_Percents(x, y);
    }
    else
    {
        DrawSB_Cursors_PointsPercents_Points(x, y);
    }
}


static const arrayHints hintsSetPointsPercents =
{
    { DrawSB_Cursors_PointsPercents_Percents,   "шаг перемещения курсоров кратен одному проценту",
                                                "the step of movement of cursors is multiple to one percent" },
    { DrawSB_Cursors_PointsPercents_Points,     "шаг перемещения курсора кратен одному пикселю",
                                                "the step of movement of the cursor is multiple to one pixel" }
};


DEF_SMALL_BUTTON(sbPointsPercents, PageCursors::PageSet::self,
    "Перемещение", "Movement",
    "Выбор шага перемещения курсоров - проценты или точки",
    "Choice of a step of movement of cursors - percent or points",
    nullptr, PressSB_Cursors_PointsPercents, DrawSB_Cursors_PointsPercents, &hintsSetPointsPercents
)


static void SetShiftCursPosU(const Chan &ch, int numCur, float delta)
{
    CURS_U_POS(ch, numCur) = Math::Limitation<float>(CURS_U_POS(ch, numCur) - delta, 0, MAX_POS_U);
}


static void MoveCursUonPercentsOrPoints(int delta)
{
    CursCntrl::E curs_cntrl = CURS_U_CONTROL(CURS_SOURCE);

    float value = (float)delta;

    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= CURS_100_PERCENTS_DU(CURS_SOURCE) / 100.0F;
    }

    if (curs_cntrl == CursCntrl::_1 || curs_cntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosU(CURS_SOURCE, 0, value);
    }

    if (curs_cntrl == CursCntrl::_2 || curs_cntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosU(CURS_SOURCE, 1, value);
    }

    Cursors_Update();
}


static void SetShiftCursPosT(const Chan &ch, int num_cur, float delta)
{
    CURS_T_POS(ch, num_cur) = Math::Limitation<float>(CURS_T_POS(ch, num_cur) + delta, 0, MAX_POS_T);
}


static void MoveCursTonPercentsOrPoints(int delta)
{
    CursCntrl::E curs_cntrl = CURS_T_CONTROL(CURS_SOURCE);

    float value = (float)(delta);

    if (CURS_MOVEMENT_IS_PERCENTS)
    {
        value *= CURS_100_PERCENTS_DT(CURS_SOURCE) / 100.0F;
    }

    if(curs_cntrl == CursCntrl::_1 || curs_cntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosT(CURS_SOURCE, 0, value);
    }

    if (curs_cntrl == CursCntrl::_2 || curs_cntrl == CursCntrl::_1_2)
    {
        SetShiftCursPosT(CURS_SOURCE, 1, value);
    }

    Cursors_Update();
}


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


DEF_PAGE_SB(pageSet, PageCursors::self, NamePage::CursorsSet_SB,
    "УСТАНОВИТЬ", "SET",
    "Переход в режим курсорных измерений",
    "Switch to cursor measures",
    sbExit,
    sbSource,
    sbtU,
    sbT,
    sb100,
    sbPointsPercents,
    nullptr, nullptr, nullptr, OnRotate_RegSet_Set
)

const Page *PageCursors::PageSet::self = &pageSet;
