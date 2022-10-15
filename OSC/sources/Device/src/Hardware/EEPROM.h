#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief ����� ����������� �� ���������� ���� ���������. ���� ���������� ����������� �����������, ������� ����� 
                                ///< ������� � ���� ����� ����� �����.


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EEPROM
{
public:

    static void SaveSettings();

    static bool LoadSettings();

    static void DeleteAllData();

    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static void DeleteData(int num);

    static void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// ���� ������� ����, ��������������� ������� ������� ����� true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);

private:
    /// ���������� ����� ������� ���������� ����� � ������� ��������
    static uint FirstFreeAddressForSettings();
    /// \brief ���������� ����� ���������� �������� ��� 0, ���� ��������� �� �����������. fromEnd ���������, ����� ��������� �� �����
    /// ����� ��������� - 0 - ���������, 1 - ������������� � ��� �����
    static uint AddressSavedSettings(int fromEnd);
    /// ���������� ����� ������� ���������� ����� ��� ������
    static uint AddressFirstEmptyByte();

    static uint GetSector(uint address);

    static void EraseSector(uint address);

    static void WriteBytes(uint address, uint8 *data, int size);

    static void ReadBytes(uint address, void *data, uint size);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    static int GetSerialNumber(char buffer[17]);
};

