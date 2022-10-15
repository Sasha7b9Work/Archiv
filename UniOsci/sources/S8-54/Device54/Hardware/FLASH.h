#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"
#include "Display/Display.h"
#include "Data/DataStorage.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup FLASH
 *  @brief ������ � ���������� ����
 *  @{
 */

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.

class FlashEEPROM
{
public:
    void LoadSettings();  ///< ���� onlyNonReset == true, ����������� ������ �������������� ���������.
    void SaveSettings();
    
    void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);    ///< ���� ������� ����, ��������������� ������� ������� ����� true.
    bool ExistData(int num);
    void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    void DeleteData(int num);
    void DeleteAllData();
};

extern FlashEEPROM flash;

class FlashOTP
{
public:
    bool SaveSerialNumber(char *servialNumber);
    int GetSerialNumber(char buffer[17]);   ///< ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
};

extern FlashOTP otp;

/** @} @}
 */
