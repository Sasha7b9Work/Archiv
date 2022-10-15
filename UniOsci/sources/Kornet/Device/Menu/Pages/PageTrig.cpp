#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Menu/MenuControls.h"

 
//----------------------------------------------------------------------------------------------------------------------------------- ����� - ����� --
static const char *itemsMode[] = { "����",      "Auto",
                                   "������",    "Wait",
                                   "���������", "Once" };

static void OnChanged_Mode()
{
    fpga.LoadTrigMode();
}

static const Choice cMode("�����", "Mode", itemsMode, &TRIG_MODE, 3, OnChanged_Mode);

//-------------------------------------------------------------------------------------------------------------------------------- ����� - �������� --
static const char *itemsSource[] = { "����� 1", "Channel 1",
                                     "����� 2", "Channel 2" };

static void OnChanged_Source()
{
    fpga.LoadTrigSource();
}

static const Choice cSource("��������", "Source", itemsSource, &TRIGSOURCE, 2, OnChanged_Source);

//------------------------------------------------------------------------------------------------------------------------------ ����� - ���������� --
static const char *itemsPolarity[] = { "�����", "Rising",
                                       "����",  "Falling" };

static void OnChanged_Polarity()
{
    fpga.LoadTrigPolarity();
}

static const Choice cPolarity("����������", "Polarity", itemsPolarity, &TRIG_POLARITY, 2, OnChanged_Polarity);

//------------------------------------------------------------------------------------------------------------------------------------ ����� - ���� --
static const char *itemsInput[] = { "��", "Full",
                                    "��", "HF",
                                    "��", "LF" };

static void OnChanged_Input()
{
    fpga.LoadTrigInput();
}

static const Choice cInput("����", "Input", itemsInput, &TRIG_INPUT, 3, OnChanged_Input);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ����� //
static const Control *controlsTrig[] = {&cMode, &cSource, &cPolarity, &cInput};

Page pageTrig("�����", "TRIG", controlsTrig, 4);
