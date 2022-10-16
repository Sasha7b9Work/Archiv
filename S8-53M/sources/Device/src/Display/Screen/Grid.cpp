// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Colors_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


namespace Grid
{
    static int SignalWidth();

    static void Draw(int left, int top, int width, int height);

    static void DrawSpectrum();

    // Если type1 == true - первый тип
    static void DrawType12(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
        float deltaY, float stepX, float stepY, bool type1);

    static void DrawType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX,
        int deltaY, int stepX, int stepY);

    static int CalculateCountV();

    static int CalculateCountH();

    static int FullHeight();

    static int FullCenterHeight();

    static float DeltaY();

    static float DeltaX();
}


int Grid::Bottom()
{
    return Grid::Top() + Height();
}


int Grid::Top()
{
    return 19;
}


int Grid::Delta()
{
    return DELTA;
}


int Grid::Left()
{
    return (Menu::IsMinimize() && MENU_IS_SHOWN ? 9 : 20) + TableAutoMeasures::GetDeltaGridLeft();
}


int Grid::Right()
{
    return ((Menu::IsMinimize() && MENU_IS_SHOWN) ? 9 : 20) + 281;
}


int Grid::Channel::Bottom()
{
    return Top() + Channel::Height();
}


int Grid::SignalWidth()
{
    return Width();
}


int Grid::Channel::Height()
{
    return (IsSeparate()) ? (FullHeight() / 2 + 1) : FullHeight();
}


int Grid::Channel::CenterHeight()
{
    return (Grid::Top() + Channel::Bottom()) / 2;
}


int Grid::FullBottom()
{
    return Grid::Bottom() - TableAutoMeasures::GetDeltaGridBottom();
}


int Grid::FullHeight()
{
    return FullBottom() - Grid::Top();
}


int Grid::Width()
{
    return Right() - Left();
}


int Grid::Height()
{
    return CELLS_IN_HEIGHT * DELTA + 1;
}


int Grid::FullCenterHeight()
{
    return (FullBottom() + Grid::Top()) / 2;
}


float Grid::DeltaY()
{
    float delta = (FullBottom() - Grid::Top()) / 10.0F;
    return IsSeparate() ? (delta / 2.0F) : delta;
}


float Grid::DeltaX()
{
    float delta = (Right() - Left()) / 14.0F;
    return delta;
}


int Grid::Math::Top()
{
    return Math::Bottom() - Math::Height() - 1;
}


int Grid::Math::Height()
{
    if (FFT_IS_ENABLED || MODE_DRAW_MATH_IS_SEPARATE)
    {
        return FullHeight() / 2;
    }

    return FullHeight();
}


int Grid::Math::Bottom()
{
    return FullBottom();
}


void Grid::Draw_Fill()
{
    if (IsSeparate())
    {
        Draw(Left(), Top(), Width(), FullHeight() / 2 + 1);

        if (FFT_IS_ENABLED)
        {
            DrawSpectrum();
        }

        if (MODE_DRAW_MATH_IS_TOGETHER || MODE_DRAW_MATH_IS_SEPARATE)
        {
            Draw(Left(), Top() + FullHeight() / 2, Width(), FullHeight() / 2);
        }
    }
    else
    {
        Draw(Left(), Top(), Width(), FullHeight());
    }
}


