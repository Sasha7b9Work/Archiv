#pragma once
#include <usbd_def.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VCP_FLUSH()                         VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size)   VCP::SendDataSynch(buffer, size)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    static USBD_HandleTypeDef handleUSBD;
    static PCD_HandleTypeDef  handlePCD;

private:
    static bool PrevSendingComplete();
};
