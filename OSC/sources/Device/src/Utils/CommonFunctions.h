#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CF
{
public:
    /// ������� � ������� num ������ �������� �� buffer
    static void LogBufferU8(uint8 *buffer, int num);
    /// ���������� ����� ���� � ������ string
    static int NumWords(char *string);
    /// ���������� ��������� �� n ����� � ������. ���� char == 0 - ����� ���, ���� ret value == 0xffffffff - �������� ����� ������� ���
    static char *GetWord(char *string, int n, char *out, int size);
};


// � ������ ����� ������� ������ Push �������� ���� ���������� ����� ���������� ������ Clear() ��������
class BufferMax5
{
public:
    BufferMax5();
    void Clear();
    void Push(uint16 value);
    uint16 GetValue(int index);
private:
    uint16 buffer[5];
    int numElements;
};
