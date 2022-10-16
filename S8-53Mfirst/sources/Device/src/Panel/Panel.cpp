// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Mutex_.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Menu/PasswordResolver.h"
#include "Menu/Pages/Definition.h"
#include "Menu/Pages/HelpContent.h"
#include "Panel/Panel.h"
#include "Settings/Settings.h"
#include <cstring>


extern LED       led_Trig(TypeLED::Trig);
extern RegSetLED led_RegSet(TypeLED::RegSet);
extern LED       led_ChA(TypeLED::ChanA);
extern LED       led_ChB(TypeLED::ChanB);
extern LED       led_Power(TypeLED::Power);


namespace Panel
{
    static void F1Long();
    static void F2Long();
    static void F3Long();
    static void F4Long();
    static void F5Long();
    static void ChannelLongA();
    static void ChannelLongB();
    static void TimeLong();
    static void HelpLong();
    static void MenuLong();
    static void TrigLong();

    static void StartDown();
    static void PowerDown();

    static void RShiftLeftA();
    static void RShiftRightA();
    static void RShiftLeftB();
    static void RShiftRightB();
    static void TrigLevLeft();
    static void TrigLevRight();
    static void TShiftLeft();
    static void TShiftRight();
    static void RangeLeftA();
    static void RangeRightA();
    static void RangeLeftB();
    static void RangeRightB();
    static void TBaseLeft();
    static void TBaseRight();
    static void SetLeft();
    static void SetRight();

    static void RShiftDownA();
    static void RShiftDownB();
    static void SetDown();
    static void TShiftDown();

    static void CopyData();
    static void ProcessEvent();
    inline void E() {};
    static bool CanChangeRShiftOrTrigLev(TrigSource::E, int16 rShift);
    static void ChangeRShift(int *prevTime, void(*f)(const Chan &, int16), const Chan &, int16 relStep);
    static void ChangeTrigLev(int *prevTime, void(*f)(TrigSource::E, int16), TrigSource::E, int16 relStep);
    static bool CanChangeTShift(int16 tShift);
    static void ChangeTShift(int *prevTime, void(*f)(int), int16 relStep);
    static void XShift(int delta);
    static int CalculateCount(int *prevTime);
    static void ChangeShiftScreen(int *prevTime, void(*f)(int), int16 relStep);

    bool isRunning = true;
    uint timeLastEvent = 0;
    static Queue<uint8> data_for_send;
    static const uint MIN_TIME = 500;
    static Key::E pressedKey = Key::None;
    static Queue<uint8> queue;                  // А здесь идёт обработка принятных данных


    struct ReceivedBuffer
    {
        ReceivedBuffer() : pointer(0) { std::memset(buffer, 0, SIZE); }

        void Push(uint8 byte)
        {
            if (pointer < SIZE) { buffer[pointer++] = byte; }
        }

        int Size() const { return pointer; }

        void Clear() { pointer = 0; }

        uint8 &operator[](int n)
        {
            if (n > (pointer - 1)) { return buffer[0]; }

            return buffer[n];
        }

        uint8 *Data() { return buffer; }

        Mutex mutex;    // При работе с буфером главный цикл лочит этот мьютекс, а прерывание проверяет, не залочен ли он

    private:

        static const int SIZE = 20;
        uint8 buffer[SIZE];
        int pointer;
    };

    static ReceivedBuffer received_buffer;      // Прерывание от панели сюда складывает принятые данные

    static const pFuncVV funcOnKeyDown[Key::Count] =
    {
        E,              // Key::None
        E,              // Key::F1
        E,              // Key::F2
        E,              // Key::F3
        E,              // Key::F4
        E,              // Key::F5
        E,              // Key::ChannelA
        E,              // Key::ChannelB
        E,              // Key::Time
        E,              // Key::Trig
        E,              // Key::Cursors
        E,              // Key::Measures
        E,              // Key::Display
        E,              // Key::Help
        StartDown,      // Key::Start
        E,              // Key::Memory                                         
        E,              // Key::Service
        E,              // Key::Menu
        PowerDown,      // Key::Power
        E,              // Key::RangeA
        E,              // Key::RangeB
        RShiftDownA,    // Key::RShiftA
        RShiftDownB,    // Key::RShiftB
        E,              // Key::TBase
        TShiftDown,     // Key::TShift
        E,              // Key::TrigLev
        SetDown         // Key::Setting
    };

