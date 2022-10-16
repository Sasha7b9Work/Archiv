// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "Display/Display.h"
#include "Display/Screen/Grid.h"
#include "Menu/Items/MenuItemsDefs.h"


class Page;
class SmallButton;
class TimeItem;
class Governor;
class Choice;


#define MAX_NUM_CHOICE_SMALL_BUTTON 6    // ������������ ���������� ��������� ��������� ������ + 1

                                            
// ������ ���� ������� ����
struct TypeItem { enum E
{
    None,           
    Choice,        // ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
    Button,        // ������.
    Page,          // ��������.
    Governor,      // ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
    Time,          // ��������� ������ �����.
    IP,            // ��������� ������ IP-�����.
    SwitchButton,
    GovernorColor, // ��������� ������� ����.
    Formula,       // ��������� ������� ���� � ������������ ��� �������������� ������� (��������� � ��������)
    MAC,           // MAC-�����
    ChoiceReg,     // ������� ������, � ������� ����� �������������� �� �������, � ������
    SmallButton,   // ������ ��� ������ ����� ������
    Count
};};


// ����� ID ���� �������������� � ���� �������
struct NamePage { enum E
{
    Empty,

    MainPage,                   // ������� �������� ����. �� �� ������ ��� ���������

    Display,                    // ������
    DisplayAccumulation,        // ������� - ����������
    DisplayAverage,             // ������� - ����������
    DisplayGrid,                // ������� - �����
    DisplayRefresh,             // ������� - ����������
    DisplaySettings,            // ������� - ���������

    ChannelA,                   // ����� 1

    ChannelB,                   // ����� 2

    Trig,                       // �����.
    TrigAuto,                   // ����� - ��� �����

    Time,                       // ���������

    Cursors,                    // �������
    CursorsSet_SB,              // ������� - ����������

    Memory,                     // ������
    MemoryLatest_SB,            // ������� �������� ������ - ���������
    MemoryInt_SB,               // ������� �������� ������ - ����� ��
    MemoryExt,                  // ������ - �����. ��
    MemoryExtSetMask_SB,        // ������� �������� ������ - ����� �� - �����
    MemoryExtSetName_SB,        // ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ���������������
                                // ����� ����� �� - ��� �����
    MemoryExtFileManager_SB,    // ������� �������� ������ - ����� �� - �����

    Measures,                   // ���������
    MeasuresTune_SB,            // ��������� - ���������

    Service,                    // ������
    ServiceDisplayColors,       // ������ - ������� - �����
    ServiceCalibrator,          // ������ - ����������
    ServiceEthernet,            // ������ - ETHERNET
    ServiceMath,                // ������ - ����������
    ServiceMathFFT,
    ServiceMathFFTCursors_SB,   // ������ - ���������� - �������
    ServiceInformation_SB,      // ������ - ����������
    ServiceMathFunction_SB,     // ������ - ���������� - �������

    Help_SB,                    // ������

    Debug,                      // �������
    DebugADC,                   // ������� - ���
    DebugADCbalance,            // ������� - ��� - ������
    DebugADCstretch,            // ������� - ��� - ��������
    DebugADCrShift,             // ������� - ��� - ���. ��������
    DebugADCaltShift,           // ������� - ��� - ���. ����. ���.
    DebugAltera,                // ������� - �������
    DebugConsole,               // ������� - �������
    DebugData,                  // ������� - ����
    DebugInfo,                  // ������� - ����
    DebugRandomizer,            // ������� - ����-���
    DebugShowRegisters,         // ������� - ��������
    DebugSound,                 // ������� - ����
    DebugSerialNumber_SB,       // ������ - �/�

    Count,
    NoPage
};};             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c


#define TITLE(item) ((item)->data->titleHint[LANG])
#define HINT(item) ((item)->data->titleHint[2 + LANG])


class StructHelpSmallButton
{
public:
    pFuncVII funcDrawUGO;    // ��������� �� ������� ��������� ����������� �������� ������
    pchar    helpUGO[2];     // ������� � ������� �����������.
};

typedef StructHelpSmallButton arrayHints[MAX_NUM_CHOICE_SMALL_BUTTON];

#include "Menu/Items/Item.h"
#include "Menu/Items/ItemPage.h"
#include "Menu/Items/ItemButton.h"
#include "Menu/Items/ItemSmallButton.h"
#include "Menu/Items/ItemGovernor.h"
#include "Menu/Items/ItemIPaddress.h"
#include "Menu/Items/ItemMACaddress.h"
#include "Menu/Items/ItemFormula.h"
#include "Menu/Items/ItemGovernorColor.h"
#include "Menu/Items/ItemChoice.h"
#include "Menu/Items/ItemTime.h"
