#pragma once
#include "Command.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef bool(*pFuncBU8)(uint8);

class Decoder
{
public:

    static void AddData(uint8 data);

    static void Update();
private:

    static void RunStep(uint8 data);

    /// ����������� �������
    static pFuncBU8 curFunc;
    /// ������� ���� ����������� �������
    static int step;

    static bool EmptyFunc(uint8) { return true; }

    static bool ButtonPress(uint8);
    /// ��� ������� ���� �������� ����� ���������� ���������� ����
    static void FinishCommand();
};
