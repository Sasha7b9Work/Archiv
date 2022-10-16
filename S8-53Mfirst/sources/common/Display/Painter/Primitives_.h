// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


inline void BoundingX(int &x) { if (x < 0) x = 0; if (x >= Display::WIDTH) x = Display::WIDTH - 1; }
inline void BoundingY(int &y) { if (y < 0) y = 0; if (y >= Display::HEIGHT) y = Display::HEIGHT - 1; }


class Point
{
public:
    void Draw(int x, int y, const Color &color) const;
    void Draw(int x, int y) const;
};


class Region
{
public:
    Region(int w, int h) : width(w), height(h) {}
    void FillBounded(int x, int y, const Color &fill, const Color &bounded);
    void Fill(int x, int y, const Color &color) const;
    void Fill(int x, int y) const;
    int Width() const  { return width;  };
    int Height() const { return height; }

private:
    int width;
    int height;
};


class Rectangle
{
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    void Draw(int x, int y, const Color &color) const;
    void Draw(int x, int y) const;
private:
    int width;
    int height;
};


class HLine
{
public:
    HLine(int w = 0) : width(w) {}
    void Draw(int y, int x1, int x2, const Color &color) const;
    void Draw(int y, int x1, int x2) const;
    void Draw(int x, int y, const Color &color) const;
    void Draw(int x, int y) const;
private:
    int width;
};


class VLine
{
public:
    VLine() : height(0) {}
    VLine(int h) : height(h) {};
    void Draw(int x, int y0, int y1, const Color &color) const;
    void Draw(int x, int y0, int y1) const;
    void Draw(int x, int y, const Color &color) const;
    void Draw(int x, int y) const;
private:
    int height;
};


// numLines ������������ �����, ��������� �� count ����� ������ � ����������� ����� ������� delta.
//  �������������� ���������� ������ ����� ������ ����� ������������� ���������� �������� ������� x[]
class MultiVPointLine
{
public:
    MultiVPointLine(int _numLines, uint16 *_x0, int _delta, int _count) :
        numLines(_numLines), x0(_x0), delta(_delta), count(_count)
    {
    }
    void Draw(int y0) const;
private:
    int numLines;
    uint16 *x0;
    int delta;
    int count;
    // ���������� ���� ������������ ������ �� count ����� c ����������� delta ����� ���������� �������
    static void DrawVPointLine(int x, int y, int count, int delta);
};


// \brief ���������� numLines �������������� �����, ��������� �� count ����� ������ � ����������� ����� ������� delta.
// ������������ ���������� ������ ����� ������ ����� ������������� ���������� �������� ������� y[]
class MultiHPointLine
{
public:
    MultiHPointLine(int _numLines, const uint8 *_y, int _delta, int _count) : numLines(_numLines), y(_y), delta(_delta),
        count(_count) {}
    void Draw(int x) const;
private:
    int numLines;
    const uint8 *y;
    int delta;
    int count;
    // ���������� ���� �������������� ������ �� count ����� c ����������� delta ����� ���������� �������
    static void DrawHPointLine(int x, int y, int count, int delta);
};


class VPointLine
{
public:
    VPointLine(float d) : delta(d) {}
    void Draw(int x, int y0, int y1) const;
private:
    float delta;
};


class HPointLine
{
public:
    HPointLine(float d) : delta(d) {}
    void Draw(int y, int x0, int x1) const;
private:
    float delta;
};


class Line
{
public:
    void Draw(int x0, int y0, int x1, int y1, const Color &color) const;
    void Draw(int x0, int y0, int x1, int y1) const;
};


// ������ ����������� �������������� �����. dFill - ����� ������, dEmpty - �����. ����� ��������. ����� ������
//  ���������� �� ������. dStart ��������� �������� ������ �������� ����� ������������ ������ ������.
class DashedHLine
{
public:
    DashedHLine(int dFill, int dEmpty) : fill(dFill), empty(dEmpty) {}
    void Draw(int y, int x0, int x1, int dStart) const;
private:
    int fill;
    int empty;
};


//  ������ ����������� ������������ �����.
class DashedVLine
{
public:
    DashedVLine(int dFill, int dEmpty) : fill(dFill), empty(dEmpty) {}
    void Draw(int x, int y0, int y1, int dStart) const;
private:
    int fill;
    int empty;
};


// ���������� ������ ������������ �����. ����� �������� ���� �� ������. y0y1 - ������ ������������ ���������.
class VLineArray
{
public:
    void Draw(int x, int numLines, uint8 *y0y1, const Color &color) const;
private:
};
