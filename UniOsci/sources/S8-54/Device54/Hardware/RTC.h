#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup RTC
 *  @brief ������ � ������ ��������� �������
 *  @{
 */

class RTClock
{
public:
    /// �������������
    static void Init();
    
    static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    
    static PackedTime GetPackedTime();
    
    static void SetCorrection(int8 correction);
};

/** @}  @}
 */
