// 2021/05/12 15:29:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_SMALL_BUTTON(sbExit, PageService::PageMath::PageFunction::self,
    "Выход", "Exit", "Кнопка для выхода в предыдущее меню", "Button for return to the previous menu",
    nullptr, nullptr, DrawSB_Exit, nullptr
)


static void OnPress_ModeDraw()
{
    if (FFT_IS_ENABLED)
    {
        Warnings::ShowWarningBad("ОТКЛЮЧИТЕ ВЫЧИСЛЕНИЕ БПФ", "DISCONNECT CALCULATION OF FFT");
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&MODE_DRAW_MATH, 0, 2);
    }
}


static void Draw_ModeDraw_Disable(int x, int y)
{
    Text(LANG_RU ? "Вык" : "Dis").Draw(x + (LANG_RU ? 2 : 0), y + 5);
}


static void Draw_ModeDraw_Separate(int x, int y)
{
    Rectangle(14, 10).Draw(x + 3, y + 5);

    HLine line(13);

    line.Draw(x + 3, y + 9);
    line.Draw(x + 3, y + 10);
}


static void Draw_ModeDraw_Together(int x, int y)
{
    Rectangle(14, 10).Draw(x + 3, y + 5);
}


static void Draw_ModeDraw(int x, int y)
{
    static const pFuncVII funcs[3] =
    {
        Draw_ModeDraw_Disable,
        Draw_ModeDraw_Separate,
        Draw_ModeDraw_Together
    };
    funcs[MODE_DRAW_MATH](x, y);
}


static const arrayHints hintsModeDraw =
{
    {Draw_ModeDraw_Disable,  "Вывод математической функции отключён",
                                            "The conclusion of mathematical function is disconnected"},
    {Draw_ModeDraw_Separate, "Сигналы и математическая функция выводятся в разных окнах",
                                            "Signals and mathematical function are removed in different windows"},
    {Draw_ModeDraw_Together, "Сигналы и математическая функция выводятся в одном окне",
                                            "Signals and mathematical function are removed in one window"}
};


DEF_SMALL_BUTTON(sbModeDraw, PageService::PageMath::PageFunction::self,
    "Экран", "Display",
    "Выбирает режим отображения математического сигнала",
    "Chooses the mode of display of a mathematical signal",
    nullptr, OnPress_ModeDraw, Draw_ModeDraw, &hintsModeDraw
)


static void OnPress_Type()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}


static void Draw_Type_Sum(int x, int y)
{
    HLine().Draw(y + 9, x + 4, x + 14);
    VLine().Draw(x + 9, y + 4, y + 14);
}


static void Draw_Type_Mul(int x, int y)
{
    Char(Symbol::UGO2::MATH_FUNC_MUL).Draw4SymbolsInRect(x + 4, y + 3, TypeFont::UGO2);
}


static void Draw_Type(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_Type_Sum, Draw_Type_Mul };
    funcs[MATH_FUNC](x, y);
}


static const arrayHints hintsType =
{
    { Draw_Type_Sum,      "Сложение",     "Addition"       },
    { Draw_Type_Mul,      "Умножение",    "Multiplication" }
};


DEF_SMALL_BUTTON(sbType, PageService::PageMath::PageFunction::self,
    "Вид", "Type",
    "Выбор математической функции",
    "Choice of mathematical function",
    nullptr, OnPress_Type, Draw_Type, &hintsType
)


static void OnPress_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}


static void Draw_ModeRegSet_Range(int x, int y)
{
    Char(LANG_RU ? 'M' : 'S').Draw(x + 7, y + 5);
}


static void Draw_ModeRegSet_RShift(int x, int y)
{
    Text(LANG_RU ? "См" : "Shif").Draw(x + 5 - (LANG_EN ? 3 : 0), y + 5);
}


static void Draw_Math_Function_ModeRegSet(int x, int y)
{
    const pFuncVII funcs[2] = { Draw_ModeRegSet_Range, Draw_ModeRegSet_RShift };
    funcs[MATH_MODE_REG_SET](x, y);
}


