#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cViewAlways,                                                                                     //--- ������� - ����������� - ������ ---
    "������", "Always",
    "��������� �������� ������ ������������ ��� ����������� ���� �����������",
    "Allows you to display data from the recorder when the recorder menu is off",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_VIEW_ALLWAYS, pageRecorder, FuncActive, FuncChangedChoice, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnEnterExit_Recorder(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_4( pageRecorder,                                                                                              //--- ������� - ����������� ---
    "�����������", "RECORDER",
    "������ � ��������������� �������� ������ � ��������",
    "Recording and playback of input signals and sensors",
    &cViewAlways,                                       ///< ������� - ����������� - ������
    PageFunction::PageRecorder::PageSource::pointer,    ///< ������� - ����������� - ��������
    PageFunction::PageRecorder::PageRecord::pointer,    ///< ������� - ����������� - ������
    PageFunction::PageRecorder::PagePlay::pointer,      ///< ������� - ����������� - ��������
    Page::Name::Function_Recorder, PageFunction::pointer, FuncActive, OnEnterExit_Recorder, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