    static const pFuncVV funcOnKeyUp[Key::Count] =
    {
        E,              // Key::None
        E,              // Key::F1
        E,              // Key::F2
        E,              // Key::F3
        E,              // Key::F4
        E,              // Key::F5
        E,              // Key::ChannelA
        E,              // Key::ChannelB
        E,              // Key::Time
        E,              // Key::Trig
        E,              // Key::Cursors
        E,              // Key::Measures
        E,              // Key::Display
        E,              // Key::Help
        E,              // Key::Start
        E,              // Key::Memory
        E,              // Key::Service
        E,              // Key::Menu
        E,              // Key::Power
        E,              // Key::RangeA
        E,              // Key::RangeB
        E,              // Key::RShiftA
        E,              // Key::RShiftB
        E,              // Key::TBase
        E,              // Key::TShift
        E,              // Key::TrigLev
        E               // Key::Setting
    };

    static const pFuncVV funcOnLongPressure[Key::Count] =
    {
        E,              // Key::None
        F1Long,         // Key::F1
        F2Long,         // Key::F2
        F3Long,         // Key::F3
        F4Long,         // Key::F4
        F5Long,         // Key::F5
        ChannelLongA,   // Key::ChannelA
        ChannelLongB,   // Key::ChannelB
        TimeLong,       // Key::Time
        TrigLong,       // Key::Trig
        E,              // Key::Cursors
        E,              // Key::Measures
        E,              // Key::Display
        HelpLong,       // Key::Help
        E,              // Key::Start
        E,              // Key::Memory
        E,              // Key::Service
        MenuLong,       // Key::Menu
        E,              // Key::Power
        E,              // Key::RangeA
        E,              // Key::RangeB
        E,              // Key::RShiftA
        E,              // Key::RShiftB
        E,              // Key::TBase
        E,              // Key::TShift
        E,              // Key::TrigLev
        E               // Key::Setting
    };

    static const pFuncVV funculatorLeft[Key::Count] =
    {
        E,              // Key::None
        E,              // Key::F1
        E,              // Key::F2
        E,              // Key::F3
        E,              // Key::F4
        E,              // Key::F5
        E,              // Key::ChannelA
        E,              // Key::ChannelB
        E,              // Key::Time
        E,              // Key::Trig
        E,              // Key::Cursors
        E,              // Key::Measures
        E,              // Key::Display
        E,              // Key::Help
        E,              // Key::Start
        E,              // Key::Memory
        E,              // Key::Service
        E,              // Key::Menu
        E,              // Key::Power
        RangeLeftA,     // Key::RangeA
        RangeLeftB,     // Key::RangeB
        RShiftLeftA,    // Key::RShiftA
        RShiftLeftB,    // Key::RShiftB
        TBaseLeft,      // Key::TBase
        TShiftLeft,     // Key::TShift
        TrigLevLeft,    // Key::TrigLev
        SetLeft         // Key::Setting
    };
    static const pFuncVV funculatorRight[Key::Count] =
    {
        E,              // Key::None
        E,              // Key::F1
        E,              // Key::F2
        E,              // Key::F3
        E,              // Key::F4
        E,              // Key::F5
        E,              // Key::ChannelA
        E,              // Key::ChannelB
        E,              // Key::Time
        E,              // Key::Trig
        E,              // Key::Cursors
        E,              // Key::Measures
        E,              // Key::Display
        E,              // Key::Help
        E,              // Key::Start
        E,              // Key::Memory
        E,              // Key::Service
        E,              // Key::Menu
        E,              // Key::Power
        RangeRightA,    // Key::RangeA
        RangeRightB,    // Key::RangeB
        RShiftRightA,   // Key::RShiftA
        RShiftRightB,   // Key::RShiftB
        TBaseRight,     // Key::TBase
        TShiftRight,    // Key::TShift
        TrigLevRight,   // Key::TrigLev
        SetRight        // Key::Setting
    };
}


void Panel::CopyData()
{
    received_buffer.mutex.Lock();

    for (int i = 0; i < received_buffer.Size(); i++)
    {
        queue.Push(received_buffer[i]);
    }

    received_buffer.Clear();

    received_buffer.mutex.Unlock();
}


