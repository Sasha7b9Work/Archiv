//  [7/27/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QColor>
#pragma warning(default:4127)
#include "TypesDisplay.h"

struct SettingsDisplay {

    SettingsDisplay();
    ~SettingsDisplay();

    NS_TypesDisplay::TypeDisplay typeDisplay;

    NS_TypesDisplay::TypeDisplay GetTypeDisplay() {
        return typeDisplay;
    }

    inline void SetActiveCurs(int _activeCurs) {
        activeCurs = _activeCurs;
    }

    inline int GetActiveCurs() {
        return activeCurs;
    }

    NS_TypesDisplay::TypeGrid typeGrid;

    NS_TypesDisplay::DrawMode drawMode;

    NS_TypesDisplay::ModeAccumulation modeAccumulation;

    bool antialiasing;
    bool trigImpulse;           // ������ ������� ����, ����� �� �������� ������� ��������������

    int numberAccumulation;     // ��������� ����������
    int countAccumulation;      // ���������� ����� ����������

    // �������
    //int spectr;                 // ���� 0, ������� �� ���������, ���� 1 - ��������� ������ ������� ������, ���� 2 - ��������� ������ ������� ������
    bool enableCursU[3];        // ���� ������� ��������� � �������� ��������� - ������� ������ �������� - true, ����� - false
    bool enableCursT[3];        // --//--

    QColor colorBackground;
    QColor colorGrid;
    QColor colorSignal[3];     
    QColor colorCurs[3];       
    QColor colorCursSpectr[2];
    QColor colorTShift;
    QColor colorTShift0;

    void SetPosUCur(int channel, int numCur, int value);
    double GetPosUCurAbs(int channel, int numCur);
    int GetPosUCur(int channel, int numCur);

    int GetPosUCurInScreen(int channel, int numCur);             // �������� ���������� ������� ������� �� ������ - ����� ������������� 0, ���� - height() - 1

    void SetPosTCur(int channel, int numCur, int value);
    double GetPosTCurAbs(int channel, int numCur);
    int GetPosTCur(int channel, int numCur);

    int GetPosTCurInScreen(int channel, int numCur);

private:
    int posCursU[4][3];         // ������� �������� ����������. � ������ ����� 0, ����� ��������� ����� ������� ��������
    int posCursT[4][3];         // ������� �������� �������. � ������ ����� 0, ����� ��������� ����� ������� ��������
    int activeCurs;
};