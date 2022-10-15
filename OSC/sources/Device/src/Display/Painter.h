#pragma once
#include "Colors.h"
#include "DisplayTypes.h"
#include "Display/Font/Font.h"
#include "TypeSymbols.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Painter
{
public:
    static void Init();

    static void BeginScene(Color color);

    static void EndScene();
    /// ���� �� ���������� �������� �����, �� ���� ��������� �������
    static void SetColor(Color color = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, Color color = Color::NUMBER);

    static int DrawText(int x, int y, const char *text, Color color = Color::NUMBER);

    static void SetColorValue(Color color, uint value);

    static void DrawRectangle(int x, int y, int width, int height, Color color = Color::NUMBER);

    static int DrawFormatText(int x, int y, Color color, char *format, ...);

    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static void DrawHLine(int y, int x0, int x1, Color color = Color::NUMBER);

    static void DrawHLine(float y, int x0, int x1, Color color = Color::NUMBER);

    static void DrawVLine(int x, int y0, int y1, Color color = Color::NUMBER);

    static void DrawVLine(int x, float y0, float y1, Color color = Color::NUMBER);

    static void DrawLine(int x0, int y0, int x1, int y1, Color color = Color::NUMBER);

    static void SetFont(Font::Type::E typeFont);

    static void SetPoint(int x, int y, Color color = Color::NUMBER);

    static void DrawBoundedRegion(int x, int y, int width, int height, Color colorFill, Color colorBound);

    static void Draw4SymbolsInRect(int x, int y, char eChar, Color color = Color::NUMBER);

    static int DrawStringInCenterRect(int x, int y, int width, int height, const char *text, Color color = Color::NUMBER);

    static int DrawTextWithLimitation(int x, int y, const char *text, int limitX, int limitY, int limitWidth, int limitHeight);

    static void ResetFlash();
    /// ����� ����� � ����������
    static int DrawTextInRectWithTransfers(int x, int y, int width, int height, const char *text);

    static int DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill);
    /// ���������� ������ ���������� ��������������
    static int DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill);
    /// ����� ������ ������ � ������ �������(x, y, width, height)������ ColorText �� �������������� � ������� ������� widthBorder ����� colorBackground
    static void DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                    Color colorBackground);
    static int DrawFormatText(int x, int y, char *format, ...);
    /// ������� ����� �� �������������� ����� colorBackgound
    static int DrawTextOnBackground(int x, int y, const char *text, Color colorBackground);
    /// ����� ������ � ������� x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);
    /// ���������� ������� �������.
    static void SetBrightnessDisplay(int16 brightness);

    static int DrawTextInRectWithTransfersC(int x, int y, int width, int height, const char *text, Color color);

    static void Draw10SymbolsInRect(int x, int y, char eChar);

    static void DrawTextRelativelyRight(int xRight, int y, const char *text, Color color = Color::NUMBER);

    static void DrawBigText(int x, int y, int size, const char *text, Color color = Color::NUMBER);

    static void DrawTextInRect(int x, int y, int width, const char *text);

    static int DrawBigChar(int eX, int eY, int size, char symbol);
    /// ���������� ������� ���� ���������
    static Color GetColor();
    /// ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
    static void DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color);
    /// \brief ���������� numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. �������������� ����������
    /// ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
    static void DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color = Color::NUMBER);
    /// \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta. ������������ ����������
    /// ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
    static void DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color = Color::NUMBER);
    /// ���������� �������������� ����� �� x0 �� x1 ������� ����� ������ delta ��������
    static void DrawHPointLine(int y, int x0, int x1, float delta);
    /// ���������� ������������ ����� �� y0 �� y1 ������� ����� ������ delta ��������
    static void DrawVPointLine(int x, int y0, int y1, float delta);

    static void DrawTesterData(uint8 mode, Color color, uint8 x[240], uint8 y[240]);

private:

    static int DrawCharWithLimitation(int eX, int eY, char symbol, int limitX, int limitY, int limitWidth, int limitHeight);

    static bool ByteFontNotEmpty(uint eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);

    static void OnTimerFlashDisplay();

    static int GetLenghtSubString(const char *text);

    static int DrawSubString(int x, int y, const char *text);

    static int DrawSpaces(int x, int y, const char *text, int *numSymbols);
};
