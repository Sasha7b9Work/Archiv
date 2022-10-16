// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Display/Colors_.h"
#include "common/Utils/Containers/String_.h"
#include "common/Utils/Containers/Values_.h"


struct Chan { enum E {
        A,
        B,
        A_B,
        Math,
        Count = 2
    } value;

    Chan(E v = A) : value(v) {}
    void Enable() const;
    void Disable() const;
    bool IsEnabled() const;
    bool IsA() const        { return (value == A);    }
    bool IsB() const        { return (value == B);    }
    bool IsAB() const       { return (value == A_B);  }
    bool IsMath() const     { return (value == Math); }
    operator int() const    { return (int)value;      }
    Color GetColor() const;
    bool IsInversed() const;
    int ToNumber() const;

    // ������ ����� ������ ����� ������������ �������
    static Chan &FirstDrawing();

    // ������ ����� ������ ����� ������������ ����������
    static Chan &SecondDrawing();

    // ���������� "��������������� �����"
    Chan Opposed() const;
};

extern Chan ChA;
extern Chan ChB;


// ���������� ������������
struct TrigPolarity { enum E {
        Front,  // ������������� �� ������.
        Back    // ������������� �� �����.
    };

    static void Set(E polarity);

    // ������������ ���������� - ��������������� ������ ����� ����������
    static void Switch();

    static void Load();

    static uint timeSwitch;     // ����� ��������������� ������� ����� ����������. ������������� � 0 �������� ��������,
                                // ��� �������������� ������ �� ������������
};


// ����������� �������� �� ������������ ���������� ������� �������.
struct MaxDbFFT { enum E {
        _40,        // ������������ ���������� �� -40��
        _60,        // ������������ ���������� �� -60��
        _80         // ������������ ���������� �� -80��
    };

    static float GetReal(E);
};



// ������� �� �������
struct TBase { enum E {
        _1ns,
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5s,
        _10s,
        Count
    };

    static void Set(TBase::E tbase);

    static void Load();

    static void Decrease();

    static void Increase();

    static pchar Name(TBase::E tbase);

    static pchar ToString(TBase::E tbase);
    static pchar ToStringEN(TBase::E tbase);

    static E MIN_P2P;       // � ���� �������� ���������� ���������� �����
    static E MIN_PEAK_DET;  // � ���� �������� ����� �������� ������� ��������
    static E MAX_RANDOMIZE;  // ������������ ��������, �� ������� �������� ������������

    // ���������� ��� ����� ������� ��� ������������� ��� ������������� ��������
    static int StepRand();
};


struct TShift
{
    static void Set(int tshift);

    static void Draw();

    // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ����
    // ������.
    static void SetDelta(int16 shift);

    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����
    static String ToString(int tshift_rel);

    // ����������� �������� �� �������, ������� ����� ���� �������� � ���������� �����.
    static int16 Min();

    // �������� �� �������, ��������������� ������� TPos.
    static int16 Zero()
    {
        return -TShift::Min();
    }

    // �������� �� �������, ������������ ��� ������� � ����/�����- �������
    static int ForLaunchFPGA();

    // tshift ����� ��������� ������� �������� � ���������� �������
    static Time ToAbs(float shift, TBase::E base);
    static Time ToAbs(int shift, TBase::E base);

    static float ToRel(float tshift_abs, TBase::E tbase);

    static const int NULL_VALUE = 1000000;

    static const int16 MAX = 16000;

    static const float abs_step[TBase::Count];

    static int add_rand;                // �������������� �������� � ������ �������������
};


// �������� �������������.
struct TrigSource { enum E {
        A,      // ����� 1.
        B,      // ����� 2.
        Ext     // �������.
    };

    static void Set(E);
    static void Set(const Chan &);
};


// ��������.
struct Divider { enum E {
        _1,
        _10
    };

    static int ToAbs(E multiplier)
    {
        static const int results[2] = { 1, 10 };

        return results[multiplier];
    }
    static int ToAbs(const Chan &ch);
};


// ������� �� ����������.
struct Range { enum E {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Count
    };

    static void Set(const Chan &, E);

    static void Load(const Chan &);

    static bool Increase(const Chan &);

    static bool Decrease(const Chan &);

    static pchar Name(Range::E);

    static pchar ToString(Range::E, Divider::E);
};

Range::E &operator++(Range::E &);
Range::E &operator--(Range::E &);


