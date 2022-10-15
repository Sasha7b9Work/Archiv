#include "Menu/MenuControls.h"
#include "Settings/Settings.h"
#include "Tester/Tester.h"


//------------------------------------------------------------------------------------------------------------------- ТЕСТЕР-КОМПОНЕНТ - Полярность --
static const char *iPolarity[] = { "+", "+",
                                   "-", "-" };

static void OnChanged_Polarity()
{
    tester.LoadPolarity();
}

static const Choice cPolarity("Полярность", "Polarity", iPolarity, &TESTER_POLARITY, 2, OnChanged_Polarity);


//------------------------------------------------------------------------------------------------------------------------- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
static const char *iStepU[] = { "100мВ", "100mV",
                                "500мВ", "500mV" };

static const char *iStepI[] = { "4мА",  "4mA",
                                "20мА", "20mA" };

static void OnChanged_Step()
{
    tester.LoadStep();
}

static Choice cStep("Шаг", "Step", iStepU, &TESTER_STEP_U, 2, OnChanged_Step, iStepI, &TESTER_STEP_I, 2);


//------------------------------------------------------------------------------------------------------------------- ТЕСТЕР-КОМПОНЕНТ - Управление --
static const char *iControl[] = { "Напряжение", "Voltage",
                                  "Ток",        "Current"};

static void OnChanged_Control()
{
    cStep.SetAlternateMode(TESTER_CONTROL_IS_U ? 0 : 1);
}

static const Choice cControl("Управление", "Control", iControl, &TESTER_CONTROL, 2, OnChanged_Control);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static const char *iType[] = {"Линии", "Lines",
"Точки", "Points"};

static const Choice cType("Рисование", "Paint", iType, &VIEW_MODE, 2);

//----------------------------------------------------------------------------------------------------------------------------------------------------
static const char *iSmooth[] = {"1", "1", "2", "2", "3", "3", "4", "4", "5", "5"};
static const Choice cSmooth("Сглаживание", "Smoothing", iSmooth, &TESTER_NUM_SMOOTH, 5);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ТЕСТЕР-КОМПОНЕНТ //
static const Control *controlsTester[] = {&cControl, &cPolarity, &cStep, &cType, &cSmooth};

Page pageTester(0, 0, controlsTester, 5);
