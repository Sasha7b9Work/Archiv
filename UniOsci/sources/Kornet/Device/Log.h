#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Log
{
public:
    void Message(char *format, ...);
private:
};


extern Log logger;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_MESSAGE(...)    logger.Message(__VA_ARGS__)
#define LOG_WRITE(...)