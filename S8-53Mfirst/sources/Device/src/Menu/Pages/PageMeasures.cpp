// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Utils/Math_.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Text_.h"
#include "Menu/Items/MenuItems.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


bool PageMeasures::choiceMeasuresIsActive = false;

// ����� �������� � ������ ������ ���������� ����������
static CursCntrl::E GetMeasuresCursCntrlActive()
{
    return MEASURES_CURS_ACTIVE_IS_T ? MEASURES_CURS_CNTRL_T : MEASURES_CURS_CNTRL_U;
}

void DrawSB_MeasTune_Settings(int x, int y)
{
    Char(Symbol::UGO2::SETTINGS_TUNE).Draw4SymbolsInRect(x + 2, y + 1, TypeFont::UGO2);
}

void PressSB_MeasTune_Settings()
{
    TableAutoMeasures::Handler::ButtonSettings();
}

void DrawSB_Markers(int x, int y)
{
    Char(Symbol::UGO2::SETTINGS_MARKER).Draw4SymbolsInRect(x + 2, y + 2, TypeFont::UGO2);
}

void PressSB_Markers()
{
    TableAutoMeasures::Handler::OnButonMarker();
}

void RotateRegMeasureSetField(int angle)
{
    CursCntrl::E curs_cntrl = GetMeasuresCursCntrlActive();

    int sign = Math::Sign(angle);

    int bytes_in_channel = EnumPointsFPGA::BytesInChannel();

    if (curs_cntrl == CursCntrl::_1 || curs_cntrl == CursCntrl::_1_2)
    {
        if (MEASURES_CURS_ACTIVE_IS_T)
        {
            MEASURES_POS_CUR_T_0 = Math::Limitation<int16>((int16)(MEASURES_POS_CUR_T_0 - sign),
                0,
                (int16)(bytes_in_channel - 1));
        }
        else
        {
            MEASURES_POS_CUR_U_0 = Math::Limitation<int16>((int16)(MEASURES_POS_CUR_U_0 + sign), 0, 200);
        }
    }

    if (curs_cntrl == CursCntrl::_2 || curs_cntrl == CursCntrl::_1_2)
    {
        if (MEASURES_CURS_ACTIVE_IS_T)
        {
            MEASURES_POS_CUR_T_1 = Math::Limitation<int16>((int16)(MEASURES_POS_CUR_T_1 - sign),
                0,
                (int16)(bytes_in_channel - 1));
        }
        else
        {
            MEASURES_POS_CUR_U_1 = Math::Limitation<int16>((int16)(MEASURES_POS_CUR_U_1 + sign), 0, 200);
        }
    }
}

DEF_SMALL_BUTTON( sbSettings, PageMeasures::PageTune::self,
    "���������", "Setup",
    "��������� ������� ����������� ���������",
    "Allows to choose necessary measurements",
    nullptr, PressSB_MeasTune_Settings, DrawSB_MeasTune_Settings, nullptr
)

                                                                 // ��������� / ���������� ������� ��� ������ ���������.
DEF_SMALL_BUTTON( sbMarkers, PageMeasures::PageTune::self,
    "������", "Marker",
    "��������� ���������� ������� ��� ����������� �������� ���������",
    "Allows to establish markers for visual control of measurements",
    nullptr, PressSB_Markers, DrawSB_Markers, nullptr
)

bool IsActiveChoiceMeasuresNumber()
{
    return SHOW_MEASURES != 0;
}

bool IsActiveChoiceMeasuresChannels()
{
    return SHOW_MEASURES != 0;
}

bool IsActivePageMeasuresFields()
{
    return SHOW_MEASURES != 0;
}

bool IsActiveChoiceMeasuresSignal()
{
    return SHOW_MEASURES != 0;
}

bool IsActiveButtonMeasuresTune()
{
    return SHOW_MEASURES != 0;
}

bool IsActiveButtonMeasuresFieldSet()
{
    return MEASURES_FIELD_IS_HAND;
}

