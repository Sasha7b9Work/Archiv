// 2021/05/12 15:57:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"


void PageService::PageCalibrator::SetCalibratorMode(CalibratorMode::E mode)
{
    CALIBRATOR_MODE = mode;

    FPGA::LoadRegUPR();
}


static void OnChanged_Mode(bool)
{
    PageService::PageCalibrator::SetCalibratorMode(CALIBRATOR_MODE);
}


DEF_CHOICE_3(cMode, PageService::PageCalibrator::self,
    "����������", "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "����", "AC",
    "0�", "OV",
    CALIBRATOR_MODE, nullptr, OnChanged_Mode, nullptr
)


static void OnPress_Calibrate()
{
    Calibrator::PerformCalibration();
}


DEF_BUTTON(cCalibrate, PageService::PageCalibrator::self,
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    nullptr, OnPress_Calibrate
)


DEF_PAGE_2(pageCalibrator, PageService::self, NamePage::ServiceCalibrator,
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Item of the calibrator and calibration of an oscillograph",
    cMode,
    cCalibrate,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageService::PageCalibrator::self = &pageCalibrator;
