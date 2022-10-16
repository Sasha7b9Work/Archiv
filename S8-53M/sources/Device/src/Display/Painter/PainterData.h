// 2021/03/17 13:26:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Settings/SettingsTypes_.h"
#include "common/Utils/Containers/Buffer_.h"
#include "Data/InputStorage/ReadingFrame.h"


namespace PainterData
{
    void DrawData();
};


// ����� ���������. ������� ��������� ��� ��������� ������ - ������������������ � ������������
struct DrawingFrame
{
    DrawingFrame() {};
    DrawingFrame(const Buffer<uint8> &);

    void PrepareForDrawing();
    void Draw();

    ReadingFrame data;

    void DrawChannel(const Chan &ch);

private:

    Buffer<uint8> points[Chan::Count]; // ����� ������������ �����, ������� � ������ �� �����

    void PrepareChannel(const Chan &ch);

    void DrawChannelPointed(int x, Buffer<uint8> &buffer);
    void DrawChannelLined(int x, Buffer<uint8> &buffer);
};


/*
    1. ������ ���������:
        - ���������� �����;
        - ���������� � �������������;
        - ���������� � ����������;
        - ����������;
        - ����� ���/����.

    2. ��� �����������:

        2.1 ���������� ������ ����� (�. 3).
        2.2 ���������� ������ ����� (�. 3).

    3. ��������� ������:

        3.1 ���� ����� �� ��������� � ��������� - �����.
        3.2 ����������� ������ ��� ��������� (�. 4).
        3.3 ���������� (�. 5).

    4. ���������� ������ ��� ���������:

        4.1 ����� ������� ������ � ��������� �������������� �����.
        4.2 ��������� ������ ������������ ������� ����������.
        4.3 ����������� �������� � �������� ����������.

    5. ��������� ������

        5.1 ���� ��������� ������� - ���� ������� ������ �����.
        5.2 ���� ��������� ������� - ���������� ������ ����� � ������� �����.

*/
