#pragma once
#include "Settings/Settings.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Sound
 *  @brief ������ �� �������� ���������
 *  @{
 */


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
    TypeWave(E v) : value(v) {};
    operator uint8() const { return (uint8)value; };
};


class Sound
{
public:
    static void Init();

    /// ���� ������� �� ������.
    static void ButtonPress();
    /// ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    static void ButtonRelease();

    static void GovernorChangedValue();
    
    static void RegulatorShiftRotate();

    static void RegulatorSwitchRotate();

    static void WarnBeepBad();

    static void WarnBeepGood();
    /// ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    static void WaitForCompletion();

    static void Beep(TypePress type);

    static void Test();

    static DAC_HandleTypeDef handleDAC;

private:

    static void Beep(const TypeWave newTypeWave, const float newFreq, const float newAmpl, const int newDuration);

    static void Stop();

    static void SetWave();

    static void CalculateMeandr();

    static uint16 CalculatePeriodForTIM();

    static void CalculateSine();

    static void CalculateTriangle();

    static void ConfigTIM7(uint16 prescaler, uint16 period);
};


/** @}  @}
 */
