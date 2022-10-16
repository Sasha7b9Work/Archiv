#include "common.h"

void IntToStrCat(char *_buffer, int _value) {
    static const int LENGHT = 10;
    char buffer[LENGHT];
    for(int i = 0; i < LENGHT; i++) {
        buffer[i] = 0;
    }
    int pointer = LENGHT -1;

    while(_value > 0) {
        buffer[pointer] = (unsigned char)(_value % 10);
        _value /= 10;
        pointer--;
    }

    while(*_buffer) {
        _buffer++;
    }
    int i = 0;
    
    for(; i < LENGHT; i++) {
        if(buffer[i] > 0) {
            break;
        }
    }

    for(; i < LENGHT; i++) {
        *_buffer = 0x30 | buffer[i];
        _buffer++;
    }

    *_buffer = 0;
}

/*
void CreateDebugMessage(const String & const _messageIn, String & const _debugMessage) {
    _debugMessage.Clear();
    _debugMessage += "\x0ff\x0fe\x0fa ";
    _debugMessage += _messageIn;
    _debugMessage += "\x0fe\x0ff";
}
*/
