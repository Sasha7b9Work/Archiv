#pragma once
#include "DisplayTypes.h"
#include "Keyboard/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Painter
{
public:

    void BeginScene(Color color);

    void SetColor(Color color);

    void SetPoint(int x, int y);

    void FillRegion(int x, int y, int width, int height);

    void DrawRectangle(int x, int y, int width, int height);

    void DrawVLine(int x, int y0, int y1);

    int DrawText(int x, int y, const char *text);

    void EndScene();

    void DrawHLine(int y, int x0, int x1);

    void SetPalette(uint8 numColor, uint valueColor);

    void SetFont(TypeFont font);

    void DrawLine(int x0, int y0, int x1, int y1);

private:

    int DrawChar(int x, int y, char symbol);
};


extern Painter painter;
