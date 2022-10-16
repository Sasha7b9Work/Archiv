#include "String.h"
#include <string.h>

String::String() {
    data = 0;
    length = 0;
}

String::String(const char *_s) {
    data = 0;
    length = strlen(_s);

    data = new char[length + 1];
    strcpy(data, _s);
}

String::String(int _value) {
    static const int LENGTH = 10;
    
    if(_value == 0) {
        length = 1;
        data = new char[length + 1];
        data[0] = '0';
        data[1] = '\0';
    } else {
        char buffer[LENGTH];
        int pointer = 0;
        while(_value > 0) {
            buffer[pointer++] = ((char)(_value % 10)) | 0x30;
            _value /= 10;
        }
        buffer[pointer] = '\0';

        length = strlen(buffer);
        data = new char[length + 1];
        for(pointer = 0; pointer < length; pointer++) {
            char symbol = buffer[length - pointer - 1];
            data[pointer] = symbol;
        }
        data[pointer] = '\0';
   }
}

String::~String() {
    if(data) {
        delete [] data;
    }
}

void String::Clear() {
    delete [] data;
    data = 0;
    length = 0;
}

int String::Length() {
    return length;
}

String& String::operator +=(const char *_s) {
    length += strlen(_s);
    char *tmpData = new char[length + 1];
    if(data) {
        strcpy(tmpData, data);
        strcat(tmpData, _s);
    } else {
        strcpy(tmpData, _s);
    }
    delete [] data;
    data = tmpData;
    return *this;
}

String& String::operator +=(const String &_string) {
    (*this) += _string.c_str();
    return *this;
}

const char* String::c_str() const {
    return data;
}
