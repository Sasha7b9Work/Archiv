#include "defines.h"
#include "Painter.h"
#include "Display.h"
#include "font/Font.h"
#include "Keyboard/Controls.h"
#include "Keyboard/Keyboard.h"
#include <string.h>
#include <stdarg.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Painter painter;

static Color color;

uint clut[256] =
{
    0x00000000,
    0x00ffffff,
    0x000000ff,
    0x0000ff00,
    0x00afafaf,
    0x000000ff,
    0x0000ff00,
    0x00ff0000
};

uint timeStart = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color col)
{
    timeStart = HAL_GetTick();

    color = col;

    memset(display.GetBuffer(), (uint8)color, 320 * 240);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color col)
{
    color = col;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    if (x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT)
    {
        *(display.GetBuffer() + y * Display::WIDTH + x) = (uint8)color;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    display.ToggleBuffers();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1)
{
    uint8 *address = display.GetBuffer() + y * Display::WIDTH + x0;

    memset(address, (uint8)color, x1 - x0 + 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1)
{
    if (y0 > y1)
    {
        int temp = y0;
        y0 = y1;
        y1 = temp;
    };

    uint8 *address = display.GetBuffer() + x + y0 * Display::WIDTH;

    for (int i = 0; i <= y1 - y0; i++)
    {
        *address = (uint8)color;
        address += Display::WIDTH;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height)
{
    for (int j = y; j < y + height + 1; j++)
    {
        DrawHLine(j, x, x + width);
    }

    /*
    extern LTDC_LayerCfgTypeDef pLayerCfg;

    DMA2D_HandleTypeDef hDMA2D;
    hDMA2D.Init.Mode = DMA2D_R2M;
    hDMA2D.Init.ColorMode = DMA2D_ARGB8888;
    hDMA2D.Init.OutputOffset = 0;
    hDMA2D.Instance = DMA2D;

    hDMA2D.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[0].InputAlpha = 0xff;
    hDMA2D.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
    hDMA2D.LayerCfg[0].InputOffset = 0;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 0) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, 4, (uint)display.GetBuffer() + 320 * 10, 10, 10) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height)
{
    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawText(int x, int y, const char *text)
{
    int numSymbols = strlen(text);
    for (int i = 0; i < numSymbols; ++i)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }
    
    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte]; //-V108
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawChar(int eX, int eY, char symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    int size = 1;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPalette(uint8 numColor, uint valueColor)
{
    clut[numColor] = valueColor;
    if (HAL_LTDC_ConfigCLUT(&hltdc, clut, 256, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    if (HAL_LTDC_EnableCLUT(&hltdc, 0) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(TypeFont typeFont)
{
    Font_SetFont(typeFont);
}

static int Sign(int x)
{
    if (x > 0)
    {
        return -1;
    }
    else if (x < 0)
    {
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x1, int y1, int x2, int y2)
{
    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)fabsf((float)(x1 - x2));
    int dy = (int)fabsf((float)(y1 - y2));
    int s1 = Sign(x1 - x2);
    int s2 = Sign(y1 - y2);
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        SetPoint(x, y);
        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}
