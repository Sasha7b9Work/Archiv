#pragma once
#include "DisplayTypes.h"
#include "Keyboard/Keyboard.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BACKGROUND                  (set.disp_background)
#define BACKGROUND_BLACK            (BACKGROUND == Display::Background::Black)

#define SHOW_STRING_NAVI            (set.disp_showStringNavigation)
#define SHOW_STRING_NAVI_TEMP       (SHOW_STRING_NAVI == Display::ShowStrNavi::Temp)
#define SHOW_STRING_NAVI_ALL        (SHOW_STRING_NAVI == Display::ShowStrNavi::All)

#define MODE_DRAW_SIGNAL            (set.disp_modeDrawSignal)
#define MODE_DRAW_SIGNAL_IS_LINES   (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Lines)
#define MODE_DRAW_SIGNAL_IS_POINTS  (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Points)

#define THICKNESS_SIGNAL            (set.disp_thickness)
#define THICKNESS_SIGNAL_IS_3       (THICKNESS_SIGNAL == Display::ThicknessSignal::_3)

#define ENUM_MIN_MAX                (set.disp_ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* ���������� ���������� ��������� �������� ��� ������� ��������� � ����������. */

#define ENUM_SMOOTHING              (set.disp_ENumSmoothing)
#define NUM_SMOOTHING               (Display::NumPointSmoothing())
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != Display::ENumSmoothing::Disable)

#define MODE_ACCUM                  (set.disp_modeAccumulation)
#define MODE_ACCUM_RESET            (MODE_ACCUM == Display::ModeAccumulation::Reset)
#define MODE_ACCUM_NO_RESET         (MODE_ACCUM == Display::ModeAccumulation::NoReset)

#define ENUM_ACCUM                  (set.disp_ENumAccum)
#define ENUM_ACCUM_INF              (ENUM_ACCUM == ENumAccum_Infinity)
#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* ���������� ����� ���������� */
#define IN_ACCUM_MODE               (ENUM_ACCUM > ENumAccum_1)

#define MODE_AVE                    (set.disp_modeAveraging)
#define ENUM_AVE                    (set.disp_ENumAverage)
#define NUM_AVE                     (1 << (int)ENUM_AVE)
#define IN_AVERAGING_MODE           (ENUM_AVE > Display::ENumAverage::_1 && (!IN_P2P_MODE))

#define SHIFT_IN_MEMORY             (set.disp_shiftInMemory)
#define TIME_MESSAGES               (set.disp_timeMessages)
#define ENUM_SIGNALS_IN_SEC         (set.disp_ENumSignalsInSec)
#define LAST_AFFECTED_CH            (set.disp_lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH.IsA())
#define DISPLAY_ORIENTATION         (set.dbg_Orientation)

#define ALT_MARKERS                 (set.disp_altMarkers)
#define ALT_MARKERS_SHOW            (ALT_MARKERS == AltMarkers::Show)
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AltMarkers::Hide)
#define ALT_MARKERS_AUTO            (ALT_MARKERS == AltMarkers::AutoHide)

#define LINKING_RSHIFT              (set.disp_linkingRShift)

#define MENU_AUTO_HIDE              (set.disp_menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp_brightness)

#define BRIGHTNESS_GRID             (set.disp_brightnessGrid)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)

#define SHIFT_IN_MEMORY_IN_POINTS   Display::ShiftInMemoryInPoints()


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// C��������, ��������� �� �����
struct Warning
{
    enum E
    {
        LimitChan1_Volts,               ///< ������ �������� �� ����������� ������� ������.
        LimitChan2_Volts,               ///< ������ �������� �� ���������� ������� ������.
        LimitSweep_Time,                ///< ������ �������� �� �������.
        EnabledPeakDet,                 ///< ������� ������� ��������.
        LimitChan1_RShift,              ///< ������ �������� �� ���������� ������� ������.
        LimitChan2_RShift,              ///< ������ �������� �� ���������� ������� ������.
        LimitSweep_Level,               ///< ������ ������ �������������.
        LimitSweep_TShift,              ///< ������ �������� �� �������.
        TooSmallSweepForPeakDet,        ///< ������� ������� �������� ��� �������� ���������.
        TooFastScanForRecorder,         ///< ������� ������� �������� ��� ����������� ������.
        FileIsSaved,                    ///< ���� ������� �� ������.
        SignalIsSaved,                  ///< ������ ������� � ���.
        SignalIsDeleted,                ///< �� ���� ����� ������.
        MenuDebugEnabled,               ///< �������� ���� �������.
        TimeNotSet,                     ///< ����� �� �����������.
        SignalNotFound,                 ///< ������ �� ������ (���������).
        SetTPosToLeft,                  ///< ������������.
        NeedRebootDevice,               ///< ��������� ������������.
        ImpossibleEnableMathFunction,   ///< ���������� �������� �������������� �������.
        ImpossibleEnableFFT,            ///< ���������� �������� ���������� ���.
        WrongFileSystem,                ///< ���������� ���������� ������.
        WrongModePeackDet,              ///< ���������� �������� ������� ��������.
        DisableChannelB,                ///< ��������� ��������� ����� 2.
        TooLongMemory,                  ///< ��������� � ������������� ��������� ������ 32�.
        NoPeakDet32k,                   ///< ������� �������� �� �������� ��� ����� ������ 32�.
        NoPeakDet16k,                   ///< ������� �������� �� �������� ��� ����� ������ 16�.
        Warn50Ohms,                     ///< �������������� �� ����������� �������� ������ �� 50 ����.
        WarnNeedForFlashDrive,          ///< ��������� ������������ ������.
        FirmwareSaved,                  ///< �������� ��������� �� ������.
        FullyCompletedOTP,              ///< 
        Number
    } value;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
    friend class LogEntity;
public:
    
