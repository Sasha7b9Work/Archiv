// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7


struct DataTime
{
    int8 *curField;       // ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����,
                          //6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
};


// ������������� � ���������� �����.
class TimeItem : public Item
{
public:
    TimeItem(const DataItem *const data) : Item(data) {};

    const DataTime *OwnData() const {
        return (const DataTime *)(data->ad);
    }

    virtual void Draw(int x, int y, bool opened = false) const;

    void SetOpened();

    void SetNewTime() const;

    void SelectNextPosition();

    void IncCurrentPosition() const;

    void DecCurrentPosition() const;

    void DrawClosed(int x, int y) const;

    void DrawOpened(int x, int y) const;

    virtual void ShortPress();

    virtual void LongPress();
};

