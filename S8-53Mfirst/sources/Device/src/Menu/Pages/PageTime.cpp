// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Display/Warnings.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"


void PageTime::SetPeakDetMode(PeakDetMode::E mode)
{
    MODE_PEAK_DET = mode;

    FPGA::LoadRegUPR();
}


static bool IsActive_Sample()
{
    return TBASE_IS_RAND;
}

DEF_CHOICE_2(mcSample, PageTime::self,
    "�������", "Sampling",
    "\"��������\" - \n"
    "\"�������������\" -"
    ,
    "\"Real\" - \n"
    "\"Equals\" - "
    ,
    "�������� �����", "Real",
    "�������������", "Equals",
    SAMPLE_TYPE, IsActive_Sample, nullptr, nullptr
)


static void WriteRShiftADC()
{
    if (PEAK_DET_IS_ENABLED)
    {
        //            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, 3, false);     // ������-�� ��� ������� ��������� �������� ����������. ��� ��� � ������������.
        //            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, 3, false);
    }
    else
    {
//        volatile int8 shift[2][3] =
//        {
//            {0, set_.chan[Chan::A].balance_shift_ADC, (int8)setNRST.balance_ADC[ChA]},
//            {0, set_.chan[Chan::B].balance_shift_ADC, (int8)setNRST.balance_ADC[ChB]}
//        };

        //            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC1, (uint8)shift[0][BALANCE_ADC_TYPE], false);
        //            FPGA::WriteToHardware(WR_ADD_RSHIFT_DAC2, (uint8)shift[1][BALANCE_ADC_TYPE], false);
    }
}


static bool IsActive_PeakDet()
{
    return PEAK_DET_IS_ALLOWED;
}


void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        PageTime::SetPeakDetMode(MODE_PEAK_DET);
        TBase::Set(TBASE);

        WriteRShiftADC();

        if (PEAK_DET_IS_ENABLED)
        {
            int centerX = SHIFT_IN_MEMORY_IN_POINTS + Grid::Width() / 2;

            SHIFT_IN_MEMORY_IN_POINTS = Math::Limitation<int16>((int16)(centerX / 2 - Grid::Width() / 2),
                0,
                (int16)(EnumPointsFPGA::PointsInChannel() - Grid::Width()));

            PageMemory::OnChanged_NumPoints(true);
        }
        else
        {
            int centerX = SHIFT_IN_MEMORY_IN_POINTS + Grid::Width() / 2;
            SHIFT_IN_MEMORY_IN_POINTS = (int16)(centerX * 2 - Grid::Width() / 2);
            ENUM_POINTS_FPGA = set.time.old_num_points;
            PageMemory::OnChanged_NumPoints(true);
        }
    }
    else
    {
        Warnings::ShowWarningBad("���. ���. �� �������� �� ���������� ����� 0.5���/���",
                                 "PEAK. DET. NOT WORK ON SWEETS LESS THAN 0.5us/div");
    }
}

DEF_CHOICE_2(mcPeakDet, PageTime::self,
    "��� ���", "Pic deat",
    "��������/��������� ������� ��������.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    MODE_PEAK_DET, IsActive_PeakDet, PageTime::OnChanged_PeakDet, nullptr
)

void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_NumPoints(active);
    TShift::Set(TSHIFT);
}

DEF_CHOICE_3(mcTPos, PageTime::self,
    "\x7b", "\x7b",
    "����� ����� �������� �������� �������� �� ������� � ������ - ����� ����, �����, ������ ����.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "����", "Left",
    "�����", "Center",
    "�����", "Right",
    TPOS, nullptr, PageTime::OnChanged_TPos, nullptr
)

static bool IsActive_SelfRecorder()
{
    return TBASE_IS_P2P;
}

DEF_CHOICE_2(mcSelfRecorder, PageTime::self,
    "���������", "Self-Recorder",
    "��������/��������� ����� ���������. ���� ����� �������� �� ��������� 20��/��� � ����� ���������.",
    "Turn on/off the recorder. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SELF_RECORDER, IsActive_SelfRecorder, nullptr, nullptr
)

DEF_CHOICE_2(mcDivRole, PageTime::self,
    "�-��� ��/���", "Func Time/DIV"
    ,
    "����� ������� ��� ����� �����/���: � ������ ����� ���������� (����/���� � ��������� ����):\n"
    "1. \"�����\" - ��������� �������� �� �������.\n"
    "2. \"������\" - ����������� �� ������."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "�����", "Time",
    "������", "Memory",
    FUNCTION_REG_TIME, nullptr, nullptr, nullptr
)

DEF_PAGE_5(pageTime, PageMain::self, NamePage::Time,
    "���������", "SCAN",
    "�������� ��������� ��������.",
    "Contains scan settings.",
    mcSample,
    mcPeakDet,
    mcTPos,
    mcSelfRecorder,
    mcDivRole,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageTime::self = &pageTime;


bool PageTime::InSelfRecoredMode()
{
    return SELF_RECORDER;
}
