#include "FastLog.h"
#include "TextBuffer.h"
#include "Timer.h"
#include <stdio.h>
#include <iostream>

using std::cout;

FastLog::FastLog() 
{
    AllocConsole();
    //HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
    textBuffer = new TextBuffer(logName, 1024 * 1024 * 100);
    timer = new Timer;
    timer->Reset();
}

FastLog::~FastLog() 
{
    delete textBuffer;
    delete timer;
};

FastLog& FastLog::TheLog() 
{
    static FastLog log;
    return log;
}

void FastLog::Write(char *nameFile, int numStr, char *msg) 
{
    static char buffer[4096];
#pragma warning(disable:4996)
    sprintf(buffer, "%s\n           - %s:%d - %s\n", msg, nameFile, numStr, timer->GetTime().ToStdString());
#pragma warning(default:4996)
    textBuffer->Write(buffer);
    cout << buffer;
}

void FastLog::WriteFormat(char * nameFile, int numStr, char * format, ...) 
{
    static char buffer[4096];
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, 4096, format, args);
    va_end(args);
    static char buffer2[4096];
#pragma warning(disable:4996)
    //sprintf(buffer2, "%s\n           - %s:%d - %s\n", buffer, nameFile, numStr, timer->GetTime().ToStdString());
    sprintf(buffer2, "%s - %s:%d\n", buffer, nameFile, numStr);
    //sprintf(buffer2, "%s\n", buffer);
#pragma warning(default:4996)
    textBuffer->Write(buffer2);
    textBuffer->Flush();
    cout << buffer2;
}