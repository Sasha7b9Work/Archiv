// 2021/05/12 15:44:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Warnings.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Items/MenuItemsDefs.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cEnable, PageService::PageMath::PageFFT::self,
    "�����������", "Display",
    "�������� � ��������� ����������� �������",
    "Enables or disables the display of the spectrum",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FFT_IS_ENABLED, nullptr, nullptr, nullptr
)


DEF_CHOICE_2(cScale, PageService::PageMath::PageFFT::self,
    "�����", "Scale",
    "����� ������� ������ ������� - �������� ��� ���������������",
    "Sets the scale of the output spectrum - linear or logarithmic",
    "��������", "Log",
    "��������", "Linear",
    FFT_SCALE, nullptr, nullptr, nullptr
)


DEF_CHOICE_3(cSource, PageService::PageMath::PageFFT::self,
    "��������", "Source",
    "����� ��������� ��� ������� �������",
    "Selecting the source for the calculation of the spectrum",
    "����� 1", "Channel 1",
    "����� 2", "Channel 2",
    "����� 1 + 2", "Channel 1 + 2",
    FFT_SOURCE, nullptr, nullptr, nullptr
)


DEF_CHOICE_4(cWindow, PageService::PageMath::PageFFT::self,
    "����", "Window",
    "����� ���� ��� ������� �������",
    "Sets the window to calculate the spectrum",
    "�����������", "Rectangle",
    "��������", "Hamming",
    "��������", "Blackman",
    "�����", "Hann",
    FFT_WINDOW, nullptr, nullptr, nullptr
)


static bool IsActive_Limit()
{
    return FFT_SCALE_IS_LOG;
}


DEF_CHOICE_3(cLimit, PageService::PageMath::PageFFT::self,
    "��������", "Range",
    "����� ����� ������ ������ ���������� �� ��������� �������",
    "Here you can set the limit of monitoring the power spectrum",
    "-40��", "-40dB",
    "-60��", "-60dB",
    "-80��", "-80dB",
    FFT_MAX_DB, IsActive_Limit, nullptr, nullptr
)


static bool IsActive()
{
    return MODE_DRAW_MATH_IS_DISABLED;
}


static void OnPress()
{
    if (!IsActive())
    {
        Warnings::ShowWarningBad("��������� �������������� �������", "DISCONNECT MATHEMATICAL FUNCTION");
    }
}


DEF_PAGE_6(pageFFT, PageService::PageMath::self, NamePage::ServiceMathFFT,
    "������", "SPECTRUM",
    "����������� ������� �������� �������",
    "Mapping the input signal spectrum",
    cEnable,
    cScale,
    cSource,
    cWindow,
    *PageService::PageMath::PageFFT::PageCursors::self,
    cLimit,
    IsActive, OnPress, nullptr, nullptr
)

const Page *PageService::PageMath::PageFFT::self = &pageFFT;
