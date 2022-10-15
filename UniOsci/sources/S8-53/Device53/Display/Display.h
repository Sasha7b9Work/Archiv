#pragma once
#include "Display/Painter.h"
#include "Tables.h"


class Display
{
public:
    static void Init();

    static void Update();

    static void RotateRShift(Channel channel);

    static void RotateTrigLev();

    static void Redraw();

    static void EnableTrigLabel(bool enable);
    /// ������� ������ ���������� ��� ������� ���������� ������. ���������� ��� ��������� �������� �� ����������.
    static void ResetP2Ppoints(bool empty);
    /// ��������� ����� � ������ ����������� ������ � ��������� data00, data01 - ��� ������� ������; data10, data11 - ��� ������� ������
    static void AddPoints(uint8 data00, uint8 data01, uint8 data10, uint8 data11);

    static void ShowWarningBad(Warning warning);

    static void ShowWarningGood(Warning warning);

    static void ClearFromWarnings();

    static void SetDrawMode(DrawMode mode, pFuncVV func);

    static void SetAddDrawFunction(pFuncVV func);

    static void RemoveAddDrawFunction();

    static void Clear();

    static void ShiftScreen(int delta);

    static void ChangedRShiftMarkers();

    static void AddStringToIndicating(const char *string);

    static void OneStringUp();

    static void OneStringDown();

    static void SetPauseForConsole(bool pause);
    /// ����� ��������� ���������� ������ ��� ������� ����� ������� ���� ���.
    static void RunAfterDraw(pFuncVV func);
    /// ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftA();
    /// ��������� ��������������� ����� ������� �������� �� ���������� ������� ������.
    static void DisableShowLevelRShiftB();
    
    static void DisableShowLevelTrigLev();

    static void OnRShiftMarkersAutoHide();

    static void ShowWarning(Warning warn);

private:

    static void ShowWarn(const char *message);
    /// ���������� �����.
    static void DrawGrid(int left, int top, int width, int height);
    /// ���������� ������ �����.
    static void DrawFullGrid();

    static void DrawCursorsWindow();

    static void DrawCursorsRShift();
    /// ���������� ������� �������� �� ����������
    static void DrawCursorRShift(Channel chan);
    /// ���������� ������ ������ �������������.
    static void DrawCursorTrigLevel();
    /// ���������� ������ �������� �� �������.
    static void DrawCursorTShift();
    /// �������� ���������� ��� ������ - � ������ ����� �������.
    static void DrawLowPart();

    static void DrawHiPart();

    static void DrawHiRightPart();

    static void DrawMath();

    static void DrawSpectrum();
    /// ������� ������� �����.
    static void DrawTime(int x, int y);
    /// ���������� ������� ��������� ���������.
    static void DrawCursors();
    /// ������� �������� �������� ��������� ���������.
    static void WriteCursors();
    /// ������� �������� �������������� ���������.
    static void DrawMeasures();
    /// �������� ��������� ���������� �������.
    static void DrawConsole();
    /// �������� ��������������.
    static void DrawWarnings();

    static void DrawTimeForFrame(uint timeMS);
    /// ���������� �������������� ������ ��������� ���������.
    static void DrawHorizontalCursor
                                (int y,             ///< �������� �������� �������.
                                int xTearing        ///< ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    /// ���������� ������������ ������ ��������� ���������.
    static void DrawVerticalCursor
                                (int x,             ///< �������� �������� �������.
                                int yTearing        ///< ���������� �����, � ������� ���������� ������� ������ ��� �������� �����������.
                                );
    /// ������� �������� ������ �������������. 
    static void WriteValueTrigLevel();

};
