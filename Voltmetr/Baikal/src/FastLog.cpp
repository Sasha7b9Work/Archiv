//  [12/24/2009 Sasha]
#include "FastLog.h"
#include "TextBuffer.h"
#include "Timer.h"
#include <iostream>

using std::cout;

//  [12/24/2009 Sasha]
FastLog::FastLog() {
    textBuffer = new TextBuffer(logName, 1024 * 1024 * 100);
    timer = new Timer;
    timer->Reset();
    FreeConsole();
    AllocConsole();
    ho = GetStdHandle(STD_OUTPUT_HANDLE);
}

//  [12/24/2009 Sasha]
FastLog::~FastLog() {
    CloseHandle(ho);
    FreeConsole();
    delete textBuffer;
    delete timer;
};

//  [12/24/2009 Sasha]
FastLog& FastLog::TheLog() {
    static FastLog log;
    return log;
}

//  [12/29/2009 Sasha]
void FastLog::Write(char *nameFile, int numStr, char *msg) {
    static char buffer[4096];
#pragma warning(disable:4996)
    sprintf(buffer, "%s\n           - %s:%d - %s\n", msg, nameFile, numStr, timer->GetTime().ToStdString());
#pragma warning(default:4996)
    textBuffer->Write(buffer);
    cout << buffer;
}

//  [12/29/2009 Sasha]
void FastLog::WriteFormat(char *nameFile, int numStr, char *format, ...) {
    static char buffer[4096];
    DWORD d;
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, 4096, format, args);
    va_end(args);
    static char buffer2[4096];
#pragma warning(disable:4996)
    sprintf(buffer2, "%s - %s:%d - %s\n", buffer, nameFile, numStr, timer->GetTime().ToStdString());
    //sprintf(buffer2, "%s - %s:%d\n", buffer, nameFile, numStr);
#pragma warning(default:4996)
    textBuffer->Write(buffer2);
    //cout << buffer2;
    WriteConsoleA(ho, buffer2, lstrlenA(buffer2), &d, NULL);
}