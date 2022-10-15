#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Menu/MenuControls.h"

 
//----------------------------------------------------------------------------------------------------------------------------------- СИНХР - Режим --
static const char *itemsMode[] = { "Авто",      "Auto",
                                   "Ждущий",    "Wait",
                                   "Однократн", "Once" };

static void OnChanged_Mode()
{
    fpga.LoadTrigMode();
}

static const Choice cMode("Режим", "Mode", itemsMode, &TRIG_MODE, 3, OnChanged_Mode);

//-------------------------------------------------------------------------------------------------------------------------------- СИНХР - Источник --
static const char *itemsSource[] = { "Канал 1", "Channel 1",
                                     "Канал 2", "Channel 2" };

static void OnChanged_Source()
{
    fpga.LoadTrigSource();
}

static const Choice cSource("Источник", "Source", itemsSource, &TRIGSOURCE, 2, OnChanged_Source);

//------------------------------------------------------------------------------------------------------------------------------ СИНХР - Полярность --
static const char *itemsPolarity[] = { "Фронт", "Rising",
                                       "Срез",  "Falling" };

static void OnChanged_Polarity()
{
    fpga.LoadTrigPolarity();
}

static const Choice cPolarity("Полярность", "Polarity", itemsPolarity, &TRIG_POLARITY, 2, OnChanged_Polarity);

//------------------------------------------------------------------------------------------------------------------------------------ СИНХР - Вход --
static const char *itemsInput[] = { "ПС", "Full",
                                    "ВЧ", "HF",
                                    "НЧ", "LF" };

static void OnChanged_Input()
{
    fpga.LoadTrigInput();
}

static const Choice cInput("Вход", "Input", itemsInput, &TRIG_INPUT, 3, OnChanged_Input);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// СИНХР //
static const Control *controlsTrig[] = {&cMode, &cSource, &cPolarity, &cInput};

Page pageTrig("СИНХР", "TRIG", controlsTrig, 4);