static const arrayHints hintsModeRegSet =
{
    {Draw_ModeRegSet_Range,  "Управление масштабом", "Management of scale"},
    {Draw_ModeRegSet_RShift, "Управление смещением", "Management of shift"}
};


DEF_SMALL_BUTTON(sbModeRegSet, PageService::PageMath::PageFunction::self,
    "Режим ручки УСТАНОВКА", "Mode regulator SET",
    "Выбор режима ручки УСТАНОВКА - управление масштабом или смещением",
    "Choice mode regulcator УСТАНОВКА - management of scale or shift",
    nullptr, OnPress_ModeRegSet, Draw_Math_Function_ModeRegSet, &hintsModeRegSet
)


static void OnPress_RangeA()
{
    MATH_RANGE = RANGE_A;
    MATH_MULTIPLIER = DIVIDER_A;
}


static void Draw_RangeA(int x, int y)
{
    Char('1').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbRangeA, PageService::PageMath::PageFunction::self,
    "Масштаб 1-го канала", "Scale of the 1st channel",
    "Берёт масштаб для математического сигнала из первого канала",
    "Takes scale for a mathematical signal from the first channel",
    nullptr, OnPress_RangeA, Draw_RangeA, nullptr
)


static void OnPress_RangeB()
{
    MATH_RANGE = RANGE_B;
    MATH_MULTIPLIER = DIVIDER_B;
}


static void Draw_RangeB(int x, int y)
{
    Char('2').Draw(x + 8, y + 5);
}


DEF_SMALL_BUTTON(sbRangeB, PageService::PageMath::PageFunction::self,
    "Масштаб 2-го канала", "Scale of the 2nd channel",
    "Берёт масштаб для математического сигнала из второго канала",
    "Takes scale for a mathematical signal from the second channel",
    nullptr, OnPress_RangeB, Draw_RangeB, nullptr
)


static void OnPress()
{
    if (FFT_IS_ENABLED)
    {
        Warnings::ShowWarningBad("ОТКЛЮЧИТЕ ВЫЧИСЛЕНИЕ БПФ", "DISCONNECT CALCULATION OF FFT");
    }
}


static void OnRegSet(int delta)
{
    if (MODE_DRAW_MATH_IS_DISABLED)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        int16 prevRShift = RSHIFT_MATH;
        int16 rShift = prevRShift;

        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * RShift::STEP();
                Math::LimitAbove(&rShift, RShift::MAX);

                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }

                Sound::RegulatorShiftRotate();
                RSHIFT_MATH = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * RShift::STEP();
                Math::LimitBelow(&rShift, RShift::MIN);

                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }

                Sound::RegulatorShiftRotate();
                RSHIFT_MATH = rShift;
            }
        }
    }

    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = RShift::ToAbs(RSHIFT_MATH, MATH_RANGE);

        if (sum > 2)
        {
            if (MATH_RANGE < Range::Count - 1)
            {
                ++MATH_RANGE;
                RSHIFT_MATH = (int16)RShift::ToRel(rShiftAbs, MATH_RANGE);
                Holder::RotateSwitch(delta);
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (MATH_RANGE > 0)
            {
                --MATH_RANGE;
                RSHIFT_MATH = (int16)RShift::ToRel(rShiftAbs, MATH_RANGE);
                Holder::RotateSwitch(delta);
            }
            sum = 0;
        }
    }
}


static bool IsActive()
{
    return !FFT_IS_ENABLED;
}


DEF_PAGE_SB(pageMathFunction, PageService::PageMath::self, NamePage::ServiceMathFunction_SB,
    "ФУНКЦИЯ", "FUNCTION",
    "Установка и выбор математической функции - сложения или умножения",
    "Installation and selection of mathematical functions - addition or multiplication",
    sbExit,
    sbModeDraw,
    sbType,
    sbModeRegSet,
    sbRangeA,
    sbRangeB,
    IsActive, OnPress, nullptr, OnRegSet
);


const Page *PageService::PageMath::PageFunction::self = &pageMathFunction;
