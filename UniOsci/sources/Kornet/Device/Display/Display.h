#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LogEntity
{
    friend class Log;
    void AddToConsole(char *string);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
    friend class LogEntity;
public:
    
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    void Init();

    void Update();

private:

    void DrawGrid();
    /// ���������� ���������� � ������ ����� ������.
    void WriteLowPart();

    int WriteChannel(Channel ch, int x, int y);

    void WriteTBase(int x, int y);

    void DrawRShift();

    void DrawRShift(Channel ch);

    void DrawConsole();

    void UpdateOsci();

    void UpdateTester();

    void DrawDataTester(int numStep, int x, int y);

    void UpdateMultimeter();
    /// true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    bool inProcessDrawConsole;

public:
    LogEntity logEntity;

};


extern Display display;
