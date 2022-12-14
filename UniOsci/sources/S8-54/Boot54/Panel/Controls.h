#pragma once


// ?????????????? ??????.

typedef enum
{
    B_Empty     = 0x00,     // ?????? ?? ??????
    B_ChannelA  = 0x01,     // ????? 1
    B_Service   = 0x02,     // ??????
    B_ChannelB  = 0x03,     // ????? 2
    B_Display   = 0x04,     // ???????
    B_Time      = 0x05,     // ????
    B_Memory    = 0x06,     // ??????
    B_Trig      = 0x07,     // ?????
    B_Start     = 0x08,     // ????/?????
    B_Cursors   = 0x09,     // ???????
    B_Measures  = 0x0a,     // ?????
    B_Power     = 0x0b,     // ???????
    B_Help      = 0x0c,     // ??????
    B_Menu      = 0x0d,     // ????
    B_F1        = 0x0e,     // 1
    B_F2        = 0x0f,     // 2
    B_F3        = 0x10,     // 3
    B_F4        = 0x11,     // 4
    B_F5        = 0x12,     // 5
    B_NumButtons            // ????? ?????????? ??????
} PanelButton;

// ?????????????? ???????????.
typedef enum
{                           //               ??????? - ??????????
    R_Empty         = 0x00,
    R_RangeA        = 0x14,     // ?????/??? ???.A  0x9c - 0x1c
    R_RShiftA       = 0x15,     //                  0x9d - 0x1d
    R_RangeB        = 0x16,     // ?????/??? ???.B  0x9e - 0x1e
    R_RShiftB       = 0x17,     //                  0x9f - 0x1f
    R_TBase         = 0x18,     // ?????/???        0xa0 - 0x20
    R_TShift        = 0x19,     //                  0xa1 - 0x21
    R_TrigLev       = 0x1a,     // ???????          0xa2 - 0x22
    R_Set           = 0x1b,     // ?????????        0xa3 - 0x23
    R_Range1_BTN    = 0x1c,     // ??????? ????????? ???????? ?? ??????????
    R_RShiftA_BTN   = 0x1d,     // ????????? ????
    R_RangeB_BTN    = 0x1e,     // ??????? ????????? ???????? ?? ?????????
    R_RShiftB_BTN   = 0x1f,     // ????????? ????
    R_TBase_BTN     = 0x20,     // ????????? ?????? ????????
    R_TShift_BTN    = 0x21,     // ????????? ????
    R_TrigLev_BTN   = 0x22,     // ????????? ???? ????????????? ??? ?????????????? ?????????????
    R_Set_BTN       = 0x23
} PanelRegulator;

// ??????????? ???????? ???????????.
typedef enum
{
    Direction_Left,     // ???????? ?????????? ?????
    Direction_Right,    // ???????? ?????????? ??????
    Direction_None      // ???????? ???????????
} RegulatorDirection;


#define NUM_REGULATORS (R_Set - R_RangeA + 1)


const char *NameButton(PanelButton button);
const char *NameRegulator(PanelRegulator regulator);
