//  [12/24/2009 Sasha]
#pragma once
#include <fstream>

/* 
   ����� TextBuffer ������������ ��� ������� ������ � �����
   ���������� �������� ������ ������ � ����
*/

class TextBuffer {
public:
    // � ����������� ��������� �������� �����, � ������� ����� ����������� ������ � ������ ���������� ����������� ����� � ������
    TextBuffer(char *fileName, int sizeBlock = 1024);
    ~TextBuffer();

    void Write(char *string);
    void Flush();
private:
    int sizeBlock;
    int pointerPos;      // � ��� ������� �������� ����� ����� ������������ ������
    char *buffer;        // ���� ������������ ������ ������, ���� ����� �� ����� ��������� �������� (pointerPos == sizeBlock)
                         // ����� �� ����������, ���������� ������ � ����
    std::ofstream file;
};