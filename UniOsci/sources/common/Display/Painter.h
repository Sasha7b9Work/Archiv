#pragma once
#include "Display/Colors.h"
#include "DisplayTypes.h"
#include "ffconf.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
    /// \brief ���������� ������� ���������� ����� ��������� � ���� �������, �� ��������� ����������� ��������� ������� EndScene(). 
    /// ����� ��������, ���� ������� ��������� ����� ��������� ������ ������ ������ �������. ��������, ����� �������������� ����� �������� �� ������ � 
    /// ������ ����������.
    static void RunDisplay();
    /// ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    static void EndScene();
    /// ������� ����������� �� ������� ���������� ����� USB ��� LAN. ���� first == true, �� ���������� �����
    static void SendFrame(bool first);
    /// ����� ������� �������. ����� ��� ����, ����� �������� ������ ��� ����������� �� �������� � ������
    static void ResetFlash();
    /// ���������� ���� ���������
    static void SetColor(Color color);
    /// ���������� ������� ���� ���������
    static Color GetColor();

    static void LoadPalette();

    static void SetPalette(Color color);

    static void SetPoint(int x, int y);

    static void DrawHPointLine(int y, int x0, int x1, float delta);

    static void DrawVPointLine(int x, int y0, int y1, float delta);
    /// \brief ���������� numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
    /// ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
    static void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);
    /// \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. ������������ ����������
    /// ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
    static void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);
    /// ���������� �������������� �����
    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);
    /// ���������� ������������ �����
    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);
    /// \brief ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������ ���������� �� ������. 
    /// dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    /// ������ ����������� ������������ �����.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    static void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade);
    /// ���������� ������� �������.
    static void SetBrightnessDisplay(int16 brightness);

    static uint16 ReduceBrightness(uint16 colorValue, float newBrightness);
    /// ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
    static void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// modeLines - true - �������, false - �������.
    static void DrawSignal(int x, uint8 data[281], bool modeLines);

    static void DrawPicture(int x, int y, int width, int height, uint8 *address);

    static bool SaveScreenToFlashDrive();

    static void SendToDisplay(uint8 *bytes, int numBytes);

    static void SendToInterfaces(uint8 *pointer, int size);

    static void SetFont(TypeFont typeFont);
    ///  ��������� ������ � �������
    static void LoadFont(TypeFont typeFont);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);
    /// ������� ����� �� �������������� ����� colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);

    static int DrawFormatText(int x, int y, char *format, ...);
    /// ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, char *text, ...);

    static int DrawTextWithLimitationC(int x, int y, const char *text, Color color, int limitX, int limitY, int limitWidth, int limitHeight);
    /// ���������� ������ ���������� ��������������
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);
    /// ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, 
                                             Color colorBackground);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);

    static void DrawHintsForSmallButton(int x, int y, int width, void *smallButton);

    static void DrawTextInRect(int x, int y, int width, char *text);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    static void Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2);

    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static void Draw10SymbolsInRect(int x, int y, char eChar);
    /// ����� ����� � ����������
    static int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    static void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);

private:

    static void CalculateCurrentColor();
};


#define WRITE_BYTE(offset, value)   *(command + offset) = (uint8)value
#define WRITE_SHORT(offset, value)  *((uint16 *)(command + offset)) = (uint16)value

/** @} @}
 */
