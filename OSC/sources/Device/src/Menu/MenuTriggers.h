#pragma once
#include "Keyboard/Keyboard.h"


/*
    � ���� ����� - �������� ���� - ��� �������-������� �� ������� ������� ����������
*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� - ����������� ���������� �������
class TriggerDebugConsole
{
public:
    /// ���������� true, ���� ������� �������� - ������� ���������
    static bool Update(Key key);

private:
    static const int SIZE_BUFFER_FOR_BUTTONS = 10;
    static Key bufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
    static const Key sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS];
};
