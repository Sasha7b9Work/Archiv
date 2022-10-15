#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup Measure
 *  @brief ��������� ������. ����� ������� ������� ������������� � ������� �� ����� � �������������� ���������� �������������� ���������
 *  @{
 */


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SHOW_MEASURES           (set.meas_show)

#define MEAS_MARKED             (set.meas_marked)

/// �� ������ ������ ����������� �������������� ���������
#define SOURCE_MEASURES         (set.meas_source)
/// �������������� ��������� ������������ ������ �� ������ A
#define SOURCE_MEASURES_IS_A    (SOURCE_MEASURES == Measure::Source::A)
/// �������������� ��������� ������������ ������ �� ������ B
#define SOURCE_MEASURES_IS_B    (SOURCE_MEASURES == Measure::Source::B)
/// �������������� ��������� ������������ �� ������� A � B
#define SOURCE_MEASURES_IS_BOTH (SOURCE_MEASURES == Measure::Source::A_B)
/// �������� �������������� ��������� �� ������ A
#define VIEW_MEASURES_A         (SET_ENABLED_A && (SOURCE_MEASURES_IS_A || SOURCE_MEASURES_IS_BOTH))
/// �������� �������������� ��������� �� ������ B
#define VIEW_MEASURES_B         (SET_ENABLED_B && (SOURCE_MEASURES_IS_B || SOURCE_MEASURES_IS_BOTH))
/// �������� �������������� ��������� �� ����� �������
#define VIEW_MEASURES_BOTH      (SET_ENABLED_BOTH && SOURCE_MEASURES_IS_BOTH)


/// ������� �� ����� ��� ������ ���������
#define MODE_VIEW_SIGNALS               (set.meas_modeViewSignals) 
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == Measure::ModeViewSignals::Compress)

#define NUM_MEASURES                    (set.meas_number)
#define NUM_MEASURES_IS_1_5             (NUM_MEASURES == Measure::OnDisplay::_1_5)
#define NUM_MEASURES_IS_2_5             (NUM_MEASURES == Measure::OnDisplay::_2_5)
#define NUM_MEASURES_IS_3_5             (NUM_MEASURES == Measure::OnDisplay::_3_5)
#define NUM_MEASURES_IS_6_1             (NUM_MEASURES == Measure::OnDisplay::_6_1)
#define NUM_MEASURES_IS_6_2             (NUM_MEASURES == Measure::OnDisplay::_6_2)

#define POS_MEAS_CUR_U(n)               (set.meas_PosCurU[n])
#define POS_MEAS_CUR_U_0                (POS_MEAS_CUR_U(0))
#define POS_MEAS_CUR_U_1                (POS_MEAS_CUR_U(1))

