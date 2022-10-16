// 2022/02/01 11:50:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/**
  * TIME_TICKS - ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement().
  * � ����� ������� 60.000.000 �����. ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
  * ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32).
***/

#define TIMER_TICKS HAL_TIM2::GetTicks()
#define TICKS_IN_MS 60000               // ������� ����� ���������� � 1 ��
#define TICKS_IN_US 60                  // ������� ����� ���������� � 1 ���

#ifdef GUI
    #define TIMER_MS HAL_TIM::TimeMS()
#else
    extern volatile unsigned int uwTick;
    #define TIMER_MS              uwTick
#endif


namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    // ��������� ������� ��� ��������� ����� �������� �������.
    void StartMultiMeasurement();

    void DelayTicks(uint numTicks);

    void DelayUS(uint timeUS);

    // �������� �� timeMS �����������
    void DelayMS(uint timeMS);

    namespace Logging
    {
        // ������������� ��������� ����� ������������.
        // ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����.
        void Start();

        uint PointUS(char *name);

        uint PointMS(char *name);
    }
};
