#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Grid
{
public:

    // Константы используются для расчёта размеров, которые остаются одинаковыми при любых размерах и пропорциях сетки - например, элементов меню
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;
    static const int LEFT = 19;

    // Эти значения могут меняться в зависимости от выставленного режима
    static int Left();
    static int Top();
    static int Width();
    static int Height();
    static int Bottom();
    static int Right();
    static int NumRows();
    static int NumCols();
    static int MathBottom() { return 0; };
    static int MathHeight() { return 0; };

private:
};
