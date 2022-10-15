#pragma once
#include "defines.h"
#include "Display/DisplayTypes.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct Font
{
    struct Type
    {
        enum E
        {
            _5,
            _8,
            _UGO,
            _UGO2,
            Number,
            None
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    int height;
    Symbol symbol[256];

    static int  GetSize();
    static int  GetLengthText(const char *text);
    static int  GetHeightSymbol(char symbol);
    static int  GetLengthSymbol(char symbol);
    static void SetFont(Font::Type typeFont);
};


extern const Font *font;
extern const Font *fonts[Font::Type::Number];

extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
