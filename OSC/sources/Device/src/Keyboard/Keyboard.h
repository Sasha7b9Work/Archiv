#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Key
{
    enum E
    {
        None = 0,
        Function = 1,
        Measure = 2,
        Memory = 3,
        Service = 4,
        ChannelA = 5,
        ChannelB = 6,
        Time = 7,
        Start = 8,
        Trig = 9,
        Display = 10,
        RangeMoreA = 11,
        RangeLessA = 12,
        RShiftMoreA = 13,
        RShiftLessA = 14,
        RangeMoreB = 15,
        RangeLessB = 16,
        RShiftMoreB = 17,
        RShiftLessB = 18,
        TBaseMore = 19,
        TBaseLess = 20,
        TShiftMore = 21,
        TShiftLess = 22,
        TrigLevMore = 23,
        TrigLevLess = 24,
        Left = 25,
        Right = 26,
        Up = 27,
        Down = 28,
        Enter = 29,
        F1 = 30,
        F2 = 31,
        F3 = 32,
        F4 = 33,
        F5 = 34,
        Number
    } value;

    Key(E v = None) : value(v) {};
    bool IsFunctional() const { return value >= F1 && value <= F5; };
    operator uint8() const { return (uint8)value; };
    pString Name();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypePress
{
    enum E
    {
        Press,      ///< ������� ������
        Repeat,     ///< �������������� ������
        Release,    ///< ���������� ������
        Long,       ///< "�������" ������� ������
        None
    } value;

    TypePress(E v = None) : value(v)  { };
    operator uint8() const { return (uint8)value; };
    pString ToString();
    bool IsLong() const { return value == Long; };
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyEvent
{
    KeyEvent(Key::E k = Key::None, TypePress::E t = TypePress::None) : key(k), type(t)
    {
    };
    Key key;
    TypePress type;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:
    /// �������� ������ ����������
    /** ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton */
    static void Disable();
    /// �������� ������� �������
    static Key WaitPressingButton();

    static void Enable();
};
