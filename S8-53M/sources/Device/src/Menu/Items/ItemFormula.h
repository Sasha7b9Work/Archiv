// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3


struct DataFormula
{
    int8 *function;      // ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8 *koeff1add;     // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff2add;     // ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff1mul;     // ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff2mul;     // ����� ������������ ��� ������ ����� ��� ���������
    int8 *curDigit;      // ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����,
                            //  2 - ���� ������� �����, 3 - ����������� ������� �����
    pFuncVV funcOfChanged;  // ��� ������� ���������� ����� ��������� ��������� �������� ����������.
};


class Formula : public Item
{
public:
    Formula(const DataItem *const data) : Item(data) {};
    const DataFormula *OwnData() const {
        return reinterpret_cast<const DataFormula *>(data->ad);
    }
    virtual void Draw(int x, int y, bool opened = false) const;
private:
    void DrawClosed(int x, int y) const;
    void DrawLowPart(int x, int y) const;
    void WriteText(int x, int y, bool) const;
};
