#include "FPGA/FPGA.h"
#include "Menu/MenuControls.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"
#include "Log.h"


//---------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ���� --
static const char *itemsInputA[] = { DISABLE_RU, DISABLE_EN,
                                     ENABLE_RU,  ENABLE_EN };

static void OnChanged_InputA()
{

}

static const Choice cEnableA("����", "Input", itemsInputA, &SET_ENABLED(A), 2, OnChanged_InputA);

//--------------------------------------------------------------------------------------------------------------------------------- ����� 1 - ����� --
static const char *itemsCoupleA[] = { "AC",  "AC",
                                      "DC",  "DC",
                                      "GND", "GND" };

static void OnChanged_CoupleA()
{
    fpga.LoadRanges();
}

static const Choice cCoupleA("�����", "Couple", itemsCoupleA, &SET_COUPLE_A, 3, OnChanged_CoupleA);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 1 //
const Control* controlsA[] = {&cEnableA, &cCoupleA};

Page pageChannelA("����� 1", "CHANNEL 1", controlsA, 2);


//---------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ���� --
static const char *itemsInputB[] = { DISABLE_RU, DISABLE_EN,
                                     ENABLE_RU,  ENABLE_EN };

static void OnChanged_InputB()
{

}

static const Choice cEnableB("����", "Input", itemsInputB, &SET_ENABLED(B), 2, OnChanged_InputB);

//--------------------------------------------------------------------------------------------------------------------------------- ����� 2 - ����� --
static const char *itemsCoupleB[] = { "AC",  "AC",
                                      "DC",  "DC",
                                      "GND", "GND" };

static void OnChanged_CoupleB()
{
    fpga.LoadRanges();
}

static const Choice cCoupleB("�����", "Couple", itemsCoupleB, &SET_COUPLE_B, 3, OnChanged_CoupleB);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� 2//
static const Control* controlsB[] = {&cEnableB, &cCoupleB};

Page pageChannelB("����� 2", "CHANNEL 2", controlsB, 2);
