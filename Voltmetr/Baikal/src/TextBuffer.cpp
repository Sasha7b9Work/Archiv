//  [12/24/2009 Sasha]
#include "TextBuffer.h"

using std::ios;

//  [12/24/2009 Sasha]
TextBuffer::TextBuffer(char *_fileName, int _sizeBlock):pointerPos(0) {
    file.open(_fileName, ios::out);
    sizeBlock = _sizeBlock;
    buffer = new char[sizeBlock];
}

//  [12/24/2009 Sasha]
TextBuffer::~TextBuffer() {
    Flush();
    file.close();
    delete []buffer;
}

// [12/24/2009 Sasha]
void TextBuffer::Write(char *_string) {
    int sizeString = strlen(_string);
    if((pointerPos + sizeString) > sizeBlock) {
        Flush();
        pointerPos = 0;
    }
#pragma warning(disable:4996)
    strcpy(&buffer[pointerPos], _string);
#pragma warning(default:4996)
    pointerPos += sizeString;
}

//  [12/24/2009 Sasha]
void TextBuffer::Flush() {
    buffer[pointerPos] = 0;
    file << buffer;
}