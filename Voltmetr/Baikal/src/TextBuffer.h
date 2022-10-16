//  [12/24/2009 Sasha]
#pragma once
#include <fstream>

/* 
   Класс TextBuffer предназначен для быстрой записи в буфер
   Деструктор вызывает запись буфера в файл
*/

class TextBuffer {
public:
    // В конструктор передаётся название файла, в который будет произведена запись и размер единичного выделяемого блока в байтах
    TextBuffer(char *fileName, int sizeBlock = 1024);
    ~TextBuffer();

    void Write(char *string);
    void Flush();
private:
    int sizeBlock;
    int pointerPos;      // В эту позицию текущего блока будет производится запись
    char *buffer;        // Сюда производится запись строки, пока буфер не будет полностью заполнен (pointerPos == sizeBlock)
                         // Когда он заполнится, происходит запись в файл
    std::ofstream file;
};