void Panel::Update()
{
    CopyData();

    while (queue.Size() != 0 && queue[0] != 0xFF)
    {
        queue.Front();
    }

    while (queue.Size() >= 3)
    {
        ProcessEvent();
    }
}


void Panel::ProcessEvent()
{
    queue.Front();

    Key::E key = (Key::E)queue.Front();
    Action action(queue.Front());

    if (!isRunning)
    {
        pressedKey = key;
        return;
    }

    PasswordResolver::ProcessEvent(KeyboardEvent(key, action.value));

    timeLastEvent = TIMER_MS;

    if (action.IsUp())
    {
        Menu::Handlers::ReleaseKey(key);
        funcOnKeyUp[key]();

        if (pressedKey != Key::None)
        {
            Menu::Handlers::ShortPressureKey(key);
            pressedKey = Key::None;
        }
    }
    else if (action.IsDown())
    {
        funcOnKeyDown[key]();
        Menu::Handlers::PressKey(key);
        pressedKey = key;
    }
    else if (action.IsLong())
    {
        funcOnLongPressure[key]();
        pressedKey = Key::None;
    }
    else if (action.IsRotateLeft())
    {
        funculatorLeft[key]();
    }
    else if (action.IsRotateRight())
    {
        funculatorRight[key]();
    }
}


void Panel::CallbackOnReceiveSPI5(const uint8 *data, uint size)
{
    if (data[1] == Key::None)
    {
        return;
    }

    static ReceivedBuffer buffer;

    for (uint i = 0; i < size; i++)              // Сначала сохраняем данные в промежуточный буфер
    {
        buffer.Push(data[i]);
    }

    if (!received_buffer.mutex.IsLocked())      // А затем, если основной буфер не заблокирован
    {
        for (int i = 0; i < buffer.Size(); i++)
        {
            received_buffer.Push(buffer[i]);    // сохраняем в нём все несохранённые данные
        }

        buffer.Clear();                         // И не забываем очистить прожежуточный
    }
}


uint Panel::TimePassedAfterLastEvent()
{
    return TIMER_MS - timeLastEvent;
}


void Panel::DisablePower()
{
    if (TIMER_MS > 1000)
    {
        TransmitData(TypeLED::Power);
    }
}


uint8 Panel::NextData()
{
    return data_for_send.Front();
}


void Panel::DisableInput()
{
    isRunning = false;
}


void Panel::EnableInput()
{
    isRunning = true;
}


void Panel::Init()
{
    led_RegSet.Disable();
    led_Trig.Disable();
    PageChannel::A::OnChanged_Input(true);
    PageChannel::B::OnChanged_Input(true);
}


Key::E Panel::WaitPressingKey()
{
    queue.Clear();

    pressedKey = Key::None;

    while (received_buffer.Size() != 0)
    {
        CopyData();
        queue.Clear();
    }

    while (pressedKey == Key::None)
    {
        Update();
    };

    return pressedKey;
}


void Panel::TransmitData(uint8 data)
{
    if (data_for_send.Size() > 20)
    {
        LOG_WRITE("Не могу послать в панель - мало места");
    }
    else
    {
        data_for_send.Push(data);
    }
}


static void Panel::HelpLong()
{
    Menu::showHelpHints = !Menu::showHelpHints;
    PageHelpContent::stringForHint = 0;
    PageHelpContent::itemHint = 0;
}


static void Panel::RShiftDownA()
{
    ChannelLongA();
}


static void Panel::ChannelLongA()
{
    Menu::Handlers::LongPressureKey(Key::ChannelA);
}


static void Panel::RShiftDownB()
{
    ChannelLongB();
}


static void Panel::ChannelLongB()
{
    Menu::Handlers::LongPressureKey(Key::ChannelB);
}


static void Panel::TShiftDown()
{
    TimeLong();
}


static void Panel::TimeLong()
{
    Menu::Handlers::LongPressureKey(Key::Time);
}


static void Panel::TrigLong()
{
    if (TRIG_MODE_LONG_PRESS == ModeLongPressTrig::Zero)
    {
        Menu::Handlers::LongPressureKey(Key::Trig);
    }
    else
    {
        TrigLev::FindAndSet();
    }
}


