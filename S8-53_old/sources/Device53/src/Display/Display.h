#pragma once
#include "globals.h"
#include "Display/Painter.h"
#include "Tables.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_RUN_AFTER_DRAW(func) Display::RunAfterDraw(func)
#define DISPLAY_ADD_STRING(x)        Display::AddStringToIndicating(x)
#define DISPLAY_SHOW_WARNING(warn)   Display::ShowWarning(warn)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    static void DeleteFirstString();

    static void DrawStringInRectangle(int x, int y, char const *text);

    static void AddString(const char *string);
    /// ���� data == 0, �� ������ ����� �� Processing_GetData
    static void DrawDataChannel(uint8 *data, Channel chan, DataSettings *ds, int minY, int maxY);

    static void DrawBothChannels(uint8 *data0, uint8 *data1);

    static void DrawDataMemInt();

    static void DrawDataInModeWorkLatest();

    static void DrawDataInModePoint2Point();

    static bool DrawDataInModeNormal();

    static void DrawDataMinMax();

    static bool DrawDataNormal();
    
    static bool DrawData();
    /// shiftForPeakDet - ���� ������ ���������� � �������� ��������� - �� ����� shiftForPeakDet ����� ����������� ����������� � ����������.
    static void DrawDataInRect(int x, int width, const uint8 *data, int numElems, Channel chan, int shiftForPeakDet);
    /// shiftForPeakDet - ���� ������ ���������� � �������� ��������� - �� ����� shiftForPeakDet ����� ����������� ����������� � ����������.
    static void DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI, const uint8 *data, int rightX, 
        Channel chan, int shiftForPeakDet);
    /// ���������� ���� ������
    static void DrawMemoryWindow();
    
    static void DrawScaleLine(int x, bool forTrigLev);
    
    static void WriteTextVoltage(Channel chan, int x, int y);
    
    static void WriteStringAndNumber(char *text, int16 x, int16 y, int number);
    
    static int FirstEmptyString();
    
    static int CalculateFreeSize();
};
