#pragma once
#include "Globals.h"
#include "VCP/VCP.h"
#include "usbd_conf.h"
#include "Ethernet/TcpSocket.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup SCPI
 *  @{
 */

#define ENTER_ANALYSIS                                      \
    Word parameter;                                         \
    if (su.GetWord((const char *)buffer, &parameter, 0)) {  \
        uint8 value = GetValueFromMap(map, &parameter);     \
        if (value < 255) {

#define LEAVE_ANALYSIS   }          \
    else                            \
    {                               \
        SCPI_SEND("COMMAND ERROR"); \
    }                               \
    }


#define SCPI_SEND(...)                              \
    if(CONNECTED_TO_USB)                            \
    {                                               \
        VCP::SendFormatStringAsynch(__VA_ARGS__);   \
    };                                              \
    if (gEthIsConnected)                            \
    {                                               \
        SocketTCP::SendFormatString(__VA_ARGS__);   \
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    char       *name;   ///< �������� �������
    pFuncVU8   func;    ///< ������� ����������
} StructCommand;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� � ��������� ������ �� buffer. ���������� ����� ������������ ����, ������� ����� ������� �� ������ ������
int SCPI_ParseNewCommand(uint8 *buffer, int length);

void SCPI_ProcessingCommand(const StructCommand *commands, uint8 *buffer);

void Process_DISPLAY(uint8 *buffer);

void Process_CHANNEL(uint8 *buffer);

void Process_TRIG(uint8 *buffer);

void Process_TBASE(uint8 *buffer);
/// \brief ��������� ����� ����� �� ������ � �������� �� ������ value. ���������� false, ���� � ������ ������ �� ��������� ����� ��� ����� �� ������ 
/// � ������� [min; max]
bool SCPI_FirstIsInt(uint8 *buffer, int *value, int min, int max);

/** @}
 */