static void Panel::StartDown()
{
    if (set.memory.mode_work.IsDirect())
    {
        FPGA::OnPressStartStop();
    }
}


static void Panel::PowerDown()
{
    ((Page *)Item::Opened())->ShortPressOnItem(0);

    Panel::DisablePower();
}


static void Panel::SetDown()
{
    MenuLong();
}


static void Panel::MenuLong()
{
    Menu::Handlers::LongPressureKey(Key::Menu);
}


static void Panel::F1Long()
{
    Menu::Handlers::LongPressureKey(Key::F1);
}


static void Panel::F2Long()
{
    Menu::Handlers::LongPressureKey(Key::F2);
}


static void Panel::F3Long()
{
    Menu::Handlers::LongPressureKey(Key::F3);
}


static void Panel::F4Long()
{
    Menu::Handlers::LongPressureKey(Key::F4);
}


static void Panel::F5Long()
{
    Menu::Handlers::LongPressureKey(Key::F5);
}


static bool Panel::CanChangeRShiftOrTrigLev(TrigSource::E ch, int16 rShift)
{
    static uint times[3] = {0, 0, 0};

    uint &time = times[ch];

    if (rShift == RShift::ZERO)
    {
        time = TIMER_MS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (TIMER_MS - time > MIN_TIME)
    {
        time = 0;
        return true;
    }

    return false;
}


int Panel::CalculateCount(int *prevTime)
{
    uint time = TIMER_MS;
    uint delta = time - *prevTime;
    *prevTime = (int)time;

    if (delta > 75)
    {
        return 1;
    }
    else if (delta > 50)
    {
        return 2;
    }
    else if (delta > 25)
    {
        return 3;
    }
    return 4;
}


static void Panel::ChangeRShift(int *prevTime, void(*f)(const Chan &, int16), const Chan &ch, int16 relStep)
{
    if (ENUM_ACCUM_IS_NONE)
    {
        FPGA::SetTemporaryPause();
    }
    int count = CalculateCount(prevTime);
    int rShiftOld = RSHIFT(ch);
    int rShift = RSHIFT(ch) + relStep * count;
    if ((rShiftOld > RShift::ZERO && rShift < RShift::ZERO) || (rShiftOld < RShift::ZERO && rShift > RShift::ZERO))
    {
        rShift = RShift::ZERO;
    }
    if (CanChangeRShiftOrTrigLev((TrigSource::E)ch.value, (int16)rShift))
    {
        Sound::RegulatorShiftRotate();
        f(ch, (int16)rShift);
    }
}


static void Panel::RShiftLeftA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChA, -RShift::STEP());
}


static void Panel::RShiftRightA()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChA, +RShift::STEP());
}


static void Panel::RShiftLeftB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChB, -RShift::STEP());
}


static void Panel::RShiftRightB()
{
    static int prevTime = 0;
    ChangeRShift(&prevTime, RShift::Set, ChB, +RShift::STEP());
}


static void Panel::ChangeTrigLev(int *prevTime, void(*f)(TrigSource::E, int16), TrigSource::E trigSource, int16 relStep)
{
    int count = CalculateCount(prevTime);
    int trigLevOld = TRIG_LEV(trigSource);
    int trigLev = TRIG_LEV(trigSource) + relStep * count;

    if ((trigLevOld > TrigLev::ZERO && trigLev < TrigLev::ZERO) ||
        (trigLevOld < TrigLev::ZERO && trigLev > TrigLev::ZERO))
    {
        trigLev = TrigLev::ZERO;
    }

    if (CanChangeRShiftOrTrigLev(trigSource, (int16)trigLev))
    {
        Sound::RegulatorShiftRotate();
        f(trigSource, (int16)trigLev);
    }
}


static void Panel::TrigLevLeft()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, TrigLev::Set, TRIG_SOURCE, -RShift::STEP());
}


static void Panel::TrigLevRight()
{
    static int prevTime = 0;
    ChangeTrigLev(&prevTime, TrigLev::Set, TRIG_SOURCE, +RShift::STEP());
}


static void Panel::ChangeShiftScreen(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    float step = (float)(relStep * count);
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else if (step < 1)
    {
        step = 1;
    }
    f((int)(step));
}


