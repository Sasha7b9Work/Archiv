#include "Settings.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "SettingsCursors.h"
#include "Utils/StringUtils.h"
#include "FPGA/FPGATypes.h"
#include "FPGA/FPGAMath.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float sCursors_GetCursPosU(Channel chan, int numCur)
{
    return CURS_POS_U(chan, numCur) / (Grid::ChannelBottom() == Grid::FullBottom() ? 1.0f : 2.0f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool sCursors_NecessaryDrawCursors()
{
    return ((!CURS_CNTRL_U_IS_DISABLE(CURS_SOURCE)) || (!CURS_CNTRL_T_IS_DISABLE(CURS_SOURCE))) && 
        (CURS_SHOW || Menu::GetNameOpenedPage() == PageSB_Cursors_Set);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* sCursors_GetCursVoltage(Channel source, int numCur, char buffer[20])
{
    float voltage = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, numCur), SET_RANGE(source), (uint16)SET_RSHIFT(source));
    return Voltage2String(voltage, true, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* sCursors_GetCursorTime(Channel source, int numCur, char buffer[20])
{
    float time = MathFPGA::TimeCursor(CURS_POS_T(source, numCur), SET_TBASE);
        
    return Time2String(time, true, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* sCursors_GetCursorPercentsU(Channel source, char buffer[20])
{
    buffer[0] = 0;
    float dPerc = DELTA_U100(source);
    float dValue = fabsf(sCursors_GetCursPosU(source, 0) - sCursors_GetCursPosU(source, 1));
    char bufferOut[20];
    char* percents = Float2String(dValue / dPerc * 100.0f, false, 5, bufferOut);
    strcat(buffer, percents);
    strcat(buffer, "%");
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* sCursors_GetCursorPercentsT(Channel source, char buffer[20])
{
    buffer[0] = 0;
    float dPerc = DELTA_T100(source);
    float dValue = fabsf(CURS_POS_T0(source) - CURS_POS_T1(source));
    char bufferOut[20];
    char* percents = Float2String(dValue / dPerc * 100.0f, false, 6, bufferOut);
    strcat(buffer, percents);
    strcat(buffer, "%");
    return buffer;
}
