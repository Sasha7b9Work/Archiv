// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LEDS.h"
#include "HAL.h"
#include "Keyboard.h"
#include <limits>


struct Buffer
{
    static void AppendEvent(Key::E key, Action::E action);
    static KeyboardEvent commands[10];
    static int pointer;
    static bool IsEmpty() { return (pointer == 0); }
    static KeyboardEvent GetNextEvent()
    {
        KeyboardEvent event;
    
        if (IsEmpty())
        {
            event.key = Key::None;
        }
        else
        {
            event = commands[0];
            for (int i = 1; i < pointer; i++)
            {
                commands[i - 1] = commands[i];
            }
            --pointer;
        }
    
        return event;
    }
};

KeyboardEvent Buffer::commands[10];
int Buffer::pointer = 0;


#define NUM_SL 6
#define NUM_RL 8    


struct KeyStruct
{
    KeyStruct(Key::E k) : key(k), timePress(0U), happendLongPressed(false) { }

    bool IsValid() const { return (key != Key::None); }

    void Process(uint time, bool pressed);

    Key::E key;
    uint   timePress;               // ����� ������� ������. ���� 0 - ������ �� ������
    bool   happendLongPressed;      // ���� true, �� ��������� ������� ������� ������
};


static KeyStruct keys[NUM_SL][NUM_RL] =     
//    RL0            RL1            RL2           RL3           RL4          RL5           RL6           RL7
    {{Key::ChannelA, Key::None,     Key::None,    Key::RangeA,  Key::None,   Key::None,    Key::RShiftA, Key::Menu},  // SL0
     {Key::ChannelB, Key::None,     Key::None,    Key::RangeB,  Key::None,   Key::None,    Key::RShiftB, Key::F1},    // SL1
     {Key::Time,     Key::None,     Key::None,    Key::TBase,   Key::None,   Key::None,    Key::TShift,  Key::F2},    // SL2
     {Key::Trig,     Key::None,     Key::None,    Key::TrigLev, Key::Start,  Key::None,    Key::None,    Key::F3},    // SL3
     {Key::Cursors,  Key::Measures, Key::Display, Key::Help,    Key::Memory, Key::Service, Key::None,    Key::F4},    // SL4
     {Key::Power,    Key::None,     Key::None,    Key::Setting, Key::None,   Key::None,    Key::None,    Key::F5}};   // SL5

static Pin *sls[NUM_SL] = { &pinSL0, &pinSL1, &pinSL2, &pinSL3, &pinSL4, &pinSL5 };
static Pin *rls[NUM_RL] = { &pinRL0, &pinRL1, &pinRL2, &pinRL3, &pinRL4, &pinRL5, &pinRL6, &pinRL7 };

#define SET_SL(n)       sls[n]->On();
#define RESET_SL(n)     sls[n]->Off();
#define READ_RL(n)      rls[n]->Read()


struct GovernorStruct
{
    GovernorStruct(Key::E k, uint8 rlA_, uint8 rlB_, uint8 sl_) :
        key(k), rlA(rlA_), rlB(rlB_), sl(sl_), prevStateIsSame(false), prevState(false), nextTime(0) { }
    void Process();

private:
    void SendEvent(Key::E key, Action::E action);

    Key::E key;
    uint8  rlA;
    uint8  rlB;
    uint8  sl;
    bool   prevStateIsSame;      // true, ���� ���������� ��������� ���������
    bool   prevState;            // � ���� ��������� ��� ������� ���������� � ������� ���������� ���
    uint   nextTime;             // �����, ��������� ���������. ������������ ��� ���������� ��������
    static const int dT = 10;
};

#define NUM_GOVERNORS 8

static GovernorStruct governors[NUM_GOVERNORS] =
{
    GovernorStruct(Key::RangeA,  1, 2, 0),
    GovernorStruct(Key::RangeB,  1, 2, 1),
    GovernorStruct(Key::RShiftA, 4, 5, 0),
    GovernorStruct(Key::RShiftB, 4, 5, 1),
    GovernorStruct(Key::TBase,   1, 2, 2),
    GovernorStruct(Key::TShift,  4, 5, 2),
    GovernorStruct(Key::TrigLev, 1, 2, 3),
    GovernorStruct(Key::Setting, 1, 2, 5)
};


void Keyboard::Init()
{
    for (int i = 0; i < NUM_SL; i++)
    {
        SET_SL(i);
    }

    do 
    {
        Update();
        Buffer::GetNextEvent();

    } while (!Buffer::IsEmpty());

    LED::DisableAll();
}


void Keyboard::Update()
{
    uint time = TIME_MS;

    for (int sl = 0; sl < NUM_SL; sl++)
    {
        RESET_SL(sl);

        for (int rl = 0; rl < NUM_RL; rl++)
        {
            keys[sl][rl].Process(time, (READ_RL(rl) == 0));
        }

        SET_SL(sl);
    }

    for (int i = 0; i < NUM_GOVERNORS; i++)
    {
        governors[i].Process();
    }
}


void KeyStruct::Process(uint time, bool pressed)
{
    if (IsValid())
    {
        if (timePress != 0 && !happendLongPressed)          // ���� ������� ��������� � ������� ���������
        {
            uint delta = time - timePress;
            if (delta > 500)                                // ���� ������ ����� 500 �� � ������� ������� -
            {
                happendLongPressed = true;
                Buffer::AppendEvent(key, Action::Long);     // ��� ����� ������� �������
            }
            else if (delta > 100 &&                         // ���� ������ ����� 100 �� � ������� �������
                !pressed)                                   // � ������ ������ ��������� � ������� ���������
            {
                timePress = 0;                              // �� ��������� ��� � �������
                Buffer::AppendEvent(key, Action::Up);       // � ��������� ���������� ������ � ������ ������
            }
        }
        else if (pressed && !happendLongPressed)            // ���� ������ ������
        {
            timePress = time;                               // �� ��������� ����� � �������
            Buffer::AppendEvent(key, Action::Down);
        }
        else if (!pressed && happendLongPressed)
        {
            timePress = 0;
            happendLongPressed = false;
        }
    }
}


void GovernorStruct::Process()
{
    if (TIME_MS < nextTime)
    {
        return;
    }

    RESET_SL(sl);

    bool stateA = (READ_RL(rlA) != 0);
    bool stateB = (READ_RL(rlB) != 0);

    SET_SL(sl);

    if (stateA == stateB)
    {
        prevStateIsSame = true;
        prevState = stateA;
    }
    else if (prevStateIsSame && stateA && !stateB)
    {
        SendEvent(key, prevState ? Action::RotateLeft : Action::RotateRight);
    }
    else if (prevStateIsSame && !stateA && stateB)
    {
        SendEvent(key, prevState ? Action::RotateRight : Action::RotateLeft);
    }
}


void GovernorStruct::SendEvent(Key::E key_, Action::E action)
{
    Buffer::AppendEvent(key_, action);
    prevStateIsSame = false;
    nextTime = TIME_MS + dT;
}


void Buffer::AppendEvent(Key::E key, Action::E action)
{
    if (key == Key::Power && action != Action::Down)
    {
        return;
    }

    commands[pointer].key = key;
    commands[pointer].action = action;
    pointer++;
}


bool Keyboard::Buffer::IsEmpty()
{
    return ::Buffer::IsEmpty();
}


int Keyboard::Buffer::NumEvents()
{
    return ::Buffer::pointer;
}


KeyboardEvent Keyboard::Buffer::GetNextEvent()
{
    return ::Buffer::GetNextEvent();
}
