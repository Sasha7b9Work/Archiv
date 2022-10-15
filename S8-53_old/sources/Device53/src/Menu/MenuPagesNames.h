#pragma once

/// ����� ID ���� �������������� � ���� �������
typedef enum
{
    Page_Display,                   ///< ������
    Page_Display_Accum,             ///< ������� - ����������
    Page_Display_Average,           ///< ������� - ����������
    Page_Display_Grid,              ///< ������� - �����
    Page_DisplayRefresh,            ///< ������� - ����������
    Page_Main,                      ///< ������� �������� ����. �� �� ������ ��� ���������
    Page_Debug,                     ///< �������
    Page_DebugConsole,              ///< ������� - �������
    Page_DebugADC,                  ///< ������� - ���
    Page_DebugADCbalance,           ///< ������� - ��� - ������
    Page_DebugADCstretch,           ///< ������� - ��� - ��������
    Page_DebugADCrShift,            ///< ������� - ��� - ���. ��������
    Page_DebugADCaltShift,          ///< ������� - ��� - ���. ����. ���.
    Page_DebugShowRegisters,        ///< ������� - ��������
    Page_DebugRandomizer,           ///< ������� - ����-���
    Page_DebugInformation,          ///< ������� - ����������
    Page_DebugSound,                ///< ������� - ����
    Page_DebugAltera,               ///< ������� - �������
    Page_ChannelA,                  ///< ����� 1
    Page_ChannelB,                  ///< ����� 2
    Page_Cursors,                   ///< �������
    Page_Trig,                      ///< �����.
    Page_Memory,                    ///< ������
    Page_Memory_Drive,              ///< ������ - �����. ��
    Page_Measures,                  ///< ���������
    Page_MeasuresField,             ///< ��������� - ����
    Page_Service,                   ///< ������
    Page_Service_Display,           ///< ������ - �������
    Page_Service_Display_Colors,    ///< ������ - ������� - �����
    Page_Service_Calibrator,        ///< ������ - ����������
    Page_Service_Ethernet,          ///< ������ - ETHERNET
    Page_Help,                      ///< ������
    Page_Time,                      ///< ���������
    Page_Math,                      ///< ������ - ����������
    Page_MathFFT,
    Page_Trig_Search,               ///< ����� - ��� �����
    PageSB_Cursors_Set,             ///< ������� �������� ������� - ����������
    PageSB_Measures_Tune,           ///< ������� �������� ��������� - ���������
    Page_SB_MeasTuneField,          ///< ������� �������� ��������� - ���� - ������
    PageSB_Memory_Last,             ///< ������� �������� ������ - ���������
    PageSB_Memory_Internal,         ///< ������� �������� ������ - ����� ��
    PageSB_Memory_Drive_Mask,       ///< ������� �������� ������ - ����� �� - �����
    PageSB_Memory_SetName,          ///< ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� ����� �� - ��� �����
    PageSB_Memory_Drive_Manager,    ///< ������� �������� ������ - ����� �� - ��TA���
    PageSB_Help,                    ///< ������� �������� HELP
    PageSB_Service_FFT_Cursors,     ///< ������� �������� ������ - ���������� - �������
    PageSB_Service_Function,        ///< ������ - ���������� - �������
    PageSB_Service_Information,     ///< ������ - ����������
    PageSB_Debug_SerialNumber,      ///< ������ - �/�
    Page_NumPages,
    Page_NoPage
} NamePage;             // ��� ���������� ������� ����� �������� ������������� SettingsMenu � SSettings.c