    static const int HEIGHT = 240;

    static const int WIDTH = 320;
    /// ��� ������� ����� �������� ��� �������� ����� ������ ������������� ��� ��������� ��������� ��������������� ��������
    static void RotateTrigLev();

    static void Init();

    static void Update();

    static void SetKey(Key key);

    static void RemoveAddDrawFunction();

    static void ShowWarning(Warning::E warning);
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode::Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode::Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    static void SetDrawMode(DrawMode mode, pFuncVV func);

    static void FuncOnWaitStart(const char *text, bool eraseBackground);

    static void FuncOnWaitStop();

    static void SetAddDrawFunction(pFuncVV func);

    static void ChangedRShiftMarkers(bool active);

    static void Clear();

private:

    static Key key;

public:

    /// ����� ����� ����.
    struct Background
    {
        enum E
        {
            Black,
            White
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct ModeDrawSignal
    {
        enum E
        {
            Lines,
            Points
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ������� ��������� �������
    struct ThicknessSignal
    {
        enum E
        {
            _1,         ///< ������ �������� ������ �������� ���� �����
            _3          ///< ������ �������� ������ �������� ��� �����
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ����� ������ ������ ���������
    struct ShowStrNavi
    {
        enum E
        {
            Temp,   ///< ���������� �� ��������� ������
            All,    ///< ������ ����������
            None    ///< ������� �� ����������
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ���������� ��������� ��� ������� ������������ � ������������� ��������.
    struct ENumMinMax
    {
        enum E
        {
            _1,
            _2,
            _4,
            _8,
            _16,
            _32,
            _64,
            _128
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ��� ���������� �� ����������
    struct ModeAveraging
    {
        enum E
        {
            Accurately,   ///< ��������� �����.
            Around        ///< ��������� ��������������.
        } value;
        ModeAveraging(E v) : value(v) {};
        operator uint8() const  { return (uint8)value; };
    };

    struct Orientation
    {
        enum E
        {
            Direct,
            Back
        } value;
    };
    /// ����� ����������� �������������� ������� �������� �������� �� ����������.
    struct AltMarkers
    {
        enum E
        {
            Hide,        /// ������ �� ��������.
            Show,        /// ������ ��������.
            AutoHide     /// �������� � ������� ����� timeShowLevels.
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ��� �������� � �������� �� ���������
    struct LinkingRShift
    {
        enum E
        {
            Voltage,      ///< �������� ��������� � ����������
            Position      ///< �������� ��������� � �������
        } value;
    };

    /// ����������� FPS.
    struct ENumSignalsInSec
    {
        enum E
        {
            _25,
            _10,
            _5,
            _2,
            _1
        } value;
        ENumSignalsInSec(E v = _25) : value(v) { };
        operator uint8() const { return (uint8)value; };
        /// ���������� ���������� ������ � �������
        int ToFPS() const;
    };

    /// ���������� ����� ��� ������� �����������.
    struct ENumSmoothing
    {
        enum E
        {
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
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct ModeAccumulation
    {
        enum E
        {
            NoReset,   /// � ���� ������ ������������ ������ N ���������.
            Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ���������� ����������.
    struct ENumAccum
    {
        enum E
        {
            _1,
            _2,
            _4,
            _8,
            _16,
            _32,
            _64,
            _128,
            _Infinity
        } value;
        operator uint8() const { return (uint8)value; };
    };

    /// ���������� ���������� �� ����������.
    struct ENumAverage
    {
        enum E
        {
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
        } value;
        operator uint8() const  { return (uint8)value; };
    };

    static void SetOrientation(Orientation orientation);
    /// ���������� ����� ����������
    static ModeAveraging GetModeAveraging();
    /// ������������� ����������� ������� ������
    static void SetNumSignalsInS(int maxFPS);
    /// ���������� ����� ����� ������������� ������� (����� ������� - �����������)
    static int NumPointSmoothing();
    /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
    static BitSet64 PointsOnDisplay();
    /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
    static BitSet64 BytesOnDisplay();
    /// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
    static int TimeMenuAutoHide();
    /// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();
    /// brightness = 1..100
    static void SetBrightness(int16 brightness);

    static int NumAverage();
    /// ��� �������� ������ �� ������ � ������ �������� ��������� ��� ����� � ��� ���� ������, ��� ����, ������ ��� ������ ����� ������������ ����� �������
    static int ShiftInMemoryInPoints();
};
