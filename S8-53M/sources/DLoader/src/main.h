// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>
#include "Display/Display.h"
#include "Hardware/FDrive/FDrive.h"


#define FILE_NAME "S8-53M.bin"
#define ENABLE_UPDATE
    

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration

#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5    // ����� ������ ��������


struct State { enum E {
    NoDrive,            // ��������� ��������. ���� ���, �� �� ���������� ������
    DriveDetected,      // �������� ���������
    DriveNeedMount,     // ��������� ������������ ������
    DriveIsMounted,     // ������ ��������������
    WrongDrive,         // ������ ����, �� ��������� ������
    FileNotFound,       // ���� ���� �������������, �� ���������� �� �� ����
    EraseSectors,       // ���������� �������� ��������
    UpdateInProgress,   // ��� ������� ����������
    UpdateIsFinished    // ���������� ������ ���������
};};


struct MainStruct
{
    static float percentUpdate;
    static State::E state;

    static int sizeFirmware;        // ������ ��������
    static int sizeUpdated;         // ��������� ����
    static int speed;               // �������� ���������� � ������/�������
    static int timeLeft;            // �������� �������
};
