#include "defines.h"
#include "VCP.h"
#include "Utils/Math.h"
#include "SCPI/SCPI.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "Hardware/Timer.h"
#include <usbd_cdc.h>
#include <usbd_def.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
USBD_HandleTypeDef handleUSBD;
PCD_HandleTypeDef handlePCD;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 

static bool PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}

void VCP::SendDataAsinch(uint8 *buffer, int size)
{
    const int SIZE_BUFFER = 64;
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::MinFrom2Int(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {};
    memcpy(trBuf, buffer, size);

    USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}

static const int SIZE_BUFFER_VCP = 256;     // WARN ???? ????????? ?????? ?????? 512, ?? ?? ??207 ?????
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;

void VCP::Flush()
{
    if (sizeBuffer)
    {
        USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
        while (pCDC->TxState == 1) {};
        USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, sizeBuffer);
        USBD_CDC_TransmitPacket(&handleUSBD);
        while (pCDC->TxState == 1) {};
    }
    sizeBuffer = 0;
}

void VCP::SendDataSynch(const uint8 *buffer, int size)
{
    if (gBF.connectToHost == 0)
    {
        return;
    }

    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;

    do 
    {
        if (sizeBuffer + size > SIZE_BUFFER_VCP)
        {
            int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
            Limitation<int>(&reqBytes, 0, size);
            while (pCDC->TxState == 1) {};
            memcpy(buffSend + sizeBuffer, buffer, reqBytes);
            USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, SIZE_BUFFER_VCP);
            USBD_CDC_TransmitPacket(&handleUSBD);
            size -= reqBytes;
            buffer += reqBytes;
            sizeBuffer = 0;
        }
        else
        {
            memcpy(buffSend + sizeBuffer, buffer, size);
            sizeBuffer += size;
            size = 0;
        }
    } while (size);
}

void SendData(const uint8 *buffer, int size)
{

}

void VCP::SendStringAsinch(char *data)
{
    SendDataAsinch((uint8*)data, strlen(data));
}

void VCP::SendStringSynch(char *data)
{
    SendDataSynch((uint8*)data, strlen(data));
}

void VCP::SendFormatStringAsynch(char *format, ...)
{
    static const int SIZE_BUFFER = 200;
    static char buffer[SIZE_BUFFER];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    strcat(buffer, "\n");
    SendDataAsinch((uint8*)buffer, strlen(buffer));
}

void VCP::SendFormatStringSynch(char *format, ...) {
    static const int SIZE_BUFFER = 200;
    char buffer[SIZE_BUFFER];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    strcat(buffer, "\n");
    SendDataSynch((uint8*)buffer, strlen(buffer));
}

void VCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void OTG_FS_IRQHandler() {
    HAL_PCD_IRQHandler(&handlePCD);
}

#ifdef __cplusplus
}
#endif
