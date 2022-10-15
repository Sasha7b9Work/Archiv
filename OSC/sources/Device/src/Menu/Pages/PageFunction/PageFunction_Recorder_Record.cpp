#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecord;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cDestinationSource,                                                                //--- ������� - ����������� - ������ - ��������� � ---
    "��������� �", "Save to",
    "���� ��������� ������",
    "Where to save data",
    "USB-PC",    "USB-PC",
    "USB-Flash", "USB-Flash",
    RECORDER_STORAGE_RECORD, pageRecord, FuncActive, FuncChangedChoice, FuncDraw
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1(pageRecord,                                                                                        //--- ������� - ����������� - ������ ---
    "������", "RECORD",
    "������ � ������ ������",
    "Access to data record",
    &cDestinationSource,        ///< ������� - ����������� - ������ - ��������� �
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
