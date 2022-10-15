#pragma once


class VCP
{
public:
    static void Init();
    static void SendDataAsinch(uint8 *buffer, int size);
    static void SendDataSynch(const uint8 *data, int size);
    static void SendStringSynch(char *data);                   ///< ��������� ������ ��� ������������ ����.
    static void SendFormatStringAsynch(char *format, ...);     ///< ��� ������ ��������� � ����������� �������� \n.
    static void SendFormatStringSynch(char *format, ...);      ///< ��� ������ ��������� � ����������� �������� \n.
    static void SendByte(uint8 data);
    static void Flush();
    static void SendStringAsinch(char *data);
};
