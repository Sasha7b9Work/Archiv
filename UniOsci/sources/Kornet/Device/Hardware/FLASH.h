#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FlashMemory
{
public:
    void SaveSettings();
    void LoadSettings();
private:
    /// ���������� ����� ������� ���������� ����� � ������� ��������
    uint FirstFreeAddressForSettings();
    /// \brief ���������� ����� ���������� �������� ��� 0, ���� ��������� �� �����������. fromEnd ���������, ����� ��������� �� �����
    /// ����� ��������� - 0 - ���������, 1 - ������������� � ��� �����
    uint AddressSavedSettings(int fromEnd);
    /// ���������� ����� ������� ���������� ����� ��� ������
    uint AddressFirstEmptyByte();

    uint GetSector(uint address);

    void EraseSector(uint address);

    void WriteBytes(uint address, uint8 *data, int size);

    void ReadBytes(uint address, void *data, uint size);
};


extern FlashMemory flash;

