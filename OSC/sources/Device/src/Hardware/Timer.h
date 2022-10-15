#pragma once


#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Timer
 *  @brief ������ �������
 *  @{
 */

 /// ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32)
 /// � ����� ������� 120.000.000 ����� ��� �8-53 � 90.000.000 ����� ��� �8-54.
 /// ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
#define TIME_TICKS (TIM2->CNT)
#define TIME_US    (TIM2->CNT / 90)
#define TIME_MS    HAL_GetTick()


class Timer
{
public:

    struct Type
    {
        enum E
        {
            ShowLevelTrigLev,          ///< ����� �������������, ����� ���������� ��������� ����� ������ �������������
            FlashDisplay,              ///< ������ ��� ��������� �������� ������ �����-������ �������.
            ShowMessages,              ///< ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
            MenuAutoHide,              ///< ������ ��� ������� ������� ��������� ����.
            RShiftMarkersAutoHide,     ///< ������ ��� ������� ������� ��������� �������������� ������� �����.
            StopSound,                 ///< ��������� ����
            StrNaviAutoHide,           ///< ������� ������ ��������� ����
            TimerDisplay,              ///< ������ �� ������ ��������� ������
            Temp,
            Number
        };
    };


    static void Init();

    static void DeInit();
    /// ��������� ������� timer ������� � ����� ������������
    static void Set(Timer::Type::E type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(Timer::Type::E type, pFuncVV func, uint dTms);

    static void SetAndEnable(Timer::Type::E type, pFuncVV func, uint dTms);

    static void StartOnce(Timer::Type::E type);

    static void Enable(Timer::Type::E type);

    static void Disable(Timer::Type::E type);

    static bool IsRun(Timer::Type::E type);

    static void PauseOnTime(uint timeMS);

    static void PauseOnTicks(uint numTicks);
    /// ��������� ������� ��� ��������� ����� �������� �������
    static void StartMultiMeasurement();
    /// ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����
    static void StartLogging();

    static uint LogPointUS(char *name);

    static uint LogPointMS(char *name);
    /// ��������� �������. ���������� ������ �� ����������
    static void ElapsedCallback();
};


/** @}  @}
 */
