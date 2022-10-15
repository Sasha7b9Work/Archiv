#include "Painter.h"
#include "Hardware/Decoder.h"
#include "Hardware/FSMC.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Painter painter;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Color::Chan(Channel ch)
{
    return ch == A ? CHAN_A : CHAN_B;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Color::ChanHalf(Channel ch)
{
    return (ch == A) ? CHAN_A_HALF : CHAN_B_HALF;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(uint8 color)
{
    uint8 buffer[2] = {PAINT_BEGIN_SCENE, color};
    fsmc.WriteToPanel(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    uint8 buffer[1] = {PAINT_END_SCENE};
    fsmc.WriteToPanel(buffer, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(uint8 color)
{
    if (color != 255)
    {
        uint8 buffer[2] = {PAINT_SET_COLOR, color};
        fsmc.WriteToPanel(buffer, 2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, uint8 color)
{
    SetColor(color);
    uint8 buffer[7] = {PAINT_FILL_REGION, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height};
    fsmc.WriteToPanel(buffer, 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawText(int x, int y, const char *text, uint8 color)
{
    #define MAX_SIZE_BUFFER 100
    
    if(strlen(text) + 1 > MAX_SIZE_BUFFER)
    {
        return;
    }
       
    SetColor(color);
    size_t size = (size_t)(1 + 2 + 1 + 1 + strlen(text));
    uint8 buffer[MAX_SIZE_BUFFER] = {PAINT_DRAW_TEXT, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)(size - 5)};

    uint8 *pointer = &buffer[5];

    for (int i = 0; i < strlen(text); i++)
    {
        *pointer++ = (uint8)text[i];
    }
    fsmc.WriteToPanel(buffer, (int)size);

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPalette(uint8 numColor, uint valueColor)
{
    uint8 buffer[6] = {PAINT_SET_PALETTE, numColor, (uint8)valueColor, (uint8)(valueColor >> 8), (uint8)(valueColor >> 16), (uint8)(valueColor >> 24)};

    fsmc.WriteToPanel(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, uint8 color)
{
    SetColor(color);
    uint8 buffer[7] = {PAINT_DRAW_RECTANGLE, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height};
    fsmc.WriteToPanel(buffer, 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawFormatText(int x, int y, uint8 color, char *text, ...)
{
    SetColor(color);
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    CHAR_BUF(buffer, SIZE_BUFFER_DRAW_FORM_TEXT);
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    DrawText(x, y, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawChar(int x, int y, char symbol, uint8 color)
{
    DrawFormatText(x, y, color, "%c", symbol);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, uint8 color)
{
    SetColor(color);
    uint8 buffer[6] = {PAINT_DRAW_HLINE, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8)};
    fsmc.WriteToPanel(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, uint8 color)
{
    SetColor(color);
    uint8 buffer[5] = {PAINT_DRAW_VLINE, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1};
    fsmc.WriteToPanel(buffer, 5);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x0, int y0, int x1, int y1, uint8 color)
{
    SetColor(color);
    uint8 buffer[7] = {PAINT_DRAW_LINE, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1};
    fsmc.WriteToPanel(buffer, 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y, uint8 color)
{
    SetColor(color);
    uint8 buffer[4] = {PAINT_SET_POINT, (uint8)x, (uint8)(x >> 8), (uint8)y};
    fsmc.WriteToPanel(buffer, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(TypeFont typeFont)
{
    uint8 buffer[2] = {PAINT_SET_FONT, (uint8)typeFont};
    fsmc.WriteToPanel(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBoundedRegion(int x, int y, int width, int height, uint8 colorFill, uint8 colorBound)
{
    DrawRectangle(x, y, width, height, colorBound);
    FillRegion(x + 1, y + 1, width - 2, height - 2, colorFill);
}
