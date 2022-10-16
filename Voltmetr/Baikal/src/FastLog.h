//  [12/24/2009 Sasha]
#pragma once
#include <Windows.h>

class Timer;
class TextBuffer;

//#define MY_DEBUG

#ifdef MY_DEBUG
	#define LOG(...) FastLog::TheLog().WriteFormat(__FILE__, __LINE__, __VA_ARGS__);
	#define ERROR_LOG(...) FastLog::TheLog().WriteFormat(__FILE__, __LINE__, __VA_ARGS__);
#else
	#define LOG
	#define ERROR_LOG
#endif

class FastLog {
#define logName "log.txt"
public:
    static FastLog& TheLog();
    ~FastLog();
    void Write           (char *nameFile, int numStr, char *msg);
    void WriteFormat     (char *nameFile, int numStr, char *format, ...);
private:
    FastLog();
    Timer *timer;
    TextBuffer *textBuffer;
    HANDLE ho;
};