struct RShift
{
    static void Set(const Chan &ch, int16 rShift);

    static void Load(const Chan &ch);

    static int ToRel(float rshift_abs, Range::E);

    static Voltage ToAbs(int16 rshift, Range::E);

    static Voltage ToAbs(int16 rshift, Range::E, Divider::E multiplier);

    static void Draw();

    static pchar ToString(int16 rshift, Range::E, Divider::E multiplier);

    static bool show_line[Chan::Count];    // ����� �� �������� �������������� ����� ������ �������� �������

    static const int16 MIN = 20;    // ����������� �������� �������� ������ �� ����������, ���������� � ���������� �����
                                    // ������������� �������� 10 ������ ���� �� ����������� �����
    static const int16 ZERO = 500;  // ������������� ������������ ������ �� ������ ������.
    static const int16 MAX = 980;   // ������������� �������� 10 ������ ����� �� ����������� �����.

    static int16 STEP();            // �� ������� ������ ����� �������� �������� ��������, ����� ������ �������� ��
                                    // ���������� ������������ �� ���� �����

private:

    static void Draw(const Chan &ch);

    static void OnChanged(const Chan &ch);

    static void FuncOnTimerDisableShowA();
    static void FuncOnTimerDisableShowB();

    // ��������� ��������������� ����� ������� �������� �� ����������
    static void DisableShowLine(const Chan &ch);

    static const float abs_step[Range::Count];
};


struct TrigLev
{
    static void Load();

    static void Set(TrigSource::E ch, int16 trigLev);
    static void Set(const Chan &ch, int16 trigLev);

    // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    static void FindAndSet();

    static void Draw();

    // ���������� �������� �� ������
    static void WriteValue();

    static bool need_auto_find;   // ���� true, �� ����� ���������� ����� �������������

    static const int16 MIN = RShift::MIN;
    static const int16 MAX = RShift::MAX;
    static const int16 ZERO = RShift::ZERO;     // ������� �������� ������ �������������, ���������� � ������.
                          // ������ ������������� ��� ���� ���������� �� ����� ������ � �������� �������� �� ����������.

    static bool show_level;                     // ����� �� ���������� �������� �� �����
};


// ����� ����� �������, � ������� ��� ������.
struct EnumPointsFPGA { enum E {
        _281,
        _512,
        _1k,
        Count
    };

    // ��������� ������������ � ���������� �����
    static int ToPoints(E v)
    {
        static const int points[Count] =
        {
            281,
            512,
            1024
        };

        return points[v];
    }

    static E FromPoints(uint num_points);

    // ������� ����� � ����� ������ ��� ������� ����������
    static int PointsInChannel();

    // ������� ���� �������� ���� ����� ��� ������� ����������
    static int BytesInChannel();

    // ������� ���� ����� ��� ���������� ������ ����� ������� ��� ������� ����������
    static int BytesForData()
    {
        int result = 0;

        int bytes_in_channel = BytesInChannel();

        if (ChA.IsEnabled()) { result += bytes_in_channel; }

        if (ChB.IsEnabled()) { result += bytes_in_channel; }

        return result;
    }
};


// �������� ������������� � ������.
struct TPos { enum E {
        Left,      // ������������� ��������� � ������ ������.
        Center,    // ������������� ��������� � ������ ������.
        Right,     // ������������� ��������� � ����� ������.
        Count
    };

    // ������ �������� ���������� ������ ������������� � ������
    static int InPoints(EnumPointsFPGA::E enum_points, E t_pos);

    static void Draw();

    static void Draw(int x, int y);
};


// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide { enum E {
        None,   // �������.
        _5,     // ����� 5 ������.
        _10,    // ����� 10 ������.
        _15,    // ����� 15 ������.
        _30,    // ����� 30 ������.
        _60     // ����� 60 ������.
    };

    // ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������.
    static int Time(E);
};


// ���������� ��������� ��� ������� ������������ � ������������� ��������.
struct ENumMinMax { enum E {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128
    };

    static int ToAbs(E value) { return (1 << value); }
    // ���������� 0, ���� value==_1
    static int ToDisplayValue(E);
};


// ���������� ����� ��� ������� ����������� �������.
struct Smoothing { enum E {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };

    // ���������� ����� ����� ������������� ������� (����� ������� - �����������).
    static uint NumPoints(E);
};


// ����������� FPS.
struct ENumSignalsInSec { enum E {
        _25,
        _10,
        _5,
        _2,
        _1,
        Count
    };

