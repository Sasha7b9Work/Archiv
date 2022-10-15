#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WIDTH_SCREEN    320
#define HEIGHT_SCREEN   240
#define HEIGHT_TITLE    19
#define WIDTH_ITEM      69
#define HEIGHT_ITEM     ((HEIGHT_SCREEN - HEIGHT_TITLE - 1) / 4)
#define HEIGHT_SIGNAL   ((HEIGHT_SCREEN - HEIGHT_TITLE) / 2)
#define WIDTH_SIGNAL    ((WIDTH_SCREEN - WIDTH_ITEM) - 1)

typedef enum
{
    TypeFont_5,
    TypeFont_8,
    TypeFont_UGO,
    TypeFont_UGO2,
    TypeFont_Number,
    TypeFont_None
} TypeFont;


typedef enum
{
    EMPTY_FUCTION,
    SET_BRIGHTNESS,         // 1
    SET_ORIENTATION,        // 2 ���������� ���������� ������. 0 - ����������, 1 - ��������
    SET_PALETTE_COLOR,      // 3 numColor:1 colorValue:2
    SET_COLOR,              // 4 numColor:1
    SET_FONT,               // 5 numFont:1
    DRAW_TEXT,              // 6
    DRAW_PIXEL,             // 7 x:2 y:1
    DRAW_LINE,              // 8 ���������� �����
    FILL_REGION,            // 9 x:2 y:1 width:2 height:1
    DRAW_MULTI_HPOINT_LINE, // 10 numLines:1 x:2 numPoints:1 dx:1 lines:numLines
    DRAW_MULTI_VPOINT_LINE, // 11 numLines:1 y:1 numPoints:1 dy:1 empty:1 lines:numLines*2
    DRAW_SIGNAL_LINES,      // 12 x:2 points:281
    DRAW_SIGNAL_POINTS,     // 13 x:2 points:281
    DRAW_VLINES_ARRAY,      // 14 x0:2 numLines:1 lines:numLines * 2
    INVALIDATE,             // 15
    RUN_BUFFER,             // 16
    GET_PIXEL,              // 17
    LOAD_IMAGE,             // 18
    DRAW_HLINE,             // 19 y:1 x0:2 x1:2
    DRAW_VLINE,             // 20 x:2 y0:1 y1:1
    SET_REINIT,             // 21

    LOAD_FONT,              // typeFont:1 height:4 symbols:9 * 256 . �������� symbols ������ � Font.h
    NUM_COMM
} Command;
