#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "FPGA/DataStorage.h"


/// ��������� � ����������� ������
#define ADDR_FLASH_SECTOR_0     ((uint)0x08000000)  ///< Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((uint)0x08004000)  ///< Base @ of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((uint)0x08008000)  ///< Base @ of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((uint)0x0800C000)  ///< Base @ of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((uint)0x08010000)  ///< Base @ of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((uint)0x08020000)  ///< Base @ of Sector 5, 128 Kbytes  |
#define ADDR_FLASH_SECTOR_6     ((uint)0x08040000)  ///< Base @ of Sector 6, 128 Kbytes  | �������� ��������
#define ADDR_FLASH_SECTOR_7     ((uint)0x08060000)  ///< Base @ of Sector 7, 128 Kbytes  /

/// ���������� ������
#define ADDR_SECTOR_DATA_MAIN   ((uint)0x08080000)  ///< Base @ of Sector 8, 128 Kbytes
#define ADDR_SECTOR_DATA_HELP   ((uint)0x080A0000)  ///< Base @ of Sector 9, 128 Kbytes

/// ����������� � �������� �������
#define ADDR_SECTOR_RESOURCES   ((uint)0x080C0000)  ///< Base @ of Sector 10, 128 Kbytes

/// ���������
#define ADDR_SECTOR_SETTINGS    ((uint)0x080E0000)  ///< Base @ of Sector 11, 128 Kbytes


#define MAX_NUM_SAVED_WAVES 23  ///< ���� ���������� ����������� �����������, ������� ����� ������� � ���� ����� ����� �����

bool FLASH_LoadSettings(void);
void FLASH_SaveSettings(void);
     
void FLASH_GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);                    ///< ���� ������� ����, ��������������� ������� ������� ����� true/.
bool FLASH_ExistData(int num);
void FLASH_SaveData(int num, DataSettings *ds, uint8 *data0, uint8 *data1);
bool FLASH_GetData(int num, DataSettings **ds, uint8 **data0, uint8 **data1);
void FLASH_DeleteData(int num);

bool OTP_SaveSerialNumber(char *serialNumber);  ///< 
int OTP_GetSerialNumber(char buffer[17]);       ///< \brief ������� ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������. 
