// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];

    struct S5
    {
        static const char INVINITY_1 = '\xb7';
        static const char INVINITY_2 = '\xb8';
    };

    struct S8
    {
        static const char GOVERNOR_SHIFT_3 = '\x0e';    // ������ ����������
        static const char TRIANGLE_LEFT    = '\x80';    // ����������� �����
        static const char TRIANGLE_RIGHT   = '\x81';    // ����������� ������
        static const char TSHIFT_NORM_2    = '\x84';    // 2-� ����� ������� �������� �� ������� �����������
        static const char TSHIFT_NORM_1    = '\x85';    // 1-� ����� ������� �������� �� ������� �����������
        static const char TSHIFT_LEFT_2    = '\x86';    // 2-� ����� ������� �������� �� ������� ����� �� ������
        static const char TSHIFT_RIGHT_2   = '\x87';    // 2-� ����� ������� �������� �� ������� ������ �� ������
        static const char TPOS_1           = '\x8a';    // ������ TPos � ���� ������
        static const char TPOS_3           = '\x8a';    // 2-� ����� ������� TPos
        static const char TPOS_2           = '\x8b';    // 1-� ����� ������� TPos
        static const char GOVERNOR_SHIFT_2 = '\x8c';    // ������ ����������
        static const char COUPLE_GND       = '\x90';
        static const char COUPLE_AC        = '\x91';
        static const char COUPLE_DC        = '\x92';
        static const char TSHIFT_RIGHT_1   = '\x93';    // 1-� ����� ������� �������� �� ������� ������ �� ������
        static const char RSHIFT_NORMAL    = '\x93';    // ������ �������� �� ���������� ����������
        static const char TSHIFT_LEFT_1    = '\x94';    // 1-� ����� ������� �������� �� ������� ����� �� ������
        static const char TRIG_LEV_NORMAL  = '\x94';    // ������ ������� ������������� ����������
        static const char TRIG_LEV_LOWER   = '\x95';    // ������ ������� ������������� ���� �����
        static const char RSHIFT_LOWER     = '\x95';    // ������ �������� �� ���������� ���� �����
        static const char TRIG_LEV_ABOVE   = '\x96';    // ������ ������� ������������� ���� �����
        static const char RSHIFT_ABOVE     = '\x96';    // ������ �������� �� ���������� ���� �����
        static const char GOVERNOR_SHIFT_0 = '\xaa';    // ������ ����������
        static const char GOVERNOR_SHIFT_1 = '\xac';    // ������ ����������
        static const char PLAY             = '\xae';    // ������ ������ ������ ������ ������
    };

    struct UGO2
    {
        static const char PEAK_DET_1          = '\x12';
        static const char PEAK_DET_2          = '\x13';
        static const char BACKSPACE           = '\x20';
        static const char DEL                 = '\x22';     // ����� ��-�����-�������
        static const char INSERT              = '\x26';     // ����� ��-���-��������
        static const char SAVE_TO_MEM         = '\x2c';
        static const char SETTINGS_MARKER     = '\x60';     // ������ ������� �������������� ���������
        static const char SETTINGS_TUNE       = '\x62';     // ������ ��������� �������������� ���������
        static const char MEM_INT_SHOW_DIRECT = '\x6a';     // ������ �������� ������� ������ � ������ ����� ��
        static const char MEM_INT_SHOW_SAVED  = '\x6c';     // ������ �������� ���������� ������ � ������ ����� ��
        static const char TAB                 = '\x6e';
        static const char FLASH_DRIVE         = '\x80';     // ������ ������ ����� ������
        static const char ETHERNET            = '\x82';     // ������ Ethernet ����� ������
        static const char USB                 = '\x84';     // ������ USB ����� ������
        static const char MATH_FUNC_MUL       = '\x8a';     // ����������-�������-���������
    };
};


struct TypeFont { enum E
{
    S5,
    S8,
    UGO,
    UGO2,
    Count,
    None
};};

struct Font
{
    int height;
    Symbol symbol[256];

    static int GetSize();
    static int GetLengthText(pchar text);
    static int GetHeightSymbol();
    static int GetLengthSymbol(uchar symbol);
    static void Set(TypeFont::E typeFont);

    static const Font *font;
    static const Font *fonts[TypeFont::Count];

    static const uchar font5display[3080];
    static const uchar font8display[3080];
    static const uchar fontUGOdisplay[3080];
    static const uchar fontUGO2display[3080];
};