    // ���������� ����������� ������� ������.
    static int NumSignalsInS(E);

    // ���������� ���������� ������� ����� �����������
    static uint DeltaTime(E);
};


// ��� ���������� �� ����������.
struct ModeAveraging { enum E {
        Accurately,   // ��������� �����.
        Around        // ��������� ��������������.
    };

    // ���������� ����� ����������
    static ModeAveraging::E Current();
};


// ���������� ���������� �� ����������.
struct ENumAveraging { enum E {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        _512
    };

    static int NumAverages(E);
    static int ToAbs(E);
    // ���������� 0, ���� E==_1
    static int ToDisplayValue(E);
};


// ������������ ����������.
struct ENumAccumulation { enum E {
		_1,
		_2,
		_4,
		_8,
		_16,
		_32,
		_64,
		_128,
		Infinity
	};

    // �������������� � �������� ��� ������ �� �����. 1 - 0, Infinity - -1
    static int ToDisplayValue(E);
};


// ����� ������ �� �����.
struct ModeCouple { enum E {
        DC,      // �������� ����.
        AC,      // �������� ����.
        GND      // ���� �������.
    };

    static void Set(const Chan &, E);
};


// ��� ������� ��� ������ �������������.
struct SampleType { enum E {
    Real,   // �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
    Equal   // ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
};};


// �������� ������� ��� ����������� �������.
struct SourceFFT { enum E {
    A_,          // ������ � ����������� ������� ������� ������.
    B_,          // ������ � ����������� ������� ������� ������.
    Both_        // ������ � ����������� �������� ����� �������.
};};


// ���� ����
struct Language { enum E
{
    Russian,    // �������
    English,    // ����������
    Count
};};


// ����� �� ��������� ������������ ������������ ��� ��������
struct RShiftADCtype { enum E
{
    Disabled,
    Enabled
};};


// ����� ��������� �������.
struct ModeDrawSignal { enum E {
    Lines,      // ������ �������� �������.
    Points,     // ������ �������� �������.
    Count
};};


// ��� ����� �� ������.
struct TypeGrid { enum E {
    _1,
    _2,
    _3,
    _4,
    Count
};};


// �������� �����
struct ColorScheme { enum E {
    WhiteLetters,   // � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����.
    BlackLetters    // � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������.
};};


// ����� ����������.
struct ModeAccumulation { enum E {
    NoReset,   // � ���� ������ ������������ ������ N ��������� ���������.
    Reset      // � ���� ������ ���������� N ��������� ��������� � ����� ������������.
};};


// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers { enum E {
    Hide,        // ������ �� ��������.
    Show,        // ������ ��������.
    AutoHide     // �������� � ������� ����� timeShowLevels.
};};


// ��� �������� � �������� �� ���������
struct LinkingRShift { enum E {
    Voltage,      // �������� ��������� � ����������
    Position      // �������� ���������� � ������� �� ������
};};


// ���������� � ������������ �� ������ ����������� ���������.
struct MeasuresNumber { enum E {
    _1,             // 1 ��������� ����� �����.
    _2,             // 2 ��������� ����� �����.
    _1_5,           // 1 ������ � 5 �����������.
    _2_5,           // 2 ������ �� 5 ���������.
    _3_5,           // 3 ������ �� 5 ���������.
    _6_1,           // 6 ����� �� 1 ���������.
    _6_2            // 6 ����� �� 2 ���������.
};};


// ����, �� ������� ��������� ��������
struct MeasuresField { enum E {
    Screen,         // ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    AllMemory,      // ��������� ����� ������������� �� ����� �������.
    Hand            // ��������� ����� ������������� �� ����, ����������� �������������.
};};


// ����� ������.
struct ModeWork { enum E {
        Direct,        // �������� �����.
        Latest,        // � ���� ������ ����� ����������� ��������� ���������� ���������.
        MemInt,        // � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� ����������.
    };
    E v;
    ModeWork(E _v = Direct) : v(_v) { }
    bool IsDirect() const { return v == Direct; }
    bool IsLatest() const { return v == Latest; }
    bool IsMemInt() const { return v == MemInt; }
    operator int() { return (int)v; }
};


// ����� ������������ ������.
struct FileNamingMode { enum E {
    Mask,        // ��������� �� �����.
    Manually     // ��������� �������.
};};


// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
struct ModeShowIntMem { enum E {
    Direct,  // ���������� ������ ��������� �������.
    Saved,   // ���������� ���������� ������.
    Both     // ���������� ������ ��������� ������� � ���������� ������.
};};


// ��� ������ ��� ������� ������ ������.
struct ModeBtnMemory { enum E {
    Menu,     // ����� ������������ ��������������� �������� ����.
    Save      // ���������� ����������� ������ �� ������.
};};


// ��� ��������� ������ �� ������.
struct ModeSaveSignal { enum E {
    BMP,     // ��������� ������ �� ������ � ������� .bmp.
    TXT      // ��������� ������ �� ������ � ���������� ����.
};};


// ������� ����� �����/���.
struct FunctionTime { enum E {
    Time,              // ����� ��������� ��������� �� �������.
    ShiftInMemory      // ����� ��������� ������������ �� ������ �������� ������.
};};


// ����� ������ �������� ���������.
struct PeakDetMode { enum E {
    Disable,
    Enable,
    Average,
    Count
};};


// ����� �������.
struct StartMode { enum E {
    Auto,             // ��������������.
    Wait,             // ������.
    Single            // �����������.
};};


// ���� �������������.
struct TrigInput { enum E {
        Full,   // ������ ������.
        AC,     // ����������.
        LPF,    // ���.
        HPF     // ���.
    };
    
    static void Set(E);
};


// ����� ����������� ������� ������ �����.
struct ModeLongPressTrig { enum E {
    Zero,   // ����� ������ ������������� � 0.
    Auto    // �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������
};};


// ����� ��������� �������������.
struct TrigModeFind { enum E {
    Hand,   // ������� ������������� ��������������� �������.
    Auto    // ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������
};};


// ����� �������� ���������.
struct CursCntrl { enum E {
    _1,            // ������.
    _2,            // ������.
    _1_2,          // ������.
    Disabled       // �������.
};};


// ������������ ����������� ��������.
struct CursMovement { enum E {
    Points,    // �� ������.
    Percents   // �� ���������.
};};


// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
struct CursActive { enum E {
    U,
    T,
    None
};};


// ����� �������� ��������.
struct CursLookMode { enum E {
    None,      // ������� �� ������.
    Voltage,   // ������� ������ �� ����������� �������������.
    Time,      // ������� ������ �� �������� �������������.
    Both       // ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};};


// ������� �� ������� ��� ������ ���������.
struct ModeViewSignals { enum E {
    AsIs,       // ���������� ������� ��� ����
    Compress    // ������� ����� � ���������
};};


struct ScaleMath { enum E
{
    Channel0,     // ������� ������ �� ������ 1
    Channel1,     // ������� ������ �� ������ 2
    Hand          // ������� ������� �������.
};};


// ������� ����� ��� ����������� �������.
struct ScaleFFT { enum E {
    Log,        // ��� �������� �������� ��������������� ����� ������ �������.
    Linear      // ��� �������� �������� �������� ����� ������ �������.
};};


// ��� ���� ��� ��������� ������� ����� �������� �������.
struct WindowFFT { enum E {
    Rectangle,  // ����� �������� ������� ������ �� ���������� �� ����.
    Hamming,    // ����� �������� ������� ������ ���������� �� ���� ��������.
    Blackman,   // ����� �������� ������� ������ ���������� �� ���� ��������.
    Hann        // ����� �������� ������� ������ ���������� �� ���� �����.
};};


// ����� ������� ������������.
struct Function { enum E {
    Sum,        // ����� ���� �������.
    Mul         // ������������ ���� �������.
};};


// ��� ������������ �������������� ������.
struct ModeDrawMath { enum E    {
    Disable,    // ����������� ��������������� ������� ���������.
    Separate,   // �������������� ������ ������������ �� ��������� ������.
    Together    // �������������� ������ ������������ ������ �������� �������.
};};


// ��� ��������� ����� ���������, ����� �������� �������������� �������.
struct ModeRegSet { enum E {
    Range,      // ����� ��������� ��������� ��������� �� ����������.
    RShift      // ����� ��������� ��������� ��������� �� ����������.
};};


// ����� ������ �����������.
struct CalibratorMode { enum E {
    Freq,           // �� ������ ����������� 4�, 1���.
    DC,             // �� ������ ����������� 4� ����������� ����������.
    GND             // �� ������ ����������� 0�.
};};


struct CalibrationMode { enum E {
    x1,
    x10,
    Disable
};};
