#include "FPGA/FPGA.h"
#include "Menu/MenuControls.h"
#include "Settings/Settings.h"
#include "Utils/Dictionary.h"
#include "Log.h"


//----------------------------------------------------------------------------------------------------------------------------------  ¿Õ¿À 1 - ¬ıÓ‰ --
static const char *itemsInputA[] = { DISABLE_RU, DISABLE_EN,
                                     ENABLE_RU,  ENABLE_EN };

static void OnChanged_InputA()
{

}

static const Choice cEnableA("¬ıÓ‰", "Input", itemsInputA, &SET_ENABLED(A), 2, OnChanged_InputA);

//---------------------------------------------------------------------------------------------------------------------------------  ¿Õ¿À 1 - —‚ˇÁ¸ --
static const char *itemsCoupleA[] = { "AC",  "AC",
                                      "DC",  "DC",
                                      "GND", "GND" };

static void OnChanged_CoupleA()
{
    fpga.LoadRanges();
}

static const Choice cCoupleA("—‚ˇÁ¸", "Couple", itemsCoupleA, &SET_COUPLE_A, 3, OnChanged_CoupleA);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  ¿Õ¿À 1 //
const Control* controlsA[] = {&cEnableA, &cCoupleA};

Page pageChannelA(" ¿Õ¿À 1", "CHANNEL 1", controlsA, 2);


//----------------------------------------------------------------------------------------------------------------------------------  ¿Õ¿À 2 - ¬ıÓ‰ --
static const char *itemsInputB[] = { DISABLE_RU, DISABLE_EN,
                                     ENABLE_RU,  ENABLE_EN };

static void OnChanged_InputB()
{

}

static const Choice cEnableB("¬ıÓ‰", "Input", itemsInputB, &SET_ENABLED(B), 2, OnChanged_InputB);

//---------------------------------------------------------------------------------------------------------------------------------  ¿Õ¿À 2 - —‚ˇÁ¸ --
static const char *itemsCoupleB[] = { "AC",  "AC",
                                      "DC",  "DC",
                                      "GND", "GND" };

static void OnChanged_CoupleB()
{
    fpga.LoadRanges();
}

static const Choice cCoupleB("—‚ˇÁ¸", "Couple", itemsCoupleB, &SET_COUPLE_B, 3, OnChanged_CoupleB);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  ¿Õ¿À 2//
static const Control* controlsB[] = {&cEnableB, &cCoupleB};

Page pageChannelB(" ¿Õ¿À 2", "CHANNEL 2", controlsB, 2);
