//  [6/24/2009 Sasha]
#pragma once
#include <QString>

class MathDisplayRegistrator 
{
public:
    MathDisplayRegistrator();
    ~MathDisplayRegistrator();

    // ���������� �������� ����������, ��������������� ������� ������� ������ ��� ������� �������� �� ����������. ������ ������� ������������� -GetMaxVoltage();
    double GetMaxVoltage(int range);

    // ���������� �������� �������, ��������������� ������ ������� ������ ��� ������� �������� �� �������. ����� ������� ������������� -GetMaxTime(). ���� �����,
    // ������������ �������� ����������� � �������������, ��� TPos == center � tShift == 0.
    double GetMaxTime(int tBase);

    // ���������� ���������� �������� �������� �� ���������� � ��������� ���� � ������� ��� ������ ����
    QString GetRShiftAbs(const int rShift, const int range);

    // ���������� ������������� �������� �������� �� ���������� ��� ������� � ������ �����
    // ��������� �������� �������� �� ����������
    int GetRShiftRel(const int rShiftPrev, const int prevRange, const int currentRange);

    // ���������� ���������� �������� �������� �� ������� � ��������� ���� � ������� ��� ������ ����
    QString GetTShiftAbs(const int tShift, const int tBase);

    // ���������� ������������� �������� �������� �� ������� ��� ������� � ������ �����
    // ��������� �������� �������� �� �������
    int GetTShiftRel(const int tShiftPrev, const int prevTBase, const int currentTBase);

    /*
    �������
    */

    // ���������� ������� ������� ���������� � ��������� ����
    QString GetPosUCur(int pos, int range);

    // ���������� ���������� �������� ������� ���������� ��� ����� ��������
    double GetPosUCurAbs(int pos, int range);

    // ���������� ������������� �������� ������� ���������� �� �����������
    int GetPosUCurRel(double pos, int range);

    // ���������� �������� ����� ��������� �������� � ��������� ����
    QString GetDeltaU(int pos1, int pos2, int range);

    // ���������� ������� ������� ����� ��������� �������� �� ����������
    int GetPosUCurNew(int prevPos, int prevRange, int currentRange);

    // ���������� ������� ������� ������� � ���������� ����
    //QString GetPosTCur(int pos, int tBase, int tShift, TPos tPos = TPos_Center);

    // ���������� ���������� �������� ������� ������� ��� ����� ��������
    double GetPosTCurAbs(int pos, int tBase);

    // ���������� ������������� �������� ������� ������� �� �����������
    int GetPosTCurRel(double pos, int tBase);

    // ���������� �������� ����� ��������� �������� � ���������� ����
    QString GetDeltaT(int pos1, int pos2, int tBase);

    // ���������� ������� ������� ����� ��������� �������� �� �������
    int GetPosTCurNew(int prevPos, int prevTBase, int currentTBase);
};