/*
*************** �������� ����� ****************************
*/
#include "main.h"
#include "globals.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Panel/Panel.h"
#include "Hardware/FLASH.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_NAME "S8-53.bin"

typedef void(*pFunction)();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainStruct *ms;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upgrade();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    pFunction JumpToApplication;

    ms = (MainStruct *)malloc(sizeof(MainStruct));
    ms->percentUpdate = 0.0f;

    Hardware::Init();

    Settings::Load();

    Timer::PauseOnTime(250);
    ms->state = State_Start;

    Display::Init();

    Timer::SetAndEnable(kTemp, Display::Update, 10);

    uint timeStart = TIME_MS;

    FDrive::Init();

    while (TIME_MS - timeStart < TIME_WAIT && !FDrive::Update())
    {
    }

    if ((ms->drive.connection && ms->drive.active == 0) ||  // ���� ����� ����������, �� � �������� ��������� ������-�� �� �������
        (ms->drive.active && ms->state != State_Mount))     // ��� ������� � �������� ���������, �� ������-�� �� ���������� ������� ������������
    {
        free(ms);
        NVIC_SystemReset();
    }

    if (ms->state == State_Mount)                           // ��� ��������, ��� ���� ������ �������������
    {
        if (FDrive::FileExist(FILE_NAME))                    // ���� �� ����� ���������� ��������
        {
            ms->state = State_RequestAction;

            while (1)
            {
                PanelButton button = Panel::PressedButton();
                if (button == B_F1)
                {
                    ms->state = State_Upgrade;
                    Upgrade();
                    break;
                }
                else if (button == B_F5)
                {
                    ms->state = State_Ok;
                    break;
                }
            }
        }
        else
        {
            ms->state = State_NotFile;
        }
    }
    else if (ms->state == State_WrongFlash) // ���� �� ������� ��������������
    {
        Timer::PauseOnTime(5000);
    }

    ms->state = State_Ok;

    Panel::DeInit();

    Timer::Disable(kTemp);

    while (Display::IsRun())
    {
    }

    Display::Update();

    HAL_DeInit();

    free(ms);

    Hardware::DeInit();

    __disable_irq();
    // ������ ��������� �� �������� ���������
    JumpToApplication = (pFunction)(*(__IO uint*)(MAIN_PROGRAM_START_ADDRESS + 4));
    __set_MSP(*(__IO uint*)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Upgrade()
{
    const int sizeSector = 1 * 1024;

    uint8 buffer[sizeSector];

    FLASH_Prepare();

    int size = FDrive::OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = ADDR_SECTOR_PROGRAM_0;

    while (size)
    {
        int readedBytes = FDrive::ReadFromFile(sizeSector, buffer);
        FLASH_WriteData(address, buffer, readedBytes);
        size -= readedBytes;
        address += (uint)readedBytes;

        ms->percentUpdate = 1.0f - (float)size / fullSize;
    }

    FDrive::CloseOpenedFile();
}


/*! \page page1 �������� ������ ����������
    \verbatim
    �� ������ 0x08000000 ��������� ���������.
    ����� ������ �� ��������� ������� ������.
        ���� ������ ����:
            ��������� ������� ����� S8-53.bin. ���� ������� �������, ������� ��������� "�� USB-����� ���������� ������ ����������� �����������. ����������?"
            ���� ������ "��":
                1. ��������� �������:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. �� �� ����� ������������ ���������� ����� S8-53.bin
        ���� ������ �������������� �� �������:
            ������� ��������� "�� ������� �������������� ������. ���������, ��� �� ��� �������� ������� fat32"
    ����� ����������� ������� �� ������, ���������� � 0x0802004
    \endverbatim
*/