#define POS_MEAS_CUR_T(n)               (set.meas_PosCurT[n])
#define POS_MEAS_CUR_T_0                (POS_MEAS_CUR_T(0))
#define POS_MEAS_CUR_T_1                (POS_MEAS_CUR_T(1))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Measure
{
public:
    // ���� ���������
    struct Type
    {
        enum E
        {
            None,
            VoltageMax,
            VoltageMin,
            VoltagePic,
            VoltageMaxSteady,
            VoltageMinSteady,
            VoltageAmpl,
            VoltageAverage,
            VoltageRMS,
            VoltageVybrosPlus,
            VoltageVybrosMinus,
            Period,
            Freq,
            TimeNarastaniya,
            TimeSpada,
            DurationPlus,
            DurationMinus,
            SkvaznostPlus,
            SkvaznostMinus,
            DelayPlus,
            DelayMinus,
            PhazaPlus,
            PhazaMinus,
            Number
        } value;
        Type(int8 v = 0) : value((E)v) {};
        operator uint8() const { return (uint8)value; };
    };

    Measure(int r, int c) : row(r), col(c) {};
    /// ���������� ��������� �� ������ (row, col)
    static Measure Get(int row, int col) { return Measure(row, col); };

    Type GetType();

    /// ������������� �������� ��������� ��� ���������� ���������
    static void ChangeActive(int delta);
    /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    static void SetData(bool needSmoothing);
    /// ���������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    static float CalculateCursorU(Chan ch, float posCurT);
    /// ���������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    static float CalculateCursorT(Chan ch, float posCurU, int numCur);

    static void SetMarkerVoltage(Chan ch, int num, float value);

    static void SetMarkerTime(Chan ch, int num, int value);
   
    static char GetChar(Type measure);

    static int NumRows();

    static int NumCols();
    /// �� ������� ������� ����� �� �����������
    static int GetDeltaGridLeft();

    static void ShortPressOnSmallButonMarker();
    /// ������� ��������� ��������� (�� ������� ������)
    static int8 posActive;
    /// ���� true - �������� �������� ������ ���������
    static bool pageChoiceIsActive;
    /// ������� ������� �� �������� ������ ���������
    static int8 posOnPageChoice;

    //------------------------------------------------------------------------------------------------------------------------------------------------
    /// ������� �� ������� ��� ������ ���������.
    struct ModeViewSignals
    {
        enum E
        {
            AsIs,       ///< ���������� ������� ��� ����.
            Compress    ///< ������� ����� � ���������.
        } value;
        operator uint8() const
        {
            return (uint8)value;
        };
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct Source
    {
        enum E
        {
            A,
            B,
            A_B
        } value;
        operator uint8() const { return (uint8)value; };
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct OnDisplay
    {
        enum E
        {
            _1,      ///< 1 ��������� ����� �����.
            _2,      ///< 2 ��������� ����� �����.
            _1_5,    ///< 1 ������ � 5 �����������.
            _2_5,    ///< 2 ������ �� 5 ���������.
            _3_5,    ///< 3 ������ �� 5 ���������.
            _6_1,    ///< 6 ����� �� 1 ���������.
            _6_2     ///< 6 ����� �� 2 ���������.
        } value;
        operator uint8() const { return (uint8)value; };
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class Graphics
    {
    public:
        /// ���������� ���������� �������������� ���������
        static void Draw();
        /// ���������� ���������� x �������� ������ ���� ������� ��������� ���������
        static int GetTop();

    private:
        /// ������� ���������� y ��������� ������� �������������� ���������
        static int top;
    };

#define MARKER_VOLTAGE(ch, num)             (Processing::markerVoltage[ch][num] - MIN_VALUE)
#define MARKER_TIME(ch, num)                (Processing::markerTime[ch][num])

private:

    /// ���������� ������ ���� ��� ������ ��������������� ���������
    static int DY();
    /// ���������� ������ ���� ��� ������ ��������������� ���������
    static int DX();
    /// ���������� �������� ���������
    static Measure GetActive();
    /// ���������� true, ���� ��������� �������� - ������� ������
    bool IsActive();
    /// ������� ��������
    static void SetActive(int row, int col);

    static int GetTopTable();

    /// �� ������� ������� ����� �� ���������
    static int GetDeltaGridBottom();

    pString Name();
    /// ���������� �������� ������ ���������
    static void DrawPageChoice();
    /// ������ � �������, � ������� ��������� ������ ���������
    int row;
    /// ������� � �������, � ������� ��������� ������ ���������
    int col;

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class Processing
    {
        friend class Measure;

    public:
        /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
        static void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
        /// ���������� ������ ��������������� ���������
        static char* GetStringMeasure(Measure::Type measure, Chan ch, char *buffer, int lenBuf);
        /// ��������� ��� �������������� ���������
        static void CalculateMeasures();

        static int markerTime[Chan::Number][2];

        static int markerVoltage[Chan::Number][2];

    private:
        /// ���������� ������� � ������������� � ������� ����������
        static void CountedToCurrentSettings();
        /// ���������� ������� � ������� Range � rShift
        static void CountedRange(Chan ch);
        /// ���������� ������� � �������� TBase
        static void CountedTBase();
        /// ���������� ������� � �������� tShift
        static void CountedTShift();
        /// ���������� ���������� ����� � ������� � ������������� ����������
        static void CountedEnumPoints();
    };
};


/** @}  @}
 */
