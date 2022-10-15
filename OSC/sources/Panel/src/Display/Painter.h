#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** @addtogroup Display
 *  @{
 *  @defgroup Painter
 *  @brief ������� ���������
 *  @{
 */

class Painter
{
public:
    /// ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    static void BeginScene(Color color);
    /// ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    static void EndScene();

    static void SetColor(Color color);
    /// ���������� ������� ���� ���������
    static Color GetColor();
    /// ������������� ����� �������� ��������. ����������� � ������� LoadPalette() ��� SetPalette()
    static void SetColorValue(Color color, uint value);
    /// ��������� � ������� ��� �����
    static void LoadPalette();

    static void DrawTesterData(uint8 mode, Color color, uint8 x[240], uint8 y[240]);
    /// ���������� ���� ���������
    /// ���������� ����� ������� ������
    static void SetPoint(int x, int y);
    /// ���������� �������������� ����� �� x0 �� x1 ������� ����� ������ delta ��������
    static void DrawHPointLine(int y, int x0, int x1, float delta);
    /// ���������� ������������ ����� �� y0 �� y1 ������� ����� ������ delta ��������
    static void DrawVPointLine(int x, int y0, int y1, float delta);
    /// ���������� �������������� �����
    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    /// ���������� ������������ �����
    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);
    /// ���������� ������������ �����
    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);
    /// \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    /// dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// ������ ����������� ������������ �����.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static void SetFont(Font::Type typeFont);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);
    /// ������� ����� �� �������������� ����� colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    static int DrawFormatText(int x, int y, char *format, ...);
    /// ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

private:

    static Color currentColor;

    static Font::Type::E currentTypeFont;

    static bool ByteFontNotEmpty(int eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);
};


/** @} @}
 */
