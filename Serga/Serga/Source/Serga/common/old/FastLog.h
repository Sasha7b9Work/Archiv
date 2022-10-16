//  [12/24/2009 Sasha]
#pragma once

class Timer;
class TextBuffer;

#define MY_DEBUG

#define LOG
#define ERROR_LOG

#ifdef MY_DEBUG
    #undef LOG
    #undef ERROR_LOG

    //#define LOG(msg)   FastLog::TheLog().Write(__FILE__, __LINE__, msg);
    #define LOG(...) FastLog::TheLog().WriteFormat(__FILE__, __LINE__, __VA_ARGS__);
    #define ERROR_LOG(...) FastLog::TheLog().WriteFormat(__FILE__, __LINE__, __VA_ARGS__);
#endif

class FastLog {
#define logName "log.log"
public:
    static FastLog& TheLog();
    ~FastLog();
    void Write           (char *nameFile, int numStr, char *msg);
    void WriteFormat     (char *nameFile, int numStr, char *format, ...);
private:
    FastLog();
    Timer *timer;
    TextBuffer *textBuffer;
    HANDLE hConsole;
};
