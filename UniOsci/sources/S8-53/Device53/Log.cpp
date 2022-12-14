#include "Log.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include <stdarg.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log_Write(char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(buffer);
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void Log_Error(const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    char numBuffer[20];
    sprintf(numBuffer, ":%d", numLine);
    char message[SIZE_BUFFER_LOG];
    message[0] = 0;
    strcat(message, "!!!ERROR!!! ");
    strcat(message, module);
    strcat(message, " ");
    strcat(message, func);
    strcat(message, numBuffer);
    Display::AddStringToIndicating(message);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(message);
        VCP::SendFormatStringAsynch(buffer);
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void Log_DisconnectLoggerUSB()
{
    //static uint8 data = 20;
    //Log_Write("??????? %d", data);
    //VCP_SendData(&data, 1);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
void Log_EnableLoggerUSB(bool enable)
{
    loggerUSB = enable;
}