void Grid::Draw_Border()
{
    Rectangle(Grid::Width(), Grid::Channel::Height()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    if (FFT_IS_ENABLED)
    {
        Rectangle(257, Grid::Math::Height() + 1).Draw(Grid::Left(), Grid::Math::Top());
    }
    else if (MODE_DRAW_MATH_IS_SEPARATE)
    {
        Rectangle(281, Grid::Math::Height() + 1).Draw(Grid::Left(), Grid::Math::Top());
    }
}


void Grid::Draw(int left, int top, int width, int height)
{
    int right = left + width - 1;
    int bottom = top + height - 1;

    Color::FILL.SetAsCurrent();

    if (top == Grid::Top())
    {
        HLine().Draw(top, 1, left - 2);
        HLine().Draw(top, right + 2, Display::WIDTH - 2);

        HLine().Draw(top + height - 1, 1, left - 2);
        HLine().Draw(top + height - 1, right + 2, Display::WIDTH - 2);

        if (!Menu::IsMinimize() || !MENU_IS_SHOWN)
        {

            VLine().Draw(1, top + 2, bottom - 2);
            VLine().Draw(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() * (float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Color::GRID.SetAsCurrent();

    if (TYPE_GRID_IS_1)
    {
        DrawType12(left, top, right, bottom, (float)(centerX), (float)(centerY), deltaX, deltaY, stepX, stepY, true);
    }
    else if (TYPE_GRID_IS_2)
    {
        DrawType12(left, top, right, bottom, 0, 0, deltaX, deltaY, stepX, stepY, false);
    }
    else if (TYPE_GRID_IS_3)
    {
        DrawType3(left, top, right, bottom, centerX, centerY, (int)(deltaX), (int)(deltaY),
            (int)(stepX), (int)(stepY));
    }
}


void Grid::DrawSpectrum()
{
    if (FFT_SCALE_IS_LOG)
    {
        static const int nums[] = { 4, 6, 8 };
        static pchar strs[] = { "0", "-10", "-20", "-30", "-40", "-50", "-60", "-70" };
        int numParts = nums[FFT_MAX_DB];
        float scale = (float)Grid::Math::Height() / numParts;

        for (int i = 1; i < numParts; i++)
        {
            int y = (int)(Grid::Math::Top() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);

            if (!Menu::IsMinimize())
            {
                Color::FILL.SetAsCurrent();
                Text(strs[i]).Draw(3, y - 4);
            }
        }

        if (!Menu::IsMinimize())
        {
            Color::FILL.SetAsCurrent();
            Text("дБ").Draw(5, Grid::Math::Top() + 1);
        }
    }
    else if (FFT_SCALE_IS_LINEAR)
    {
        static pchar strs[] = { "1.0", "0.8", "0.6", "0.4", "0.2" };
        float scale = (float)Grid::Math::Height() / 5;

        for (int i = 1; i < 5; i++)
        {
            int y = (int)(Grid::Math::Top() + i * scale);
            HLine().Draw(y, Grid::Left(), Grid::Left() + 256, Color::GRID);

            if (!Menu::IsMinimize())
            {
                Text(strs[i]).Draw(5, y - 4, Color::FILL);
            }
        }
    }

    bool needDrawSmallButtons = MENU_SHOWN && (Item::Opened() == PageService::PageMath::PageFFT::PageCursors::self);

    VLine(Grid::Math::Bottom() - Grid::Channel::Bottom() - 4).Draw(318, Grid::Channel::Bottom() + 2, Color::FILL);
    HLine(needDrawSmallButtons ? 7 : 18).Draw(1, Grid::FullBottom() - 1);
    HLine(needDrawSmallButtons ? 52 : 41).Draw(needDrawSmallButtons ? 267 : 278, Grid::FullBottom() - 1);
}


void Grid::DrawType12(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX,
    float deltaY, float stepX, float stepY, bool type1)
{
    {
        uint16 masX[15];
        masX[0] = (uint16)(left + 1);
        masX[14] = (uint16)(right - 1);

        for (int i = 1; i < 14; i++)
        {
            masX[i] = (uint16)(left + deltaX * i);
        }

        MultiVPointLine(15, masX, (int)stepY, CalculateCountV()).Draw((int)(top + stepY));

        if (type1)
        {
            uint16 mas[2] = { (uint16)(centerX - 1), (uint16)(centerX + 1) };
            MultiVPointLine(2, mas, (int)(stepY), CalculateCountV()).Draw((int)(top + stepY));
        }

        uint8 masY[11];
        masY[0] = (uint8)(top + 1);
        masY[10] = (uint8)(bottom - 1);

        for (int i = 1; i < 10; i++)
        {
            masY[i] = (uint8)(top + deltaY * i);
        }

        MultiHPointLine(11, masY, (int)stepX, CalculateCountH()).Draw((int)(left + stepX));

        if (type1)
        {
            uint8 mas[2] = { (uint8)(centerY - 1),  (uint8)(centerY + 1) };
            MultiHPointLine(2, mas, (int)stepX, CalculateCountH()).Draw((int)(left + stepX));
        }
    }
}


void Grid::DrawType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY,
    int stepX, int stepY)
{
    HPointLine((float)(stepX)).Draw(centerY, left + stepX, right);

    uint8 masY[6] = { (uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1),
        (uint8)(bottom - 2), (uint8)(bottom - 1) };

    MultiHPointLine(6, masY, deltaX, (right - top) / deltaX).Draw(left + deltaX);

    VPointLine((float)(stepY)).Draw(centerX, top + stepY, bottom);

    uint16 masX[6] = { (uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1),
        (uint16)(right - 2), (uint16)(right - 1) };

    MultiVPointLine(6, masX, deltaY, (bottom - top) / deltaY).Draw(top + deltaY);
}


int Grid::CalculateCountV()
{
    if(MEASURES_MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        bool source_is_AB = MEASURES_SOURCE_IS_B;

        if (MEASURES_NUMBER_IS_1_5)
        {
            return source_is_AB ? 42 : 44;
        }
        else if (MEASURES_NUMBER_IS_2_5)
        {
            return source_is_AB ? 69 : 39;
        }
        else if (MEASURES_NUMBER_IS_3_5)
        {
            return source_is_AB ? 54 : 68;
        }
    }

    return 49;
}


int Grid::CalculateCountH()
{
    if (MEASURES_MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (MEASURES_NUMBER_IS_6_1)
        {
            return 73;
        }
        if (MEASURES_NUMBER_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}


bool Grid::IsSeparate()
{
    return (!MODE_DRAW_MATH_IS_DISABLED && MODE_DRAW_MATH_IS_SEPARATE) || FFT_IS_ENABLED;
}
