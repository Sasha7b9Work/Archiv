// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ������ �����
struct Holder
{
    static void RotateGovernor(int delta);
    static void RotateSwitch(int delta);
    static void Draw(int x, int y, const Color &color);
    static const char chars[4];     // ������, ��������������� ������� �� ������ ��������� ����� ��������
    static int angle;               // ���� �������� �� 0 �� 3
private:
    static void Rotate(int delta);
};



struct DataGovernor
{
    int16  *cell;
    int     minValue;           // ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;           // ������������ ��������.
    pFuncVV funcOfChanged;      // �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
};


// ��������� ���������.
class Governor : public Item
{
public:

    Governor(const DataItem *const data) : Item(data), inStateIncrease(false), inStateDecrease(false), timeStartMS(0) {};
    const DataGovernor *OwnData() const {
        return (const DataGovernor *)(data->ad);
    }
    // ��������� ��������� ������� �� ������� Governor � ������� governor.
    virtual void ShortPress();
    void StartChange(int detla);            // ��������� �������� �������� ���������� ��� ���������� �������� ����
                                            // ���� Governor (� ����������� �� ����� delta)
    float Step() const;                     // ������������ ��������� ���� ��������.
    virtual void OnRegSet(int delta) const; // �������� �������� � ������� ������� ��� ��������� ��������
    int16 NextValue() const;                // ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;                // ���������� ��������� ������� ��������, ������� ����� ������� governor.

    virtual void Draw(int x, int y, bool opened = false) const;

private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;

    // ���������� ����� ��������� � ���� ��� ����� �������� governor.
    // ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;

    // ��� �������� �������� ������������ ������ �� ��������� �������
    void NextPosition();

    void DrawLowPart(int x, int y) const;

    bool inStateIncrease;
    bool inStateDecrease;
    uint timeStartMS;
};
