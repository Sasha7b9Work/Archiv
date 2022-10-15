#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4100 4101 4189)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Log logger;

#define SIZE_BUFFER 200


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Message(char *format, ...)
{
    static char buffer[SIZE_BUFFER];
    buffer[0] = 0;
    char *pointer = buffer;

    va_list args;

    va_start(args, format);
    vsprintf(pointer, format, args);
    va_end(args);

    display.logEntity.AddToConsole(buffer);
}

#ifdef WIN32
#pragma warning(pop)
#endif
