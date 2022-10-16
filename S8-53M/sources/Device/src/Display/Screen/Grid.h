// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Grid
{
    static const int DELTA = 20;
    static const int WIDTH = 280;
    static const int CELLS_IN_HEIGHT = 10;

    // Отрисовать заполнение
    void Draw_Fill();

    // Отрисовать окантовку
    void Draw_Border();

    int Delta();
    int Left();
    int Right();
    int Width();
    int Height();
    int Top();
    int Bottom();

    int FullBottom();

    // Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    bool IsSeparate();

    namespace Channel
    {
        int Bottom();
        int Height();
        int CenterHeight();
    };

    namespace Math
    {
        int Height();
        int Bottom();
        int Top();
    };
};
