#pragma once
#include "Keyboard/Keyboard.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BufferButtons
{
public:
    static void Push(KeyEvent str);
    static KeyEvent Extract();
    static bool IsEmpty();

private:
    enum
    {
        SIZE = 100
    };

    /// ����� �������� ��� �������
    static KeyEvent buffer[SIZE];
    /// ������� ������� ���������� ����� � ������. ����� end == 0, ����� ������
    static int end;
    /// ������� ������� ��������� ������� � ������.
    static int start;
};
