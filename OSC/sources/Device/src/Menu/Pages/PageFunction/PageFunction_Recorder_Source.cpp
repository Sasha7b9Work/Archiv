#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSource;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSourceA,                                                                            //--- ������� - ����������� - �������� - ����� 1 ---
    "����� 1", "Channl 1",
    "����� ������ 1 ��� ������ � ���������",
    "Select channel 1 for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_A, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cSourceB,                                                                            //--- ������� - ����������� - �������� - ����� 2 ---
    "����� 2", "Channl 2",
    "����� ������ 2 ��� ������ � ���������",
    "Select channel 2 for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_B, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cSourceSensor,                                                                        //--- ������� - ����������� - �������� - ������ ---
    "������", "Sensor",
    "����� ������� ��� ������ � ���������",
    "Sensor selection for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_SENSOR, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageSource,                                                                                     //--- ������� - ����������� - �������� ---
    "��������", "SOURCE",
    "����� ������������ ��������",
    "Selection of recorded signals",
    &cSourceA,       ///< ������� - ����������� - �������� - ����� 1
    &cSourceB,       ///< ������� - ����������� - �������� - ����� 2
    &cSourceSensor,  ///< ������� - ����������� - �������� - ������
    Page::Name::Function_Recorder_Source, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageSource::pointer = &pageSource;
