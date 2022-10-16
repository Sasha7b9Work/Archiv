// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Utils/Math_.h"
#include "Data/ProcessingData.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


static void SetCursPosU(const Chan &ch, int numCur, float pos);         // Установить позицию курсора напряжения.
static void SetCursPosT(const Chan &ch, int numCur, float pos);         // Установить значение курсора по времени.


#define MAX_POS_U   200
#define MAX_POS_T   280

void Cursors_Update()
{
    CursLookMode::E look_mode_0 = CURS_LOOK_MODE_0;
    CursLookMode::E look_mode_1 = CURS_LOOK_MODE_1;

    if((look_mode_0 == CursLookMode::Voltage || look_mode_0 == CursLookMode::Time) && CURS_ACTIVE_IS_T)
    {
        float posU0 = Processing::GetCursU(CURS_SOURCE, CURS_T_POS(CURS_SOURCE, 0));
        SetCursPosU(CURS_SOURCE, 0, posU0);
    }
    if((look_mode_1 == CursLookMode::Voltage || look_mode_1 == CursLookMode::Both) && CURS_ACTIVE_IS_T)
    {
        float posU1 = Processing::GetCursU(CURS_SOURCE, CURS_T_POS(CURS_SOURCE, 1));
        SetCursPosU(CURS_SOURCE, 1, posU1);
    }
    if((look_mode_0 == CursLookMode::Time || look_mode_0 == CursLookMode::Both) && CURS_ACTIVE_IS_U)
    {
        float posU0 = CURS_U_POS(CURS_SOURCE, 0);
        float posT0 = Processing::GetCursT(CURS_SOURCE, posU0, 0);
        SetCursPosT(CURS_SOURCE, 0, posT0);
    }
    if((look_mode_1 == CursLookMode::Time || look_mode_1 == CursLookMode::Both) && CURS_ACTIVE_IS_U)
    {
        float posU1 = CURS_U_POS(CURS_SOURCE, 1);
        float posT1 = Processing::GetCursT(CURS_SOURCE, posU1, 1);
        SetCursPosT(CURS_SOURCE, 1, posT1);
    }
}


void SetCursPosU(const Chan &ch, int num_cur, float pos)
{
    CURS_U_POS(ch, num_cur) = Math::Limitation(pos, 0.0f, (float)MAX_POS_U);
}



void SetCursPosT(const Chan &ch, int num_cur, float pos)
{
    CURS_T_POS(ch, num_cur) = Math::Limitation(pos, 0.0f, (float)MAX_POS_T);
}


DEF_CHOICE_2(mcShow, PageCursors::self,
    "Показывать", "Shown",
    "Включает/отключает курсоры.",
    "Enable/disable cursors.",
    "Нет", "No",
    "Да", "Yes",
    CURS_SHOW, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(mcTrackingT1U1, PageCursors::self,
    "Слежение \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "Задаёт режим слежения за первым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают "
    "изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают "
    "изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор "
    "производилось последнее воздействие."
    ,
    "Sets the mode tracking for second cursor:\n"                   // WARN Перевод
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track "
    "changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes "
    "in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "Напряжение", "Voltage",
    "Время", "Time",
    "Напряж и время", "Volt and time",
    CURS_LOOK_MODE_0, nullptr, nullptr, nullptr
)

DEF_CHOICE_4(mcTrackingT2U2, PageCursors::self,
    "Слежение \x8f, \x9f", "Tracking \x8f, \x9f"
    ,
    "Задаёт режим слежения за вторым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают "
    "изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают "
    "изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор "
    "производилось последнее воздействие."
    ,
    "Sets the mode tracking cursors channel 2:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track "
    "changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes "
    "in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "Напряжение", "Voltage",
    "Время", "Time",
    "Напряж. и время", "Volt. and time",
    CURS_LOOK_MODE_1, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcShowFreq, PageCursors::self,
    "1/dT", "1/dT",
    "Если выбрано \"Вкл\", в правом верхнем углу выводится величина, обратная расстоянию между курсорами времени - "
    "частота сигнала, один период которого равен расстоянию между временными курсорами.",
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - "
    "frequency signal, a period equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    CURS_SHOW_FREQ, nullptr, nullptr, nullptr
)


DEF_PAGE_5(pageCursors, PageMain::self, NamePage::Cursors,
    "КУРСОРЫ", "CURSORS",
    "Курсорные измерения.",
    "Cursor measurements.",
    mcShow,
    mcTrackingT1U1,
    mcTrackingT2U2,
    mcShowFreq,
    *PageCursors::PageSet::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageCursors::self = &pageCursors;


static int CalculateYforCurs(int y, bool top)
{
    return top ? y + Item::HEIGHT / 2 + 4 : y + Item::HEIGHT - 2;
}


static int CalculateXforCurs(int x, bool left)
{
    return left ? x + Item::WIDTH - 20 : x + Item::WIDTH - 6;
}


void PageCursors::CalculateXY(int *x0, int *x1, int *y0, int *y1)
{
    *x0 = CalculateXforCurs(*x0, true);
    *x1 = CalculateXforCurs(*x1, false);
    *y0 = CalculateYforCurs(*y0, true);
    *y1 = CalculateYforCurs(*y1, false);
}


void PageCursors::DrawMenuCursorsVoltage(int x, int y, bool top, bool bottom)
{
    x -= 65;
    y -= 21;
    int x0 = x, x1 = x, y0 = y, y1 = y;

    CalculateXY(&x0, &x1, &y0, &y1);

    for (int i = 0; i < (top ? 3 : 1); i++)
    {
        HLine().Draw(y0 + i, x0, x1);
    }

    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
        HLine().Draw(y1 - i, x0, x1);
    }
}


void PageCursors::DrawMenuCursorsTime(int x, int y, bool left, bool right)
{
    x -= 58;
    y -= 16;
    int x0 = x, x1 = x, y0 = y, y1 = y;

    CalculateXY(&x0, &x1, &y0, &y1);

    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        VLine().Draw(x0 + i, y0 - 1, y1);
    }

    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        VLine().Draw(x1 - i, y0 - 1, y1);
    }
}