static bool Panel::CanChangeTShift(int16 tShift)
{
    static uint time = 0;
    if (tShift == 0)
    {
        time = TIMER_MS;
        return true;
    }
    else if (time == 0)
    {
        return true;
    }
    else if (TIMER_MS - time > MIN_TIME)
    {
        time = 0;
        return true;
    }
    return false;
}


static void Panel::ChangeTShift(int *prevTime, void(*f)(int), int16 relStep)
{
    int count = CalculateCount(prevTime);
    int tShiftOld = TSHIFT;
    float step = (float)(relStep * count);
    if (step < 0)
    {
        if (step > -1)
        {
            step = -1;
        }
    }
    else
    {
        if (step < 1)
        {
            step = 1;
        }
    }

    int16 tShift = (int16)(TSHIFT + step);
    if (((tShiftOld > 0) && (tShift < 0)) || (tShiftOld < 0 && tShift > 0))
    {
        tShift = 0;
    }
    if (CanChangeTShift(tShift))
    {
        Sound::RegulatorShiftRotate();
        f(tShift);
    }
}


static void Panel::XShift(int delta)
{
    static int prevTime = 0;
    if (!FPGA::IsRunning() || FUNCTION_REG_TIME_SHIFT_IN_MEMORY)
    {
        if (ENUM_POINTS_FPGA != EnumPointsFPGA::_281)
        {
            ChangeShiftScreen(&prevTime, Display::ShiftScreen, (int16)(2 * delta));
        }
    }
    else
    {
        ChangeTShift(&prevTime, TShift::Set, (int16)delta);
    }
}


static void Panel::TShiftLeft()
{
    XShift(-1);
}


static void Panel::TShiftRight()
{
    XShift(1);
}


static void Panel::RangeLeftA()
{
    Sound::RegulatorSwitchRotate();
    Range::Increase(ChA);
}


static void Panel::RangeRightA()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(ChA);
}


static void Panel::RangeLeftB()
{
    Sound::RegulatorSwitchRotate();
    Range::Increase(ChB);
}


static void Panel::RangeRightB()
{
    Sound::RegulatorSwitchRotate();
    Range::Decrease(ChB);
}


static void Panel::TBaseLeft()
{
    Sound::RegulatorSwitchRotate();
    TBase::Increase();
}


static void Panel::TBaseRight()
{
    Sound::RegulatorSwitchRotate();
    TBase::Decrease();
}


static void Panel::SetLeft()
{
    Menu::Handlers::RegulatorSet(Action::RotateLeft);
}


static void Panel::SetRight()
{
    Menu::Handlers::RegulatorSet(Action::RotateRight);
}


LED::LED(TypeLED::E _led) : led(_led), enabled(false)
{
}


void RegSetLED::Switch()
{
    static bool first = true;
    static bool prevState = false;  // true - горит, false - не горит

    bool state = NeedForFire();

    if (first)
    {
        first = false;
        led_RegSet.SwitchToState(state);
        prevState = state;
    }
    else if (prevState != state)
    {
        led_RegSet.SwitchToState(state);
        prevState = state;
    }
}


bool RegSetLED::NeedForFire()
{
    if (!MENU_IS_SHOWN)
    {
        return false;
    }

    NamePage::E name = Menu::GetNameOpenedPage();

    if (name == NamePage::MeasuresTune_SB && MEASURES_NUMBER_IS_1 && !PageMeasures::choiceMeasuresIsActive)
    {
        return false;
    }

    if (
        name == NamePage::ServiceMathFFTCursors_SB ||
        name == NamePage::MeasuresTune_SB ||
        name == NamePage::MemoryLatest_SB ||
        name == NamePage::MemoryInt_SB ||
        (name == NamePage::ServiceMathFunction_SB && !MODE_DRAW_MATH_IS_DISABLED) ||
        (name == NamePage::CursorsSet_SB && Cursors::NecessaryDraw())
        )
    {
        return true;
    }

    if (Menu::CurrentItem()->IsGovernor() || Menu::CurrentItem()->IsChoiceReg())
    {
        return true;
    }

    if (Item::Opened()->IsChoice() ||
        (Item::Opened()->IsPage() && (Item::Opened()->ReinterpretToPage()->NumSubPages() > 1)))
    {
        return true;
    }

    return false;
}
