#pragma once
#include "Settings/Settings.h"


/*
    ������ � ����������� EEPROM AT25160N, ��������������� ��� �������� ��������.
*/
class AT25160N
{
friend class Settings;
public:
    static void Init();
private:
    static void Save(Settings &set);
    static void Load(Settings &set);
    /// ��������� ������
    static void SetWriteLatch();
    /// ��������� ������
    static void ResetWriteLatch();
    /// ������ ������� �������
    static uint8 ReadStatusRegister();
    /// �������� ������� �������
    static void WriteStatusRegister(uint8 data);
    /// ���������� size ����, ������� � ������ address
    static void WriteData(uint address, uint8 *data, uint size);
    /// �������� ������ ������ �� ������� ������. ������ �� ����� ���� ������ 32 ����
    static void Write32BytesOrLess(uint address, uint8 *data, uint size);
    /// ������ size ����, ������� � ������ address
    static void ReadData(uint address, uint8 *data, uint size);

    /// ���������� ���� � ����������
    static void WriteByte(uint8 byte);
    /// ������ ���� �� ����������
    static uint8 ReadByte();

    /// �������, ���� �� ���������� ��������� ���� ������
    static void WaitFinishWrite();
    /// ���������� �������� ����� � 1
    static void SetPin(GPIO_TypeDef *gpio, uint16 pin);
    /// ���������� �������� ����� � 0
    static void ResetPin(GPIO_TypeDef *gpio, uint16 pin);
};
