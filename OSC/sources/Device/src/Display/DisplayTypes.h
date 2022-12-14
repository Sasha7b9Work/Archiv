#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOSI_HEIGHT             9
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH_D_IP          34      /* ?????????? ?????? ????????? ip-?????? ? ?????? ??????? ????? */
#define WIDTH_SB                19
#define GRID_DELTA              20  /* ?????????? ????? ? ?????? */


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    } value;
    DrawMode(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
};
