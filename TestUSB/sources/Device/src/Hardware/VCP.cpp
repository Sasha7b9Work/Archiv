#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#ifndef WIN32
#include "defines.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include <stdarg.h>
#endif

namespace Math
{
template<class T>
T Min(T x1, T x2)
{
    return (x1 < x2) ? x1 : x2;
}

}

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBD_HandleTypeDef VCP::handleUSBD;
PCD_HandleTypeDef  VCP::handlePCD;
bool               VCP::cableUSBisConnected = false;
bool               VCP::connectedToUSB = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendDataAsynch(const uint8 *buffer, uint size)
{
#define SIZE_BUFFER 64U
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::Min(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {};
    std::memcpy(trBuf, buffer, (uint)size);

    USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, (uint16)size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define SIZE_BUFFER_VCP 256     /// \todo ???? ????????? ?????? ?????? 512, ?? ?? ??207 ?????
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;
//static uint8 buffReceive[SIZE_BUFFER_VCP];
//static int sizeBufferRx = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::Flush()
{
    if (sizeBuffer)
    {
        volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;

        while (pCDC->TxState == 1) {}; //-V712

        USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, (uint16)sizeBuffer);
        USBD_CDC_TransmitPacket(&handleUSBD);

        while (pCDC->TxState == 1) {}; //-V654 //-V712
    }
    sizeBuffer = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendDataSynch(const void *_buffer, uint size)
{
    if (CONNECTED_TO_USB)
    {
        char *buffer = (char *)_buffer;
        if (size == 0)
        {
            size = std::strlen(buffer);
        }

        volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    
        do 
        {
            if (sizeBuffer + size > SIZE_BUFFER_VCP)
            {
                int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
                LIMITATION(reqBytes, 0, (int)size); //-V2516

                while (pCDC->TxState == 1) {}; //-V712

                std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)reqBytes);
                USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, SIZE_BUFFER_VCP);
                USBD_CDC_TransmitPacket(&handleUSBD);
                size -= reqBytes;
                buffer += reqBytes;
                sizeBuffer = 0;
            }
            else
            {
                std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)size);
                sizeBuffer += size;
                size = 0;
            }
        } while (size);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendStringAsynch(char *data)
{
    SendDataAsynch((uint8 *)data, std::strlen(data));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8 *)data, std::strlen(data));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendFormatStringAsynch(char *format, ...)
{
    if (CONNECTED_TO_USB)
    {
        static char buffer[200];
        va_list args;
        va_start(args, format);
        std::vsprintf(buffer, format, args);
        va_end(args);
        std::strcat(buffer, "\r\n");
        SendDataAsynch((uint8 *)buffer, std::strlen(buffer));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendFormatStringSynch(char *format, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\r\n");
    SendDataSynch((uint8 *)buffer, std::strlen(buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void VCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}