DEF_CHOICE_7( mcMeasuresNumber, PageMeasures::self,
    "����������", "Number"
    ,
    "������������� ������������ ���������� ��������� ���������:\n"
    "\"1\" - ���� ���������\n"
    "\"2\" - ��� ���������\n"
    "\"1�5\" - 1 ������ � ����� �����������\n"
    "\"2�5\" - 2 ������ � ����� ����������� � ������\n"
    "\"3�5\" - 3 ������ � ����� ����������� � ������\n"
    "\"6x1\" - 6 ����� �� ������ ��������� � ������\n"
    "\"6�2\" - 6 ����� �� ��� ��������� � ������"
    ,
    "Sets the maximum number of output measurements:\n"
    "\"1\" - one measurement\n"
    "\"2\" - two measurements\n"
    "\"1x5\" - 1 line with the five dimensions\n"
    "\"2x5\" - two rows with five measurements in each\n"
    "\"3x5\" - 3 lines with five measurements in each"
    "\"6x1\" - 6 lines, one in each dimension\n"
    "\"6x2\" - 6 lines of two dimensions in each\n"
    ,
    "1",   "1",
    "2",   "2",
    "1x5", "1x5",
    "2x5", "2x5",
    "3x5", "3x5",
    "6x1", "6x1",
    "6x2", "6x2",
    MEASURES_NUMBER, IsActiveChoiceMeasuresNumber, nullptr, nullptr
)

DEF_CHOICE_3(mcMeasuresChannels, PageMeasures::self,
    "������", "Channels",
    "�� ����� ������� �������� ���������",
    "Which channels to output measurement",
    "1",       "1",
    "2",       "2",
    "1 � 2",   "1 and 2",
    MEASURES_SOURCE, IsActiveChoiceMeasuresChannels, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresIsShow, PageMeasures::self,
    "����������", "Show",
    "�������� ��� �� �������� ��������� �� �����",
    "Output or output measurements on screen",
    "���", "No",
    "��",  "Yes",
    SHOW_MEASURES, nullptr, nullptr, nullptr
)

DEF_CHOICE_2(mcMeasuresSignal, PageMeasures::self,
    "���", "View",
    "��������� ��� ��� ���� ������ ������� ��� ���������� ���������� ��� ���������� ���������",
    "Decrease or no zone output signal to avoid overlapping of its measurement results",
    "��� ����",    "As is",
    "���������",   "Reduce",
    MEASURES_MODE_VIEW_SIGNALS, nullptr, nullptr, nullptr
)

static void PressSB_MeasTune_Exit()
{
}


DEF_SMALL_BUTTON(sbExitMeasTune, PageMeasures::PageTune::self,
    "�����", "Exit", "������ ��� ������ � ���������� ����", "Button for return to the previous menu",
    nullptr, PressSB_MeasTune_Exit, DrawSB_Exit, nullptr
)

DEF_PAGE_SB(pTune, PageMeasures::self, NamePage::MeasuresTune_SB,
    "���������", "CONFIGURE",
    "������� � ����� ������ ��������� ���������� � ����� ���������",
    "Transition to rezhm of exact control of quantity and types of measurements",
    sbExitMeasTune,
    Item::empty,
    Item::empty,
    Item::empty,
    sbMarkers,
    sbSettings,
    IsActiveButtonMeasuresTune, nullptr, nullptr, TableAutoMeasures::Handler::RegSet
)

DEF_PAGE_5(pageMeasures, PageMain::self, NamePage::Measures,
    "���������", "MEASURES",
    "�������������� ���������",
    "Automatic measurements",
    mcMeasuresIsShow,
    mcMeasuresNumber,
    mcMeasuresChannels,
    mcMeasuresSignal,
    *PageMeasures::PageTune::self,
    nullptr, nullptr, nullptr, nullptr
)

const Page *PageMeasures::self = &pageMeasures;
const Page *PageMeasures::PageTune::self = &pTune;
