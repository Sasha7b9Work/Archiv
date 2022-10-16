// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TypeLED
{
    enum E
    {
        // ���/���� ������������ ���: ���� ����, �� ���������� ��� ��������, ���� ��������, �� ��������. ������� ���
        _None,
        Trig,   // 1 / 128 + 1
        RegSet, // 2 / 128 + 2
        ChanA,  // 3 / 128 + 3
        ChanB,  // 4 / 128 + 4
        Power   // 5
    };
};


struct Key { enum E
{
        None,       // 
        F1,         // 1
        F2,         // 2
        F3,         // 3
        F4,         // 4
        F5,         // 5
        ChannelA,   // ����� 1
        ChannelB,   // ����� 2
        Time,       // ����
        Trig,       // �����
        Cursors,    // �������
        Measures,   // �����
        Display,    // �������
        Help,       // ������
        Start,      // ����/����
        Memory,     // ������
        Service,    // ������
        Menu,       // ����
        Power,      // ����
        RangeA,     // �����/��� 1
        RangeB,     // �����/��� 2
        RShiftA,    // 
        RShiftB,    // 
        TBase,      // �����/���
        TShift,     // 
        TrigLev,    // �������
        Setting,    // ���������
        Count
    };

    Key(E v) : value(v) { }

    pstring Name() const;

    E value;

    static Key::E FromCode(uint16 code);
    static uint16 ToCode(Key::E key);
    static bool IsGovernor(Key::E key);
    static bool Is(Key::E key);
    static bool IsFunctional(Key::E key);
};

Key::E& operator++(Key::E &right);


struct Action { enum E {
        Down,         // ������� ������
        Up,           // ���������� ������
        Long,         // "�������" �������
        RotateLeft,   // �������� ����� �����
        RotateRight,  // ������� ����� ������
        Count
    } value;

    Action(uint8 v) : value((E)v) {}
    static Action::E FromCode(uint16 code);
    static uint16 ToCode(Action::E action);
    bool IsDown() const { return value == Down; }
    bool IsUp() const { return value == Up; }
    bool IsLong() const { return value == Long; }
    bool IsRotateLeft() const { return value == RotateLeft; }
    bool IsRotateRight() const { return value == RotateRight; }
    pchar Name() const;
};


struct KeyboardEvent
{
    KeyboardEvent(Key::E k = Key::Count, Action::E a = Action::Count) :key(k), action(a) {}

    Key::E key;
    Action::E action;

    bool Is(Key::E c) const           { return (c == key); };
    bool Is(Key::E c, Action::E a) const { return (key == c) && (action == a); };
       
    bool IsUp() const   { return (action == Action::Up); }
    bool IsDown() const { return (action == Action::Down); }
    bool IsLong() const { return (action == Action::Long); }
    // ���������� true � ������ ���������� ������ ��� "��������" �������
    bool IsRelease() const;

    // true, ���� �������������� �������
    bool IsFunctional() const;

    bool IsRotate() const { return (action == Action::RotateRight) || (action == Action::RotateLeft); }
    
    bool operator==(const KeyboardEvent &rhl) const
    {
        return (rhl.key == key) && (rhl.action == action);
    }
};
