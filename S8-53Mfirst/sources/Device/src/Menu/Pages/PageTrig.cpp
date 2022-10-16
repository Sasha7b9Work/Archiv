// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"


void PageTrig::OnPress_Mode(bool)
{
    FPGA::Stop();

    if (!START_MODE_IS_SINGLE)
    {
        FPGA::Start();
    }
}

DEF_CHOICE_3(mcMode, PageTrig::self,
    "�����", "Mode"
    ,
    "����� ����� �������:\n"
    "1. \"����\" - ������ ���������� �������������.\n"
    "2. \"������\" - ������ ���������� �� ������ �������������.\n"
    "3. \"�����������\" - ������ ���������� �� ���������� ������ ������������� ���� ���. ��� ���������� ��������� �����"
    " ������ ������ ����/����."
    ,
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to"
    " press the START/STOP."
    ,
    "���� ", "Auto",
    "������", "Wait",
    "�����������", "Single",
    START_MODE, nullptr, PageTrig::OnPress_Mode, nullptr
)

static void OnChanged_Source(bool)
{
    TrigSource::Set(TRIG_SOURCE);
}

DEF_CHOICE_3(mcSource, PageTrig::self,
    "��������", "Source",
    "����� ��������� ������� �������������.",
    "Synchronization signal source choice.",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "�������", "External",
    TRIG_SOURCE, nullptr, OnChanged_Source, nullptr
)

static void OnChanged_Polarity(bool)
{
    TrigPolarity::Load();
}

DEF_CHOICE_2(mcPolarity, PageTrig::self,
    "����������", "Polarity"
    ,
    "1. \"�����\" - ������ ���������� �� ������ ��������������.\n"
    "2. \"����\" - ������ ���������� �� ����� ��������������."
    ,
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut."
    ,
    "�����", "Front",
    "����", "Back",
    TRIG_POLARITY, nullptr, OnChanged_Polarity, nullptr
)

static void OnChanged_Input(bool)
{
    TrigInput::Set(TRIG_INPUT);
}

DEF_CHOICE_4(mcInput, PageTrig::self,
    "����", "Input"
    ,
    "����� ����� � ���������� �������������:\n"
    "1. \"��\" - ������ ������.\n"
    "2. \"��\" - �������� ����.\n"
    "3. \"���\" - ������ ������ ������.\n"
    "4. \"���\" - ������ ������� ������."
    ,
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"AS\" - a gated entrance.\n"
    "3. \"LPF\" - low-pass filter.\n"
    "4. \"HPF\" - high-pass filter frequency."
    ,
    "��", "Full",
    "��", "AC",
    "���", "LPF",
    "���", "HPF",
    TRIG_INPUT, nullptr, OnChanged_Input, nullptr
)

DEF_CHOICE_2(mcAutoFind_Mode, PageTrig::PageAutoFind::self,
    "�����", "Mode"
    ,
    "����� ������ ��������������� ������ �������������:\n"
    "1. \"������\" - ����� ������������ �� ������� ������ \"�����\" ��� �� ��������� � ������� 0.5� ������ �����, ����"
    " ����������� \"������\x99��� ���� �����\x99�����������\".\n"
    "2. \"��������������\" - ����� ������������ �������������."
    ,
    "Selecting the automatic search of synchronization:\n"
    "1. \"Hand\" - search is run on pressing of the key \"Find\" or on deduction during 0.5s the ����� key if it"
    " is established \"SERVICE\x99Mode long �����\x99\x41utolevel\".\n"
    "2. \"Auto\" - the search is automatically."
    ,
    "������", "Hand",
    "��������������", "Auto",
    TRIG_MODE_FIND, nullptr, nullptr, nullptr
)

static bool IsActive_AutoFind_Search()
{
    return TRIG_MODE_FIND_IS_HAND;
}

static void OnPress_AutoFind_Search()
{
    TrigLev::FindAndSet();
}

DEF_BUTTON(mbAutoFind_Search, PageTrig::PageAutoFind::self,
    "�����", "Search",
    "���������� ����� ������ �������������.",
    "Runs for search synchronization level.",
    IsActive_AutoFind_Search, OnPress_AutoFind_Search
)

DEF_PAGE_2(pageAutoFind, PageTrig::self, NamePage::TrigAuto,
    "�����", "SEARCH",
    "���������� �������������� ������� ������ �������������.",
    "Office of the automatic search the trigger level.",
    mcAutoFind_Mode,
    mbAutoFind_Search,
    nullptr, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageTrig, PageMain::self, NamePage::Trig,
    "�����", "TRIG",
    "�������� ��������� �������������.",
    "Contains synchronization settings.",
    mcMode,
    mcSource,
    mcPolarity,
    mcInput,
    *PageTrig::PageAutoFind::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageTrig::self = &pageTrig;
const Page *PageTrig::PageAutoFind::self = &pageAutoFind;
