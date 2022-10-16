// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Chan;


// ��� �����
struct Color
{
    static const Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color DATA_A;
    static Color DATA_B;
    static Color MENU_FIELD;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;       // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ���
                                        // �������� ������� ������.
    static Color MENU_TITLE_BRIGHT;     // ���� ����� ������� ����, ��� ���� ��������� ��������. ������������ ���
                                        // �������� ������� ������.
    static Color MENU_ITEM;
    static Color MENU_ITEM_DARK;        // ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� ��������
                                        // ������� ������.
    static Color MENU_ITEM_BRIGHT;      // ���� ����� ������� ����, ��� ���� �������� ����. ������������ ��� ��������
                                        // ������� ������
    static Color MENU_SHADOW;
    static Color EMPTY;
    static Color EMPTY_A;
    static Color EMPTY_B;

    static const uint8 Count = 16;

    static Color FILL;
    static Color BACK;

    static Color BLACK_WHITE_0;
    static Color BLACK_WHITE_1;

    uint8 index;        // ����� �������� ������ �����, ���� �� ����� �������

    uint8 index1;       // \ ����� ��������� ������ � ������ ����� � ������ ��������� �����.
    uint8 index2;       // / � "������" � ���� ������ �������� �������� ������ Count.

    Color(uint8 i) : index(i), index1(0), index2(0) {}

    Color(const Color &c1, const Color &c2) : index(Count + 1), index1(c1.index), index2(c2.index) {}

    static void ResetFlash();

    void SetAsCurrent() const;

    static Color Cursors(const Chan &ch);

    // ���������� ������� ���� (�� ������ ������������ ���������)
	static Color Get() { return Color(current); };

    // ���������� ����, ������� ����� ����������� ���������
    uint8 ValueForDraw() const;

    Color GetInverse() const;

    static Color Trig();

    // ���� ��������� ����.
    static Color BorderMenu()       { return Color::MENU_TITLE; }

    // ������� ���� � ����.
    static Color LightShadingText() { return BorderMenu(); }

    // ���������� ����, ����������� � color. ����� ���� ����� ��� ������.
    static Color Contrast(const Color &color);

    static uint Make(uint8 r, uint8 g, uint8 b);

private:

    static Color current;

    static uint timeResetFlash; // �����, � ������� "���������" ��������. ����� ��� ����, ����� ���������� ��
                                // "���������" ��� ������������ �������� ���������
};


struct ColorType
{
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    uint8   color;
    int8    currentField;
    bool    alreadyUsed;

    void Init();
    void BrightnessChange(int delta);
    void SetBrightness(float brightness);
    void ComponentChange(int delta);

private:
    void SetBrightness();
    void Set();
    void CalcSteps();
};


#define R_FROM_COLOR(color) (((uint)(color) >> 16) & (uint)0xFF)
#define G_FROM_COLOR(color) (((uint)(color) >> 8) & (uint)0xFF)
#define B_FROM_COLOR(color) ((uint)(color) & 0xFF)
#define COLOR(i) set.display.colors[i]
