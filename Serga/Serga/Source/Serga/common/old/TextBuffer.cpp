#include "TextBuffer.h"
#include <string>

using std::ios;

TextBuffer::TextBuffer(char *_fileName, int _sizeBlock):pointerPos(0) 
{
    file.open(_fileName, ios::out);
    sizeBlock = _sizeBlock;
    buffer = new char[sizeBlock];
}

TextBuffer::~TextBuffer() 
{
    Flush();
    file.close();
    delete []buffer;
}

void TextBuffer::Write(char *_string) 
{
    int sizeString = strlen(_string);
    if((pointerPos + sizeString) > sizeBlock) 
        Flush();
    
#pragma warning(disable:4996)
    strcpy(&buffer[pointerPos], _string);
#pragma warning(default:4996)
    pointerPos += sizeString;
}

void TextBuffer::Flush() 
{
    buffer[pointerPos] = 0;
    pointerPos = 0;
    file << buffer;
    file.flush();
}