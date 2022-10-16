// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataItem
{
    TypeItem::E  type;          // ��� �����
    const Page  *keeper;        // ���� �������� ����������� ����
    pFuncBV      funcOfActive;  // ���� ������� ���������� true, �� ���� �������
    pchar       *titleHint;     // ������ � ���������� � ����������� ��� �����
    const void  *ad;            // ��������� �� ������������� ��� ������� ���� ����� ������
};


class Item
{
public:

    static const int TITLE_WIDTH = 79;
    static const int TITLE_HEIGHT = 34;
    static const int HEIGHT = 33;
    static const int WIDTH = TITLE_WIDTH + 1;
    static const int HEIGHT_VALUE = 13;
    static const int OPENED_HEIGHT = 14;
    static const int OPENED_WIDTH = TITLE_WIDTH;
    static const int OPENED_HEIGHT_TITLE = 19;
    static const int WIDTH_VALUE = WIDTH - 4;

    const DataItem *data;

    Item(const DataItem *const data = nullptr);

    // ���������� true, ���� ������, ��������������� ������� �������� ����, ��������� � ������� ���������.
    bool IsPressed() const;

    // ���������� true, ���� ������� ���� item �������� �������, �.�. ����� ���� �����.
    bool IsActive() const;

    // ���������� true, ���� ������� �������� ������� - �������� ����� ��������� �� ����
    bool IsCurrent() const;

    // ���������� true, ���� ����� ���� ���������� ������� (� �������� ���������, �������� ����. � �������� ����� ����
    // �������� �������)
    bool IsMayCurrent() const;

    TypeItem::E Type() const;

    // ���������� ����� ��������, �������� ����������� ������� �� ������ item.
    Page *Keeper() const;

    // �������
    void Open() const;

    // �������
    void Close() const;

    // �������� "����������"
    void ChangeOpenness() const;

    // �������/��������� ������� ����� ��������.
    void SetCurrent(bool active) const;

    // ���������� true, ���� ������� ���� �� ������ item ������.
    bool IsOpened() const;

    int OpenedPosY() const;

    // ��������� ��� ����������� �������� Governor, GovernorColor ��� Choice �� ������ item � ����������� �� ����� delta
    void Change(int delta) const;

    virtual void OnRegSet(int) const {};

    // ���������� ������ � �������� ��������� �������� Choice ��� NamePage::E �� ������ item.
    int HeightOpened() const;

    // ���������� �������� �������� �� ������ item, ��� ��� �������� �� ������� �������.
    pstring Title() const;

    // �������� "��������" ������. ����� �������� �� ���� ����, ��� ����� � ������� ������ ����� �������, � ������ �
    // ������ ����� �����
    // normal - ���� �������� ������������� �������, brigtht - ���� ����� � ������, dark - ���� ����� � ������
    void DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark,
        bool isPressed, bool isActive) const;

    // ���������� �������� ������� ����
    static const Item *Opened();

    bool IsPage() const          { return data->type == TypeItem::Page; }
    bool IsChoice() const        { return data->type == TypeItem::Choice; }
    bool IsChoiceReg() const     { return data->type == TypeItem::ChoiceReg; }
    bool IsGovernor() const      { return data->type == TypeItem::Governor; }
    bool IsGovernorColor() const { return data->type == TypeItem::GovernorColor; }
    bool IsButton() const        { return data->type == TypeItem::Button; }
    bool IsSmallButton() const   { return data->type == TypeItem::SmallButton; }
    bool IsIP() const            { return data->type == TypeItem::IP; }
    bool IsMAC() const           { return data->type == TypeItem::MAC; }
    bool IsTime() const          { return data->type == TypeItem::Time; }

    const TimeItem *ReinterpretToTime()     const { return (TimeItem *)this; }
    const Governor *ReinterpretToGovernor() const { return (Governor *)this; }
    const Choice *ReinterpretToChoice()     const { return (Choice *)this; }
    const Page *ReinterpretToPage()         const { return (Page *)this; }

    static DataItem emptyData;

    static Item empty;

    // ��������� ��������� �������
    virtual void ShortPress() {};

    // ��������� �������� ������� �� ������� ���� item.
    virtual void LongPress();

    virtual void Draw(int, int, bool = false) const {};

    // ���������� ������� ����� �� ��������
    int8 GetPosition() const;

protected:
    static int8 gCurDigit;
};


struct GraphicGovernor
{
    static const int NUM_STATES = 4;       // ���������� ��������� ����� ���������

    static void Draw(const Item *item, int x, int y);

    // ������������ ������� �����, ������� ���� ����������
    static int CalculatePosition(const Item *item);
};
