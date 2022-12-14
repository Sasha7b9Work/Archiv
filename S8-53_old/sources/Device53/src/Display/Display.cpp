#include "defines.h"
#include "Log.h"
#include "Symbols.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Font/Font.h"
#include "Hardware/CPU.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/ProcessingSignal.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <limits.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_P2P_POINTS (FPGA_MAX_POINTS)
static uint8 dataP2P_0[NUM_P2P_POINTS];
static uint8 dataP2P_1[NUM_P2P_POINTS];
static int   lastP2Pdata = 0;
static bool  dataP2PIsEmpty = true;

#define MAX_NUM_STRINGS         35
#define SIZE_BUFFER_FOR_STRINGS 2000
static char                     *strings[MAX_NUM_STRINGS] = {0};
static char                     bufferForStrings[SIZE_BUFFER_FOR_STRINGS] = {0};
static int                      lastStringForPause = -1;

#define NUM_WARNINGS            10
static const char               *warnings[NUM_WARNINGS] = {0};      // ????? ??????????????? ?????????.
static uint                     timeWarnings[NUM_WARNINGS] = {0};   // ????? ?????, ????? ??????????????? ????????? ????????? ?? ?????.

static pFuncVV funcOnHand       = 0;
static pFuncVV funcAdditionDraw = 0;
static pFuncVV funcAfterDraw    = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void FuncOnTimerDisableShowLevelRShiftA();
static void FuncOnTimerDisableShowLevelRShiftB();
static void FuncOnTimerDisableShowLevelTrigLev();
static void FuncOnTimerRShiftMarkersAutoHide();
static void OnTimerShowWarning();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init() 
{
    /*
    PainterMem_SetBuffer(buffer, 100, 100);
    PainterMem_FillRect(0, 0, 99, 99, ColorChannel(A));
    PainterMem_DrawRectangle(10, 10, 10, 30, Color::FILL);
    PainterMem_DrawHLine(90, 10, 90, ColorChannel(B));
    PainterMem_DrawVLine(90, 10, 90, Color::BACK);
    PainterMem_DrawRectangle(0, 0, 99, 99, Color::FILL);
    */

    Painter::ResetFlash();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_11,
        GPIO_MODE_INPUT,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF0_MCO,
    };
    // ?????? ?????????? ???????  ? ?????? ???????
    HAL_GPIO_Init(GPIOG, &isGPIO);

    Painter::LoadFont(TypeFont_5);
    Painter::LoadFont(TypeFont_8);
    Painter::LoadFont(TypeFont_UGO);
    Painter::LoadFont(TypeFont_UGO2);
    Painter::SetFont(TypeFont_8);

    Painter::LoadPalette();
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawStringNavigation() 
{
    char buffer[100];
    char *string = Menu::StringNavigation(buffer);
    if(string) 
    {
        int length = Font::GetLengthText(string);
        int height = 10;
        Painter::DrawRectangle(Grid::Left(), GRID_TOP, length + 2, height, Color::FILL);
        Painter::FillRegion(Grid::Left() + 1, GRID_TOP + 1, length, height - 2, Color::BACK);
        Painter::DrawText(Grid::Left() + 2, GRID_TOP + 1, string, Color::FILL);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RotateRShift(Channel chan)
{
    ResetP2Ppoints(true);
    LAST_AFFECTED_CHANNEL = chan;
    if(TIME_SHOW_LEVELS)
    {
        if(chan == A)
        {
            gBF.showLevelRShift0 = 1;
        }
        else
        {
            gBF.showLevelRShift1 = 1;
        }
        Timer::SetAndStartOnce((chan == A) ? kShowLevelRShiftA : kShowLevelRShiftB, (chan == A) ? FuncOnTimerDisableShowLevelRShiftA :
                     FuncOnTimerDisableShowLevelRShiftB, (uint)(TIME_SHOW_LEVELS * 1000));
    };
    Display::Redraw();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnTimerDisableShowLevelRShiftA()
{
    Display::DisableShowLevelRShiftA();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnTimerDisableShowLevelRShiftB()
{
    Display::DisableShowLevelRShiftB();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RotateTrigLev()
{
    if (TIME_SHOW_LEVELS)
    {
        gBF.showLevelTrigLev = 1;
        Timer::SetAndStartOnce(kShowLevelTrigLev, FuncOnTimerDisableShowLevelTrigLev, (uint)(TIME_SHOW_LEVELS * 1000));
    }
    Display::Redraw();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnTimerDisableShowLevelTrigLev()
{
    Display::DisableShowLevelTrigLev();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Redraw()
{
    gBF.needFinishDraw = 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static bool ChannelNeedForDraw(const uint8 *data, Channel chan, const DataSettings *ds)
{
    if (!data)
    {
        return false;
    }

    if (MODE_WORK_IS_DIR)
    {
        if (!sChannel_Enabled(chan))
        {
            return false;
        }
    }
    else if (ds != 0)
    {
        if ((chan == A && ds->enableCh0 == 0) || (chan == B && ds->enableCh1 == 0))
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMarkersForMeasure(float scale, Channel chan)
{
    if (chan == Math)
    {
        return;
    }
    Painter::SetColor(Color::Cursors(chan));
    for(int numMarker = 0; numMarker < 2; numMarker++)
    {
        int pos = Processing::GetMarkerHorizontal(chan, numMarker);
        if(pos != ERROR_VALUE_INT && pos > 0 && pos < 200)
        {
            Painter::DrawDashedHLine((int)(Grid::FullBottom() - pos * scale), Grid::Left(), Grid::Right(), 3, 2, 0);
        }

        pos = Processing::GetMarkerVertical(chan, numMarker);
        if (pos != ERROR_VALUE_INT && pos > 0 && pos < Grid::Right())
        {
            Painter::DrawDashedVLine((int)(Grid::Left() + pos * scale), GRID_TOP, Grid::FullBottom(), 3, 2, 0);
        }
       
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
#define CONVERT_DATA_TO_DISPLAY(out, in)                \
    out = (uint8)(maxY - ((in) - MIN_VALUE) * scaleY);  \
    if(out < (uint8)minY)          { out = (uint8)minY; }             \
    else if (out > (uint8)maxY)    { out = (uint8)maxY; };


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalLined(const uint8 *data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX, 
    bool calculateFiltr)
{
    if (endPoint < startPoint)
    {
        return;
    }
    uint8 dataCD[281];

	int gridLeft = Grid::Left();
	int gridRight = Grid::Right();
    
    int numPoints = sMemory_GetNumPoints(false);
    int numSmoothing = sDisplay_NumPointSmoothing();

    if (ds->peakDet == PeakDet_Disabled)
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            float x0 = gridLeft + (i - startPoint) * scaleX;
            if (x0 >= gridLeft && x0 <= gridRight)
            {
                int index = i - startPoint;
                int y = calculateFiltr ? Math::CalculateFiltr(data, i, numPoints, numSmoothing) : data[i];
                CONVERT_DATA_TO_DISPLAY(dataCD[index], y);
            }
        }
    }
    else
    {
        int shift = ds->length1channel;

        int yMinNext = -1;
        int yMaxNext = -1;
       
        for (int i = startPoint; i < endPoint; i++)
        {
            float x = gridLeft + (i - startPoint) * scaleX;
            if (x >= gridLeft && x <= gridRight)
            {
                int yMin = yMinNext;
                if (yMin == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMin, data[i + shift]);
                }
                int yMax = yMaxNext;
                if (yMax == -1)
                {
                    CONVERT_DATA_TO_DISPLAY(yMax, data[i]);
                }

                CONVERT_DATA_TO_DISPLAY(yMaxNext, data[i + 1]);
                if (yMaxNext < yMin)
                {
                    yMin = yMaxNext + 1;
                }

                CONVERT_DATA_TO_DISPLAY(yMinNext, data[i + shift + 1]);
                if (yMinNext > yMax)
                {
                    yMax = yMinNext - 1;
                }

                Painter::DrawVLine((int)x, yMin, yMax);
            }
        }
    }

    if(endPoint - startPoint < 281)
    {
        int points = 281 - (endPoint - startPoint);
        for(int i = 0; i < points; i++)
        {
            int index = endPoint - startPoint + i;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], MIN_VALUE);
        }
    }

    if(ds->peakDet == PeakDet_Disabled)
    {
        CONVERT_DATA_TO_DISPLAY(dataCD[280], data[endPoint]);
        Painter::DrawSignal(Grid::Left(), dataCD, true);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSignalPointed(const uint8 *data, const DataSettings *ds, int startPoint, int endPoint, int minY, int maxY, float scaleY, float scaleX)
{
    int numPoints = sMemory_GetNumPoints(false);
    int numSmoothing = sDisplay_NumPointSmoothing();
    
    uint8 dataCD[281];

    if (scaleX == 1.0f) //-V550
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing));
        }
        Painter::DrawSignal(Grid::Left(), dataCD, false);

        if (ds->peakDet)
        {
            int size = endPoint - startPoint;
            startPoint += numPoints;
            endPoint = startPoint + size;;

            for (int i = startPoint; i < endPoint; i++)
            {
                int index = i - startPoint;
                CONVERT_DATA_TO_DISPLAY(dataCD[index], Math::CalculateFiltr(data, i, numPoints, numSmoothing));
            }
            Painter::DrawSignal(Grid::Left(), dataCD, false);
        }
    }
    else
    {
        for (int i = startPoint; i < endPoint; i++)
        {
            int index = i - startPoint;
            int dat = 0;
            CONVERT_DATA_TO_DISPLAY(dat, Math::CalculateFiltr(data, i, numPoints, numSmoothing));
            Painter::SetPoint((int)(Grid::Left() + index * scaleX), dat);
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataChannel(uint8 *data, Channel chan, DataSettings *ds, int minY, int maxY)
{
    bool calculateFiltr = true;
    if (data == 0)
    {
        calculateFiltr = false;
        if (chan == A)
        {
            Processing::GetData(&data, 0, &ds);
        }
        else
        {
            Processing::GetData(0, &data, &ds);
        }
    }

    if (!ChannelNeedForDraw(data, chan, ds))
    {
        return;
    }

    float scaleY = (float)(maxY - minY) / (MAX_VALUE - MIN_VALUE);
    float scaleX = (float)Grid::Width() / 280.0f;

    if(SHOW_MEASURES)
    {
        DrawMarkersForMeasure(scaleY, chan);
    }

    int firstPoint = 0;
    int lastPoint = 0;
    sDisplay_PointsOnDisplay(&firstPoint, &lastPoint);
    if(data == dataP2P_0 || data == dataP2P_1)
    {
        if(SELFRECORDER)
        {
            LOG_TRACE
        }
        else if(lastPoint > lastP2Pdata)
        {
            lastPoint = lastP2Pdata;
        }
    }

    Painter::SetColor(Color::CHAN[chan]);
    if(MODE_DRAW_SIGNAL_IS_LINES)
    {
        /*
        if (set.display.numAveraging > NumAveraging_1)
        {
            Painter::SetColor(ColorGrid());
            DrawSignalLined(DS_GetData(chan, 0), ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX, calculateFiltr);    // WARN
        }
        Painter::SetColor(ColorChannel(chan));
        */
        DrawSignalLined(data, ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX, calculateFiltr);
    }
    else
    {
        DrawSignalPointed(data, ds, firstPoint, lastPoint, minY, maxY, scaleY, scaleX);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawMath()
{
    if (FUNC_MODE_DRAW_IS_DISABLED || Storage::GetData(A, 0) == 0 || Storage::GetData(B, 0) == 0)
    {
        return;
    }

    uint8 *dataRel0 = 0;
    uint8 *dataRel1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&dataRel0, &dataRel1, &ds);

    float dataAbs0[FPGA_MAX_POINTS];
    float dataAbs1[FPGA_MAX_POINTS];

    MathFPGA::PointsRel2Voltage(dataRel0, ds->length1channel, ds->range[A], ds->rShiftCh0, dataAbs0);
    MathFPGA::PointsRel2Voltage(dataRel1, ds->length1channel, ds->range[B], ds->rShiftCh1, dataAbs1);

    Math::CalculateMathFunction(dataAbs0, dataAbs1, ds->length1channel);
    
    uint8 points[FPGA_MAX_POINTS];
    MathFPGA::PointsVoltage2Rel(dataAbs0, ds->length1channel, SET_RANGE_MATH, SET_RSHIFT_MATH, points);

    DrawDataChannel(points, Math, ds, Grid::MathTop(), Grid::MathBottom());

    static const int WIDTH = 71;
    static const int HEIGHT = 10;
    int delta = (SHOW_STRING_NAVIGATION && FUNC_MODE_DRAW_IS_TOGETHER) ? 10 : 0;
    Painter::DrawRectangle(Grid::Left(), Grid::MathTop() + delta, WIDTH, HEIGHT, Color::FILL);
    Painter::FillRegion(Grid::Left() + 1, Grid::MathTop() + 1 + delta, WIDTH - 2, HEIGHT - 2, Color::BACK);
    Divider multiplier = MATH_MULTIPLIER;
    Painter::DrawText(Grid::Left() + 2, Grid::MathTop() + 1 + delta, sChannel_Range2String(SET_RANGE_MATH, multiplier), Color::FILL);
    Painter::DrawText(Grid::Left() + 25, Grid::MathTop() + 1 + delta, ":");
    char buffer[20];
    Painter::DrawText(Grid::Left() + 27, Grid::MathTop() + 1 + delta, sChannel_RShift2String(SET_RSHIFT_MATH, SET_RANGE_MATH, multiplier, buffer));


}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSpectrumChannel(const float *spectrum, Color color)
{
    Painter::SetColor(color);
	int gridLeft = Grid::Left();
	int gridBottom = Grid::MathBottom();
	int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++) 
    {
        Painter::DrawVLine(gridLeft + i, gridBottom, (int)(gridBottom - gridHeight * spectrum[i]));
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteParametersFFT(Channel chan, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Painter::SetColor(Color::FILL);
    Painter::DrawText(x, y, Freq2String(freq0, false, buffer));
    y += dY;
    Painter::DrawText(x, y, Freq2String(freq1, false, buffer));
    if (chan == A)
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }
    Painter::SetColor(Color::CHAN[chan]);
    Painter::DrawText(x, y, SCALE_FFT_IS_LOG ? Db2String(density0, 4, buffer) : Float2String(density0, false, 7, buffer));
    y += dY;
    Painter::DrawText(x, y, SCALE_FFT_IS_LOG ? Db2String(density1, 4, buffer) : Float2String(density1, false, 7, buffer));
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DRAW_SPECTRUM(const uint8 *data, int numPoints, Channel channel)
{
    if (!sChannel_Enabled(channel))
    {
        return;
    }
    float dataR[FPGA_MAX_POINTS];
    float spectrum[FPGA_MAX_POINTS];

    float freq0 = 0.0f;
    float freq1 = 0.0f;
    float density0 = 0.0f;
    float density1 = 0.0f;
    int y0 = 0;
    int y1 = 0;
    int s = 2;

    MathFPGA::PointsRel2Voltage(data, numPoints, gDSet->range[channel], (int16)(channel == A ? gDSet->rShiftCh0 : gDSet->rShiftCh1), dataR);
    MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);
    DrawSpectrumChannel(spectrum, Color::CHAN[channel]);
    if (!MENU_IS_SHOWN || Menu::IsMinimize())
    {
        Color color = Color::FILL;
        WriteParametersFFT(channel, freq0, density0, freq1, density1);
        Painter::DrawRectangle(FFT_POS_CURSOR_0 + Grid::Left() - s, y0 - s, s * 2, s * 2, color);
        Painter::DrawRectangle(FFT_POS_CURSOR_1 + Grid::Left() - s, y1 - s, s * 2, s * 2);

        Painter::DrawVLine(Grid::Left() + FFT_POS_CURSOR_0, Grid::MathBottom(), y0 + s);
        Painter::DrawVLine(Grid::Left() + FFT_POS_CURSOR_1, Grid::MathBottom(), y1 + s);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawSpectrum()
{
    if (!FFT_ENABLED)
    {
        return;
    }

    Painter::DrawVLine(Grid::Right(), Grid::ChannelBottom() + 1, Grid::MathBottom() - 1, Color::BACK);

    if (MODE_WORK_IS_DIR)
    {
        int numPoints = sMemory_GetNumPoints(false);
        if (numPoints < 512)
        {
            numPoints = 256;
        }

        if (SOURCE_FFT_IS_A)
        {
            DRAW_SPECTRUM(gData0, numPoints, A);
        }
        else if (SOURCE_FFT_IS_B)
        {
            DRAW_SPECTRUM(gData1, numPoints, B);
        }
        else
        {
            if (LAST_AFFECTED_CHANNEL_IS_A)
            {
                DRAW_SPECTRUM(gData1, numPoints, B);
                DRAW_SPECTRUM(gData0, numPoints, A);
            }
            else
            {
                DRAW_SPECTRUM(gData0, numPoints, A);
                DRAW_SPECTRUM(gData1, numPoints, B);
            }
        }
    }

    Painter::DrawHLine(Grid::ChannelBottom(), Grid::Left(), Grid::Right(), Color::FILL);
    Painter::DrawHLine(Grid::MathBottom(), Grid::Left(), Grid::Right());
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawBothChannels(uint8 *data0, uint8 *data1)
{
	if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(data0, A, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(data1, B, gDSet, GRID_TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(data1, B, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(data0, A, gDSet, GRID_TOP, Grid::ChannelBottom());
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataMemInt()
{
    if(gDSmemInt != 0)
     {
        DrawDataChannel(gData0memInt, A, gDSmemInt, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(gData1memInt, B, gDSmemInt, GRID_TOP, Grid::ChannelBottom());
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataInModeWorkLatest()
{
    if (gDSmemLast != 0)
    {
        DrawDataChannel(gData0memLast, A, gDSmemLast, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(gData1memLast, B, gDSmemLast, GRID_TOP, Grid::ChannelBottom());
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataInModePoint2Point()
{
    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&data0, &data1, &ds);

    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        if (SELFRECORDER || !Storage::NumElementsWithCurrentSettings())
        {
            DrawDataChannel(dataP2P_0, A, ds, GRID_TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_1, B, ds, GRID_TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data0, A, ds, GRID_TOP, Grid::ChannelBottom());
            DrawDataChannel(data1, B, ds, GRID_TOP, Grid::ChannelBottom());
        }
    }
    else
    {
        if (SELFRECORDER || !Storage::NumElementsWithCurrentSettings())
        {
            DrawDataChannel(dataP2P_1, B, ds, GRID_TOP, Grid::ChannelBottom());
            DrawDataChannel(dataP2P_0, A, ds, GRID_TOP, Grid::ChannelBottom());
        }
        else
        {
            DrawDataChannel(data1, B, ds, GRID_TOP, Grid::ChannelBottom());
            DrawDataChannel(data0, A, ds, GRID_TOP, Grid::ChannelBottom());
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDataInModeSelfRecorder()
{
    LOG_TRACE
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::DrawDataInModeNormal()
{
    static void* prevAddr = 0;
    bool retValue = true;

    uint8 *data0 = 0;
    uint8 *data1 = 0;
    DataSettings *ds = 0;
    Processing::GetData(&data0, &data1, &ds);

    int16 numSignals = (int16)Storage::NumElementsWithSameSettings();
    Limitation<int16>(&numSignals, 1, (int16)NUM_ACCUM);
    if (numSignals == 1 || ENUM_ACCUM_IS_INFINITY || MODE_ACCUM_IS_RESET || sTime_RandomizeModeEnabled())
    {
        DrawBothChannels(0, 0);
        if (prevAddr == 0 || prevAddr != ds->addrPrev)
        {
            gBF.numDrawingSignals++;
            prevAddr = ds->addrPrev;
        }
    }
    else
    {
        for (int i = 0; i < numSignals; i++)
        {
            DrawBothChannels(Storage::GetData(A, i), Storage::GetData(B, i));
        }
    }

    return retValue;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataMinMax()
{
    ModeDrawSignal modeDrawSignalOld = MODE_DRAW_SIGNAL;
    MODE_DRAW_SIGNAL = ModeDrawSignal_Lines;
    if (LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawDataChannel(Storage::GetLimitation(A, 0), A, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(A, 1), A, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(B, 0), B, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(B, 1), B, gDSet, GRID_TOP, Grid::ChannelBottom());
    }
    else
    {
        DrawDataChannel(Storage::GetLimitation(B, 0), B, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(B, 1), B, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(A, 0), A, gDSet, GRID_TOP, Grid::ChannelBottom());
        DrawDataChannel(Storage::GetLimitation(A, 1), A, gDSet, GRID_TOP, Grid::ChannelBottom());
    }
    MODE_DRAW_SIGNAL = modeDrawSignalOld;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::DrawDataNormal()
{
    bool retValue = true;
    if (!dataP2PIsEmpty)
    {
        static const pFuncVV funcs[2] = {DrawDataInModePoint2Point, DrawDataInModeSelfRecorder};
        funcs[(int)SELFRECORDER]();
    }
    else
    {
        retValue = DrawDataInModeNormal();
    }
    return retValue;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::DrawData()
{
    bool retValue = true;

    if (Storage::AllDatas())
    {

        if (MODE_WORK_IS_ROM)
        {
            if (!MODE_SHOW_MEMINT_IS_DIRECT)
            {
                DrawDataMemInt();
            }
            if (!MODE_SHOW_MEMINT_IS_SAVED)
            {
                DrawDataNormal();
            }
        }
        else if (MODE_WORK_IS_RAM)
        {
            DrawDataInModeWorkLatest();
        }
        else
        {
            if (gMemory.showAlways == 1)
            {
                DrawDataMemInt();
            }
            retValue = DrawDataNormal();
        }

        if (NUM_MIN_MAX != 1)
        {
            DrawDataMinMax();
        }
    }

    Painter::DrawRectangle(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight(), Color::FILL);

    return retValue;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = CPU::RTC_::GetPackedTime();
    
    char buffer[20];
    
    Painter::SetColor(Color::FILL);
    
    if (MODE_WORK_IS_ROM || MODE_WORK_IS_RAM)
    {
        DataSettings *ds = MODE_WORK_IS_ROM ? gDSmemInt : gDSmemLast;

        if (ds != 0)
        {
            y -= 9;
            time.day = ds->time.day;
            time.hours = ds->time.hours;
            time.minutes = ds->time.minutes;
            time.seconds = ds->time.seconds;
            time.month = ds->time.month;
            time.year = ds->time.year;
            Painter::DrawText(x, y, Int2String(time.day, false, 2, buffer));
            Painter::DrawText(x + dField, y, ":");
            Painter::DrawText(x + dField + dSeparator, y, Int2String(time.month, false, 2, buffer));
            Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
            Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String(time.year + 2000, false, 4, buffer));
            y += 9;
        }
        else
        {
            return;
        }
    }
    
    
    Painter::DrawText(x, y, Int2String(time.hours, false, 2, buffer));
    Painter::DrawText(x + dField, y, ":");
    Painter::DrawText(x + dField + dSeparator, y, Int2String(time.minutes, false, 2, buffer));
    Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
    Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String(time.seconds, false, 2, buffer));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawHiPart()
{
    WriteCursors();
    DrawHiRightPart();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawDataInRect(int x, int width, const uint8 *data, int numElems, Channel chan, int shiftForPeakDet)
{
    if(numElems == 0)
    {
        return;
    }

    width--;
    float elemsInColumn = (float)numElems / (float)width;
    uint8 min[300];
    uint8 max[300];


    if (SET_TBASE >= TBase_20ms && SET_PEAKDET)
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + 1];
            for (int elem = (int)firstElem + 2; elem <= (int)lastElem; elem += 2)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + 1], max[col]);
            }
        }
    }
    else if (shiftForPeakDet == 0)
    {
        uint8 *iMin = &min[0];
        uint8 *iMax = &max[0];

        for (int col = 0; col < width; col++, iMin++, iMax++)
        {
            int firstElem = (int)(col * elemsInColumn);
            int lastElem = firstElem + (int)elemsInColumn - 1;
            *iMin = data[firstElem];
            *iMax = data[firstElem];
            for (int elem = firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], *iMin);
                SET_MAX_IF_LARGER(data[elem], *iMax);
            }
        }
    }
    else        // ? ????? ?????, ???? ???. ???. ???????
    {
        for (int col = 0; col < width; col++)
        {
            float firstElem = col * elemsInColumn;
            float lastElem = firstElem + elemsInColumn - 1;
            min[col] = data[(int)firstElem];
            max[col] = data[(int)firstElem + shiftForPeakDet];
            for (int elem = (int)firstElem + 1; elem <= lastElem; elem++)
            {
                SET_MIN_IF_LESS(data[elem], min[col]);
                SET_MAX_IF_LARGER(data[elem + shiftForPeakDet], max[col]);
            }
        }
    }

    int bottom = 16;
    int height = 14;
    float scale = (float)height / (float)(MAX_VALUE - MIN_VALUE);

#define ORDINATE(x) ((uint8)(bottom - scale * LimitationRet<uint8>((uint8)(x - MIN_VALUE), 0, 200)))

#define NUM_POINTS (300 * 2)
    uint8 points[NUM_POINTS];

    points[0] = ORDINATE(max[0]);
    points[1] = ORDINATE(min[0]);

    for(int i = 1; i < width; i++)
    {
        int value0 = min[i] > max[i - 1] ? max[i - 1] : min[i];
        int value1 = max[i] < min[i - 1] ? min[i - 1] : max[i];
        points[i * 2] = ORDINATE(value1);
        points[i * 2 + 1] = ORDINATE(value0);
    }
	if(width < 256)
    {
		Painter::DrawVLineArray(x, width, points, Color::CHAN[chan]);
	}
    else
    {
		Painter::DrawVLineArray(x, 255, points, Color::CHAN[chan]);
		Painter::DrawVLineArray(x + 255, width - 255, points + 255 * 2, Color::CHAN[chan]);
	}
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawChannelInWindowMemory(int timeWindowRectWidth, int xVert0, int xVert1, int startI, int endI, const uint8 *data, int rightX, 
    Channel chan, int shiftForPeakDet)
{
    if(data == dataP2P_0 && data == dataP2P_1)
    {

    }
    else
    {
        DrawDataInRect(1,          xVert0 - 1,              &(data[0]),        startI,                             chan, shiftForPeakDet);
        DrawDataInRect(xVert0 + 2, timeWindowRectWidth - 2, &(data[startI]),   281,                                chan, shiftForPeakDet);
        DrawDataInRect(xVert1 + 2, rightX - xVert1 + 2,     &(data[endI + 1]), sMemory_GetNumPoints(false) - endI, chan, shiftForPeakDet);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawMemoryWindow()
{
    uint8 *dat0 = gData0memInt;
    uint8 *dat1 = gData1memInt;
    DataSettings *ds = gDSmemInt;
    
    if(MODE_WORK_IS_DIR || MODE_WORK_IS_RAM)
    {
        dat0 = gData0;
        dat1 = gData1;
        ds = gDSet;
    }
    
    int leftX = 3;
    int top = 1;
    int height = GRID_TOP - 3;
    int bottom = top + height;

    static const int rightXses[3] = {276, 285, 247};
    int rightX = rightXses[MODE_WORK];
    if (sCursors_NecessaryDrawCursors())
    {
        rightX = 68;
    }

    int timeWindowRectWidth = (int)((rightX - leftX) * (282.0f / sMemory_GetNumPoints(false)));
    float scaleX = (float)(rightX - leftX + 1) / sMemory_GetNumPoints(false);

    int16 shiftInMemory = SHIFT_IN_MEMORY;
    
    int startI = shiftInMemory;
    int endI = startI + 281;

    const int xVert0 = (int)(leftX + shiftInMemory * scaleX);
    const int xVert1 = (int)(leftX + shiftInMemory * scaleX + timeWindowRectWidth);
    bool showFull = set.display.showFullMemoryWindow;
    Painter::DrawRectangle(xVert0, top + (showFull ? 0 : 1), xVert1 - xVert0, bottom - top - (showFull ? 0 : 2), Color::FILL);

    if(!dataP2PIsEmpty)
    {
        dat0 = dataP2P_0;
        dat1 = dataP2P_1;
    }

    if (showFull)
    {
        if (gData0 || gData1 || (!dataP2PIsEmpty))
        {
            Channel chanFirst = LAST_AFFECTED_CHANNEL_IS_A ? B : A;
            Channel chanSecond = LAST_AFFECTED_CHANNEL_IS_A ? A : B;
            const uint8 *dataFirst = LAST_AFFECTED_CHANNEL_IS_A ? dat1 : dat0;
            const uint8 *dataSecond = LAST_AFFECTED_CHANNEL_IS_A ? dat0 : dat1;

            int shiftForPeakDet = ds->peakDet == PeakDet_Disabled ? 0 : ds->length1channel;

            if (ChannelNeedForDraw(dataFirst, chanFirst, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataFirst, rightX, chanFirst, shiftForPeakDet);
            }
            if (ChannelNeedForDraw(dataSecond, chanSecond, ds))
            {
                DrawChannelInWindowMemory(timeWindowRectWidth, xVert0, xVert1, startI, endI, dataSecond, rightX, chanSecond, shiftForPeakDet);
            }
        }
    }
    else
    {
        Painter::DrawVLine(leftX - 2, top, bottom, Color::FILL);
        Painter::DrawVLine(rightX + 2, top, bottom);
        Painter::DrawHLine((bottom + top) / 2 - 3, leftX, xVert0 - 2);
        Painter::DrawHLine((bottom + top) / 2 + 3, leftX, xVert0 - 2);
        Painter::DrawHLine((bottom + top) / 2 + 3, xVert1 + 2, rightX);
        Painter::DrawHLine((bottom + top) / 2 - 3, xVert1 + 2, rightX);
    }

    int x[] = {leftX, (rightX - leftX) / 2 + leftX + 1, rightX};
    int x0 = x[TPOS];

    // ?????? TPos
    Painter::FillRegion(x0 - 3, 9, 6, 6, Color::BACK);
    Painter::DrawChar(x0 - 3, 9, SYMBOL_TPOS_1, Color::FILL);

    // ?????? tShift
    float scale = (float)(rightX - leftX + 1) / ((float)sMemory_GetNumPoints(false) - (sMemory_GetNumPoints(false) == 281 ? 1 : 0));
    float xShift = 1 + (sTime_TPosInPoints((PeakDetMode)gDSet->peakDet, (int)gDSet->length1channel, TPOS) - sTime_TShiftInPoints((PeakDetMode)gDSet->peakDet)) * scale;

    Painter::FillRegion((int)xShift - 1, 3, 6, 6, Color::BACK);
    Painter::FillRegion((int)xShift, 4, 4, 4, Color::FILL);
    Painter::SetColor(Color::BACK);
    if(xShift < leftX - 2)
    {
        xShift = (float)(leftX - 2);
        Painter::DrawLine((int)xShift + 3, 5, (int)xShift + 3, 7);
        Painter::DrawLine((int)xShift + 1, 6, (int)xShift + 2, 6);
    }
    else if(xShift > rightX - 1)
    {
        xShift = (float)(rightX - 2);
        Painter::DrawLine((int)xShift + 1, 5, (int)xShift + 1, 7);
        Painter::DrawLine((int)xShift + 2, 6, (int)xShift + 3, 6);
    }
    else
    {
        Painter::DrawLine((int)xShift + 1, 5, (int)xShift + 3, 5);
        Painter::DrawLine((int)xShift + 2, 6, (int)xShift + 2, 7);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteCursors()
{
    char buffer[20];
    int startX = 43;
    if(MODE_WORK_IS_DIR)
    {
        startX += 29;
    }
    int x = startX;
    int y1 = 0;
    int y2 = 9;
    if(sCursors_NecessaryDrawCursors())
    {
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 3;
        Channel source = CURS_SOURCE;
        Color colorText = Color::CHAN[source];
        if(!CURS_CNTRL_U_IS_DISABLE(source))
        {
            Painter::DrawText(x, y1, "1:", colorText);
            Painter::DrawText(x, y2, "2:");
            x += 7;
            Painter::DrawText(x, y1, sCursors_GetCursVoltage(source, 0, buffer));
            Painter::DrawText(x, y2, sCursors_GetCursVoltage(source, 1, buffer));
            x = startX + 49;
            float pos0 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 0), SET_RANGE(source), (uint16)SET_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(sCursors_GetCursPosU(source, 1), SET_RANGE(source), (uint16)SET_RSHIFT(source));
            float delta = fabsf(pos1 - pos0);
            Painter::DrawText(x, y1, ":dU=");
            Painter::DrawText(x + 17, y1, Voltage2String(delta, false, buffer));
            Painter::DrawText(x, y2, ":");
            Painter::DrawText(x + 10, y2, sCursors_GetCursorPercentsU(source, buffer));
        }

        x = startX + 101;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 3;
        if(!CURS_CNTRL_T_IS_DISABLE(source))
        {
            Painter::SetColor(colorText);
            Painter::DrawText(x, y1, "1:");
            Painter::DrawText(x, y2, "2:");
            x+=7;
            Painter::DrawText(x, y1, sCursors_GetCursorTime(source, 0, buffer));
            Painter::DrawText(x, y2, sCursors_GetCursorTime(source, 1, buffer));
            x = startX + 153;
            float pos0 = MathFPGA::TimeCursor(CURS_POS_T0(source), SET_TBASE);
            float pos1 = MathFPGA::TimeCursor(CURS_POS_T1(source) , SET_TBASE);
            float delta = fabsf(pos1 - pos0);
            Painter::DrawText(x, y1, ":dT=");
            Painter::DrawText(x + 17, y1, Time2String(delta, false, buffer));
            Painter::DrawText(x, y2, ":");
            Painter::DrawText(x + 8, y2, sCursors_GetCursorPercentsT(source, buffer ));

            if(CURSORS_SHOW_FREQ)
            {
                int width = 65;
                x = Grid::Right() - width;
                Painter::DrawRectangle(x, GRID_TOP, width, 12, Color::FILL);
                Painter::FillRegion(x + 1, GRID_TOP + 1, width - 2, 10, Color::BACK);
                Painter::DrawText(x + 1, GRID_TOP + 2, "1/dT=", colorText);
                Painter::DrawText(x + 25, GRID_TOP + 2, Freq2String(1.0f / delta, false, buffer));
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawHiRightPart()
    {
    // ?????????????
    int y = 2;

    static const int xses[3] = {280, 271, 251};
    int x = xses[MODE_WORK];

    if (!MODE_WORK_IS_RAM)
    {
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);

        x += 2;

        if (gBF.trigEnable == 1)
        {
            Painter::FillRegion(x, 1 + y, GRID_TOP - 3, GRID_TOP - 7);
            Painter::DrawText(x + 3, 3 + y, set.common.lang == Russian ? "??" : "Tr", Color::BACK);
        }
    }

    // ????? ??????
    static const char *strings_[][2] =
    {
        {"???",     "MEAS"},
        {"????",    "LAST"},
        {"????",    "INT"}
    };

    if(!MODE_WORK_IS_DIR)
    {
        x += 18;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);
        x += 2;
        Painter::DrawText(set.common.lang == Russian ? x : x + 3, -1, set.common.lang == Russian ? "?????" : "mode");
        Painter::DrawStringInCenterRect(x + 1, 9, 25, 8, strings_[MODE_WORK][set.common.lang]);
    }
    else
    {
        x -= 9;
    }

    if (!MODE_WORK_IS_RAM)
    {

        x += 27;
        Painter::DrawVLine(x, 1, GRID_TOP - 2, Color::FILL);

        x += 2;
        y = 1;
        if (FPGA::CurrentStateWork() == StateWorkFPGA_Work)
        {
            Painter::Draw4SymbolsInRect(x, 1, SYMBOL_PLAY);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA_Stop)
        {
            Painter::FillRegion(x + 3, y + 3, 10, 10);
        }
        else if (FPGA::CurrentStateWork() == StateWorkFPGA_Wait)
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Painter::FillRegion(x, y + 1, w, h);
            Painter::FillRegion(x + w + delta, y + 1, w, h);
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursorsRShift()
{
    if (!FUNC_MODE_DRAW_IS_DISABLED)
    {
        DrawCursorRShift(Math);
    }
    if(LAST_AFFECTED_CHANNEL_IS_B)
    {
        DrawCursorRShift(A);
        DrawCursorRShift(B);
    }
    else
    {
        DrawCursorRShift(B);
        DrawCursorRShift(A);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static bool NeedForClearScreen()
{
    int numAccum = NUM_ACCUM;
    if (sTime_RandomizeModeEnabled() || numAccum == 1 || MODE_ACCUM_IS_NORESET || SELFRECORDER)
    {
        return true;
    }
    if (gBF.needFinishDraw == 1)
    {
        gBF.needFinishDraw = 0;
        return true;
    }
    if (MODE_ACCUM_IS_RESET && gBF.numDrawingSignals >= (uint)numAccum)
    {
        gBF.numDrawingSignals = 0;
        return true;
    }
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint8 *pool;


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
	uint timeStart = TIME_TICKS;
    if (funcOnHand != 0)
    {
        funcOnHand();
        return;
    }

    bool needClear = NeedForClearScreen();

    if (needClear)
    {
        Painter::BeginScene(Color::BACK);
        DrawMemoryWindow();
        DrawFullGrid();
    }

    DrawData();

    if (needClear)
    {
        DrawMath();
        DrawSpectrum();
        DrawCursors();
        DrawHiPart();
        DrawLowPart();
        DrawCursorsWindow();
        DrawCursorTrigLevel();
        DrawCursorsRShift();
        DrawMeasures();
        DrawStringNavigation();
        DrawCursorTShift();
    }
    
    Menu::Draw();

    if (needClear)
    {
        DrawWarnings();

        if (funcAdditionDraw)
        {
            funcAdditionDraw();
        }
    }

    DrawConsole();

    if (needClear)    
    {
        WriteValueTrigLevel();
    }

    DrawTimeForFrame(TIME_TICKS - timeStart);

    Painter::SetColor(Color::FILL);

    Painter::EndScene();

    if (gMemory.needForSaveToFlashDrive == 1)
    {
        if (Painter::SaveScreenToFlashDrive())
        {
            Display::ShowWarningGood(FileIsSaved);
        }
        gMemory.needForSaveToFlashDrive = 0;
    }

    if (funcAfterDraw)
    {
        funcAfterDraw();
        funcAfterDraw = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteValueTrigLevel()
{
    if (gBF.showLevelTrigLev == 1 && MODE_WORK_IS_DIR)
    {
        float trigLev = RSHIFT_2_ABS(SET_TRIGLEV_SOURCE, SET_RANGE(TRIGSOURCE));     // WARN ????? ??? ??????? ????????????? ??????????? ?????????????? ???????.
        TrigSource trigSource = TRIGSOURCE;
        if (TRIG_INPUT_IS_AC && trigSource <= TrigSource_B)
        {
            int16 rShift = SET_RSHIFT(trigSource);
            float rShiftAbs = RSHIFT_2_ABS(rShift, SET_RANGE(trigSource));
            trigLev += rShiftAbs;
        }
        char buffer[20];
        strcpy(buffer, LANG_RU ? "?? ????? = " : "Trig lvl = ");
        char bufForVolt[20];
        strcat(buffer, Voltage2String(trigLev, true, bufForVolt));
        int width = 96;
        int x = (Grid::Width() - width) / 2 + Grid::Left();
        int y = Grid::BottomMessages() - 20;
        Painter::DrawRectangle(x, y, width, 10, Color::FILL);
        Painter::FillRegion(x + 1, y + 1, width - 2, 8, Color::BACK);
        Painter::DrawText(x + 2, y + 1, buffer, Color::FILL);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static const char *strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[MAX_DB_FFT];
        float scale = (float)Grid::MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = (int)(Grid::MathTop() + i * scale);
            Painter::DrawHLine(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Painter::SetColor(Color::FILL);
                Painter::DrawText(3, y - 4, strs[i]);
            }
        }
        if (!Menu::IsMinimize())
        {
            Painter::SetColor(Color::FILL);
            Painter::DrawText(5, Grid::MathTop() + 1, "??");
        }
    }
    else if (SCALE_FFT_IS_LINEAR)
    {
        static const char *strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)Grid::MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = (int)(Grid::MathTop() + i * scale);
            Painter::DrawHLine(y, Grid::Left(), Grid::Left() + 256, Color::GRID);
            if (!Menu::IsMinimize())
            {
                Painter::DrawText(5, y - 4, strs[i], Color::FILL);
            }
        }
    }
    Painter::DrawVLine(Grid::Left() + 256, Grid::MathTop(), Grid::MathBottom(), Color::FILL);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawFullGrid()
{
    if (sDisplay_IsSeparate())
    {
        DrawGrid(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight() / 2);
        if (FFT_ENABLED)
        {
            DrawGridSpectrum();
        }
        if (!FUNC_MODE_DRAW_IS_DISABLED)
        {
            DrawGrid(Grid::Left(), GRID_TOP + Grid::FullHeight() / 2, Grid::Width(), Grid::FullHeight() / 2);
        }
        Painter::DrawHLine(GRID_TOP + Grid::FullHeight() / 2, Grid::Left(), Grid::Left() + Grid::Width(), Color::FILL);
    }
    else
    {
        DrawGrid(Grid::Left(), GRID_TOP, Grid::Width(), Grid::FullHeight());
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateCountV()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_1_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 42 : 44;
        }
        if (NUM_MEASURES_IS_2_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 69 : 39;
        }
        if (NUM_MEASURES_IS_3_5)
        {
            return SOURCE_MEASURE_IS_A_B ? 54 : 68;
        }
    }

    return 49;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateCountH()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_6_1)
        {
            return 73;
        }
        if (NUM_MEASURES_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = (uint16)(left + deltaX * i);
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = (uint16)(centerX - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = (uint16)(centerX + deltaX * (i - 9));
    }
    masX[16] = (uint16)(right - 1);

    Painter::DrawMultiVPointLine(17, (int)(top + stepY), masX, (int)stepY, CalculateCountV(), Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = (uint8)(top + deltaY * i);
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = (uint8)(centerY - 6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = (uint8)(centerY + deltaY * (i - 7));
    }
    mas[12] = (uint8)(bottom - 1);

    Painter::DrawMultiHPointLine(13, (int)(left + stepX), mas, (int)stepX, CalculateCountH(), Color::GRID);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{ 
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + deltaX * i);
    }
    masX[14] = (uint16)(right - 1);
    Painter::DrawMultiVPointLine(15, top + stepY, masX, stepY, CalculateCountV(), Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + deltaY * i);
    }
    mas[10] = (uint8)(bottom - 1);
    Painter::DrawMultiHPointLine(11, left + stepX, mas, stepX, CalculateCountH(), Color::GRID);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX, int stepY)
{
    Painter::DrawHPointLine(centerY, left + stepX, right, (float)stepX);
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), 
        (uint8)(bottom - 2), (uint8)(bottom - 1)};
    Painter::DrawMultiHPointLine(6, left + deltaX, masY, deltaX, (right - top) / deltaX, Color::GRID);
    Painter::DrawVPointLine(centerX, top + stepY, bottom, (float)stepY);
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), 
        (uint16)(right - 2), (uint16)(right - 1)};
    Painter::DrawMultiVPointLine(6, top + deltaY, masX, deltaY, (bottom - top) / deltaY, Color::GRID);

}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawGrid(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Painter::SetColor(Color::FILL);

    if (top == GRID_TOP)
    {
        Painter::DrawHLine(top, 1, left - 2);
        Painter::DrawHLine(top, right + 2, SCREEN_WIDTH - 2);

        if (!Menu::IsMinimize() || !MENU_IS_SHOWN)
        {
            Painter::DrawVLine(1, top + 2, bottom - 2);
            Painter::DrawVLine(318, top + 2, bottom - 2);
        }
    }

    float deltaX = Grid::DeltaX() *(float)width / width;
    float deltaY = Grid::DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;
    
    int centerX = left + width / 2;
    int centerY = top + height / 2;

    Painter::SetColor(Color::GRID);
    if (TYPE_GRID_IS_1)
    {
        DrawGridType1(left, top, right, bottom, (float)centerX, (float)centerY, deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_IS_2)
    {
        DrawGridType2(left, top, right, bottom, (int)deltaX, (int)deltaY, (int)stepX, (int)stepY);
    }
    else if (TYPE_GRID_IS_3)
    {
        DrawGridType3(left, top, right, bottom, centerX, centerY, (int)deltaX, (int)deltaY, (int)stepX, (int)stepY);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  DELTA 5

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawScaleLine(int x, bool forTrigLev)
{
    if(ALT_MARKERS_HIDE)
    {
        return;
    }
    int width = 6;
    int topY = GRID_TOP + DELTA;
    int x2 = width + x + 2;
    int bottomY  = Grid::ChannelBottom() - DELTA;
    int centerY = (Grid::ChannelBottom() + GRID_TOP) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };
    for(int i = 0; i < 5; i++)
    {
        Painter::DrawLine(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursorsWindow()
{
    if((!Menu::IsMinimize() || !MENU_IS_SHOWN) && gBF.drawRShiftMarkers == 1)
    {
        DrawScaleLine(2, false);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursorTrigLevel()
{
    TrigSource chan = TRIGSOURCE;
    if (chan == TrigSource_Ext)
    {
        return;
    }
    int trigLev = SET_TRIGLEV(chan) + ((chan == TrigSource_Ext) ? 0 : SET_RSHIFT(chan) - RShiftZero);
    float scale = 1.0f / ((TrigLevMax - TrigLevMin) / 2 / Grid::ChannelHeight());
    int y0 = (int)((GRID_TOP + Grid::ChannelBottom()) / 2 + scale * (TrigLevZero - TrigLevMin));
    int y = (int)(y0 - scale * (trigLev - TrigLevMin));

    if(chan != TrigSource_Ext)
    {
        y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();
    }

    int x = Grid::Right();
    Painter::SetColor(Color::Trig());
    if(y > Grid::ChannelBottom())
    {
        Painter::DrawChar(x + 3, Grid::ChannelBottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Painter::SetPoint(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if(y < GRID_TOP)
    {
        Painter::DrawChar(x + 3, GRID_TOP + 2, SYMBOL_TRIG_LEV_ABOVE);
        Painter::SetPoint(x + 5, GRID_TOP + 2);
        y = GRID_TOP + 7;
        x--;
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }
    Painter::SetFont(TypeFont_5);

    const char simbols[3] = {'1', '2', '?'};
    int dY = 0;
    
    Painter::DrawChar(x + 5, y - 9 + dY, simbols[TRIGSOURCE], Color::BACK);
    Painter::SetFont(TypeFont_8);

    if (gBF.drawRShiftMarkers == 1 && !Menu::IsMinimize())
    {
        DrawScaleLine(SCREEN_WIDTH - 11, true);
        int left = Grid::Right() + 9;
        int height = Grid::ChannelHeight() - 2 * DELTA;
        int shiftFullMin = RShiftMin + TrigLevMin;
        int shiftFullMax = RShiftMax + TrigLevMax;
        scale = (float)height / (shiftFullMax - shiftFullMin);
        int shiftFull = SET_TRIGLEV_SOURCE + (TRIGSOURCE_IS_EXT ? 0 : SET_RSHIFT(chan));
        int yFull = (int)(GRID_TOP + DELTA + height - scale * (shiftFull - RShiftMin - TrigLevMin) - 4);
        Painter::FillRegion(left + 2, yFull + 1, 4, 6, Color::Trig());
        Painter::SetFont(TypeFont_5);
        Painter::DrawChar(left + 3, yFull - 5 + dY, simbols[TRIGSOURCE], Color::BACK);
        Painter::SetFont(TypeFont_8);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursorRShift(Channel chan)
{
    int x = Grid::Right() - Grid::Width() - Measures::GetDeltaGridLeft();

    if (chan == Math)
    {
        int rShift = SET_RSHIFT_MATH;
        float scale = (float)Grid::MathHeight() / 960;
        int y = (int)((Grid::MathTop() + Grid::MathBottom()) / 2 - scale * (rShift - RShiftZero));
        Painter::DrawChar(x - 9, y - 4, SYMBOL_RSHIFT_NORMAL, Color::FILL);
        Painter::DrawChar(x - 8, y - 5, 'm', Color::BACK);
        return;
    }
    if(!sChannel_Enabled(chan))
    {
        return;
    }

    int rShift = SET_RSHIFT(chan);
 
    float scale = (float)Grid::ChannelHeight() / (STEP_RSHIFT * 200);
    int y = (int)(Grid::ChannelCenterHeight() - scale * (rShift - RShiftZero));

    float scaleFull = (float)Grid::ChannelHeight() / (RShiftMax - RShiftMin) * (sService_MathEnabled() ? 0.9f : 0.91f);
    int yFull = (int)(Grid::ChannelCenterHeight() - scaleFull *(rShift - RShiftZero));

    if(y > Grid::ChannelBottom())
    {
        Painter::DrawChar(x - 7, Grid::ChannelBottom() - 11, SYMBOL_RSHIFT_LOWER, Color::CHAN[chan]);
        Painter::SetPoint(x - 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x++;
    }
    else if(y < GRID_TOP)
    {
        Painter::DrawChar(x - 7, GRID_TOP + 2, SYMBOL_RSHIFT_ABOVE, Color::CHAN[chan]);
        Painter::SetPoint(x - 5, GRID_TOP + 2);
        y = GRID_TOP + 7;
        x++;
    }
    else
    {
        Painter::DrawChar(x - 8, y - 4, SYMBOL_RSHIFT_NORMAL, Color::CHAN[chan]);
        if(((chan == A) ? (gBF.showLevelRShift0 == 1) : (gBF.showLevelRShift1 == 1)) && MODE_WORK_IS_DIR)
        {
            Painter::DrawDashedHLine(y, Grid::Left(), Grid::Right(), 7, 3, 0);
        }
    }

    Painter::SetFont(TypeFont_5);
    int dY = 0;

    if((!Menu::IsMinimize() || !MENU_IS_SHOWN) && gBF.drawRShiftMarkers == 1)
    {
        Painter::FillRegion(4, yFull - 3, 4, 6, Color::CHAN[chan]);
        Painter::DrawChar(5, yFull - 9 + dY, chan == A ? '1' : '2', Color::BACK);
    }
    Painter::DrawChar((int)(x - 7), (int)(y - 9 + dY), chan == A ? '1' : '2', Color::BACK);
    Painter::SetFont(TypeFont_8);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursorTShift()
{
    int firstPoint = 0;
    int lastPoint = 0;
    sDisplay_PointsOnDisplay(&firstPoint, &lastPoint);

    // ?????? TPos
    int shiftTPos = sTime_TPosInPoints((PeakDetMode)gDSet->peakDet, gDSet->length1channel, TPOS) - SHIFT_IN_MEMORY;
    float scale = (lastPoint - firstPoint) / (float)Grid::Width();
    int gridLeft = Grid::Left();
    int x = (int)(gridLeft + shiftTPos * scale - 3);
    if (IntInRange(x + 3, gridLeft, Grid::Right() + 1))
    {
        Painter::Draw2SymbolsC(x, GRID_TOP - 1, SYMBOL_TPOS_2, SYMBOL_TPOS_3, Color::BACK, Color::FILL);
    };

    // ?????? tShift
    int shiftTShift = sTime_TPosInPoints((PeakDetMode)gDSet->peakDet, gDSet->length1channel, TPOS) - sTime_TShiftInPoints((PeakDetMode)gDSet->peakDet);
    if(IntInRange(shiftTShift, firstPoint, lastPoint))
    {
        x = gridLeft + shiftTShift - firstPoint - 3;
        Painter::Draw2SymbolsC(x, GRID_TOP - 1, SYMBOL_TSHIFT_NORM_1, SYMBOL_TSHIFT_NORM_2, Color::BACK, Color::FILL);
    }
    else if(shiftTShift < firstPoint)
    {
        Painter::Draw2SymbolsC(gridLeft + 1, GRID_TOP, SYMBOL_TSHIFT_LEFT_1, SYMBOL_TSHIFT_LEFT_2, Color::BACK, Color::FILL);
        Painter::DrawLine(Grid::Left() + 9, GRID_TOP + 1, Grid::Left() + 9, GRID_TOP + 7, Color::BACK);
    }
    else if(shiftTShift > lastPoint)
    {
        Painter::Draw2SymbolsC(Grid::Right() - 8, GRID_TOP, SYMBOL_TSHIFT_RIGHT_1, SYMBOL_TSHIFT_RIGHT_2, Color::BACK, Color::FILL);
        Painter::DrawLine(Grid::Right() - 9, GRID_TOP + 1, Grid::Right() - 9, GRID_TOP + 7, Color::BACK);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawHorizontalCursor(int y, int xTearing)
{
    y += GRID_TOP;
    if(xTearing == -1)
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, Grid::Right() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedHLine(y, Grid::Left() + 2, xTearing - 2, 1, 1, 0);
        Painter::DrawDashedHLine(y, xTearing + 2, Grid::Right() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(Grid::Left() - 1, y - 1, 2, 2);
    Painter::DrawRectangle(Grid::Right() - 1, y - 1, 2, 2);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawVerticalCursor(int x, int yTearing)
{
    x += Grid::Left();
    if(yTearing == -1)
    {
        Painter::DrawDashedVLine(x, GRID_TOP + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    else
    {
        Painter::DrawDashedVLine(x, GRID_TOP + 2, yTearing - 2, 1, 1, 0);
        Painter::DrawDashedVLine(x, yTearing + 2, Grid::ChannelBottom() - 1, 1, 1, 0);
    }
    Painter::DrawRectangle(x - 1, GRID_TOP - 1, 2, 2);
    Painter::DrawRectangle(x - 1, Grid::ChannelBottom() - 1, 2, 2);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawCursors()
{
    Channel source = CURS_SOURCE;
    Painter::SetColor(Color::Cursors(source));
    if (sCursors_NecessaryDrawCursors())
    {
        bool bothCursors = !CURS_CNTRL_T_IS_DISABLE(source) && !CURS_CNTRL_U_IS_DISABLE(source);  // ??????? ????, ??? ???????? ? ???????????? ? 
                                                                            // ?????????????? ??????? - ???? ?????????? ???????? ? ?????? ???????????
        int x0 = -1;
        int x1 = -1;
        int y0 = -1;
        int y1 = -1;

        if (bothCursors)
        {
            x0 = (int)(Grid::Left() + CURS_POS_T0(source));
            x1 = (int)(Grid::Left() + CURS_POS_T1(source));
            y0 = (int)(GRID_TOP + sCursors_GetCursPosU(source, 0));
            y1 = (int)(GRID_TOP + sCursors_GetCursPosU(source, 1));

            Painter::DrawRectangle(x0 - 2, y0 - 2, 4, 4);
            Painter::DrawRectangle(x1 - 2, y1 - 2, 4, 4);
        }

        CursCntrl cntrl = CURS_CNTRL_T(source);
        if (cntrl != CursCntrl_Disable)
        {
            DrawVerticalCursor((int)CURS_POS_T0(source), y0);
            DrawVerticalCursor((int)CURS_POS_T1(source), y1);
        }
        cntrl = CURsU_CNTRL;
        if (cntrl != CursCntrl_Disable)
        {
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 0), x0);
            DrawHorizontalCursor((int)sCursors_GetCursPosU(source, 1), x1);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawMeasures()
{
    if(!SHOW_MEASURES)
    {
        gBF.topMeasures = GRID_BOTTOM;
        return;
    }

    Processing::CalculateMeasures();

    if(MEAS_FIELD_IS_HAND)
    {
        int x0 = MEAS_POS_CUR_T0 - SHIFT_IN_MEMORY + Grid::Left();
        int y0 = MEAS_POS_CUR_U0 + GRID_TOP;
        int x1 = MEAS_POS_CUR_T1 - SHIFT_IN_MEMORY + Grid::Left();
        int y1 = MEAS_POS_CUR_U1 + GRID_TOP;
        Sort(&x0, &x1);
        Sort(&y0, &y1);
        Painter::DrawRectangle(x0, y0, x1 - x0, y1 - y0, Color::FILL);
    }

    int x0 = Grid::Left() - Measures::GetDeltaGridLeft();
    int dX = Measures::GetDX();
    int dY = Measures::GetDY();
    int y0 = Measures::GetTopTable();

    int numRows = Measures::NumRows();
    int numCols = Measures::NumCols();

    for(int str = 0; str < numRows; str++)
    {
        for(int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = Measures::IsActive(str, elem) && Menu::GetNameOpenedPage() == PageSB_Measures_Tune;
            Color color = active ? Color::BACK : Color::FILL;
            Meas meas = Measures::Type(str, elem);
            if(meas != Meas_None)
            {
                Painter::FillRegion(x, y, dX, dY, Color::BACK);
                Painter::DrawRectangle(x, y, dX, dY, Color::FILL);
                gBF.topMeasures = Math::MinFrom2Int(gBF.topMeasures, y);
            }
            if(active)
            {
                Painter::FillRegion(x + 2, y + 2, dX - 4, dY - 4, Color::FILL);
            }
            if(meas != Meas_None)
            {
                char buffer[20];
                Painter::DrawText(x + 4, y + 2, Measures::Name(str, elem), color);
                if(meas == MEAS_MARKED)
                {
                    Painter::FillRegion(x + 1, y + 1, dX - 2, 9, active ? Color::BACK : Color::FILL);
                    Painter::DrawText(x + 4, y + 2, Measures::Name(str, elem), active ? Color::FILL : Color::BACK);
                }
                if(SOURCE_MEASURE_IS_A)
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(meas, A, buffer), Color::CHAN[A]);
                }
                else if(SOURCE_MEASURE_IS_B)
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(meas, B, buffer), Color::CHAN[B]);
                }
                else
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(meas, A, buffer), Color::CHAN[A]);
                    Painter::DrawText(x + 2, y + 20, Processing::GetStringMeasure(meas, B, buffer), Color::CHAN[B]);
                }
            }
        }
    }

    if(Menu::GetNameOpenedPage() == PageSB_Measures_Tune)
    {
        Measures::DrawPageChoice();
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteTextVoltage(Channel chan, int x, int y)
{
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x90"
    };
    Color color = Color::CHAN[chan];

    bool inverse = SET_INVERSE(chan);
    ModeCouple modeCouple = SET_COUPLE(chan);
    Divider multiplier = SET_DIVIDER(chan);
    Range range = SET_RANGE(chan);
    uint rShift = (uint)SET_RSHIFT(chan);
    bool enable = SET_ENABLED(chan);

    if (!MODE_WORK_IS_DIR)
    {
        DataSettings *ds = MODE_WORK_IS_DIR ? gDSet : gDSmemInt;
        if (ds != 0)
        {
            inverse = (chan == A) ? ds->inverseCh0 : ds->inverseCh1;
            modeCouple = (ModeCouple)((chan == A) ? ds->modeCouple0 : ds->modeCouple1);
            multiplier = (Divider)((chan == A) ? ds->multiplier0 : ds->multiplier1);
            range = ds->range[chan];
            rShift = (chan == A) ? ds->rShiftCh0 : ds->rShiftCh1;
            enable = (chan == A) ? ds->enableCh0 : ds->enableCh1;
        }
    }

    if(enable)
    {
        const int widthField = 91;
        const int heightField = 8;

        Color colorDraw = inverse ? Color::WHITE : color;
        if(inverse)
        {
            Painter::FillRegion(x, y, widthField, heightField, color);
        }

        char buffer[100] = {0};

        sprintf(buffer, "%s\xa5%s\xa5%s", (chan == A) ? (set.common.lang == Russian ? "1?" : "1c") : (set.common.lang == Russian ? "2?" : "2c"), couple[modeCouple], 
            sChannel_Range2String(range, multiplier));

        Painter::DrawText(x + 1, y, buffer, colorDraw);

        char bufferTemp[20];
        sprintf(buffer, "\xa5%s", sChannel_RShift2String((int16)rShift, range, multiplier, bufferTemp));
        Painter::DrawText(x + 46, y, buffer);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::WriteStringAndNumber(char *text, int16 x, int16 y, int number)
{
    char buffer[100];
    Painter::DrawText(x, y, text, Color::FILL);
    if(number == 0)
    {
        sprintf(buffer, "-");
    }
    else
    {
        sprintf(buffer, "%d", number);
    }
    Painter::DrawTextRelativelyRight(x + 41, y, buffer);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawLowPart()
{
    int y0 = SCREEN_HEIGHT - 19;
    int y1 = SCREEN_HEIGHT - 10;
    int x = -1;

    Painter::DrawHLine(Grid::ChannelBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2, Color::FILL);
    Painter::DrawHLine(Grid::FullBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2);

    WriteTextVoltage(A, x + 2, y0);

    WriteTextVoltage(B, x + 2, y1);

    Painter::DrawVLine(x + 95, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2, Color::FILL);

    x += 98;
    char buffer[100] = {0};

    TBase tBase = SET_TBASE;
    int16 tShift = SET_TSHIFT;

    if (!MODE_WORK_IS_DIR)
    {
        DataSettings *ds = MODE_WORK_IS_RAM ? gDSmemLast : gDSmemInt;
        if (ds != 0)
        {
            tBase = (TBase)ds->tBase;
            tShift = ds->tShift;
        }
    }

    sprintf(buffer, "?\xa5%s", Tables_GetTBaseString(tBase));
    Painter::DrawText(x, y0, buffer);

    buffer[0] = 0;
    char bufForVal[20];
    sprintf(buffer, "\xa5%s", FPGA::GetTShiftString(tShift, bufForVal));
    Painter::DrawText(x + 35, y0, buffer);

    buffer[0] = 0;
    const char *source[3] = {"1", "2", "\x82"};
    if (MODE_WORK_IS_DIR)
    {
        sprintf(buffer, "?\xa5\x10%s", source[TRIGSOURCE]);
    }

    Painter::DrawText(x, y1, buffer, Color::Trig());

    buffer[0] = 0;
    static const char *couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static const char *polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static const char *filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if (MODE_WORK_IS_DIR)
    {
        sprintf(buffer, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        Painter::DrawText(x + 18, y1, buffer);
        Painter::DrawChar(x + 45, y1, filtr[TRIG_INPUT][0]);
        Painter::DrawChar(x + 53, y1, filtr[TRIG_INPUT][1]);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (MODE_WORK_IS_DIR)
    {
        sprintf(buffer, "\xa5\x10%c", mode[START_MODE]);
        Painter::DrawText(x + 63, y1, buffer);
    }
    
    Painter::DrawVLine(x + 79, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2, Color::FILL);

    Painter::DrawHLine(GRID_BOTTOM, GRID_RIGHT + 2, SCREEN_WIDTH - 2);
    Painter::DrawHLine(Grid::ChannelBottom(), GRID_RIGHT + 2, SCREEN_WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    int y2 = y1 + 6;
    Painter::SetFont(TypeFont_5);
    
    if (MODE_WORK_IS_DIR)
    {
        WriteStringAndNumber("??????", (int16)x, (int16)y0, NUM_ACCUM);
        WriteStringAndNumber("??????", (int16)x, (int16)y1, NUM_AVE);
        WriteStringAndNumber("??\x93???", (int16)x, (int16)y2, NUM_MIN_MAX);
    }

    x += 42;
    Painter::DrawVLine(x, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2);

    Painter::SetFont(TypeFont_8);

    if (MODE_WORK_IS_DIR)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FPGA::GetFreq();
        if (freq == -1.0f) //-V550
        {
            strcat(mesFreq, "******");
        }
        else
        {
            strcat(mesFreq, Freq2String(freq, false, buffer));
        }
        Painter::DrawText(x + 3, GRID_BOTTOM + 2, mesFreq);
    }

    DrawTime(x + 3, GRID_BOTTOM + 11);

    Painter::DrawVLine(x + 55, GRID_BOTTOM + 2, SCREEN_HEIGHT - 2);

    Painter::SetFont(TypeFont_UGO2);

    // ??????
    if (gBF.flashDriveIsConnected == 1)
    {
        Painter::Draw4SymbolsInRect(x + 57, GRID_BOTTOM + 2, SYMBOL_FLASH_DRIVE);
    }

    // Ethernet
    if ((gBF.ethIsConnected == 1 || gBF.cableEthIsConnected == 1) && TIME_MS > 2000)
    {
        Painter::Draw4SymbolsInRect(x + 87, GRID_BOTTOM + 2, SYMBOL_ETHERNET, gBF.ethIsConnected ? Color::FILL : Color::FLASH_01);
    }

    if (gBF.connectToHost == 1 || gBF.cableVCPisConnected == 1)
    {
        Painter::Draw4SymbolsInRect(x + 72, GRID_BOTTOM + 2, SYMBOL_USB, gBF.connectToHost ? Color::FILL : Color::FLASH_01);
    }
    
    Painter::SetColor(Color::FILL);
    // ??????? ????????
    if(!SET_PEAKDET_IS_DISABLED)
    {
       Painter::DrawChar(x + 38, GRID_BOTTOM + 11, '\x12');
       Painter::DrawChar(x + 46, GRID_BOTTOM + 11, '\x13');
    }

    if (MODE_WORK_IS_DIR)
    {
        Painter::SetFont(TypeFont_5);
        WriteStringAndNumber("?????.:", (int16)(x + 57), GRID_BOTTOM + 10, sDisplay_NumPointSmoothing());
        Painter::SetFont(TypeFont_8);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawTimeForFrame(uint timeTicks)
{
    if(!SHOW_STATS)
    {
        return;
    }
    static char buffer[10];
    static bool first = true;
    static uint timeMSstartCalculation = 0;
    static int numFrames = 0;
    static float numMS = 0.0f;
    if(first)
    {
        timeMSstartCalculation = TIME_MS;
        first = false;
    }
    numMS += timeTicks / 120000.0f;
    numFrames++;
    
    if((TIME_MS - timeMSstartCalculation) >= 500)
    {
        sprintf(buffer, "%.1fms/%d", (double)(numMS / numFrames), numFrames * 2);
        timeMSstartCalculation = TIME_MS;
        numMS = 0.0f;
        numFrames = 0;
    }

    Painter::DrawRectangle(Grid::Left(), Grid::FullBottom() - 10, 84, 10, Color::FILL);
    Painter::FillRegion(Grid::Left() + 1, Grid::FullBottom() - 9, 82, 8, Color::BACK);
    Painter::DrawText(Grid::Left() + 2, Grid::FullBottom() - 9, buffer, Color::FILL);

    char message[20] = {0};
    sprintf(message, "%d", Storage::NumElementsWithSameSettings());
    strcat(message, "/");
    char numAvail[10] = {0};
    sprintf(numAvail, "%d", Storage::NumberAvailableEntries());
    strcat(message, numAvail);
    Painter::DrawText(Grid::Left() + 50, Grid::FullBottom() - 9, message);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DisableShowLevelRShiftA()
{
    gBF.showLevelRShift0 = 0;
    Timer::Disable(kShowLevelRShiftA);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DisableShowLevelRShiftB()
{
    gBF.showLevelRShift1 = 0;
    Timer::Disable(kShowLevelRShiftB);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DisableShowLevelTrigLev()
{
    gBF.showLevelTrigLev = 0;
    Timer::Disable(kShowLevelTrigLev);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EnableTrigLabel(bool enable)
{
    gBF.trigEnable = enable ? 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ResetP2Ppoints(bool empty)
{
    dataP2PIsEmpty = empty;
    lastP2Pdata = 0;
    memset(dataP2P_0, AVE_VALUE, NUM_P2P_POINTS);
    memset(dataP2P_1, AVE_VALUE, NUM_P2P_POINTS);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddPoints(uint8 data00, uint8 data01, uint8 data10, uint8 data11)
{
    dataP2PIsEmpty = false;
    if (SELFRECORDER)
    {
        if (lastP2Pdata == NUM_P2P_POINTS)
        {
            memcpy(dataP2P_0, dataP2P_0 + 2, NUM_P2P_POINTS - 2);
            memcpy(dataP2P_1, dataP2P_1 + 2, NUM_P2P_POINTS - 2);
        }
    }

    dataP2P_0[lastP2Pdata] = data00;
    dataP2P_1[lastP2Pdata++] = data10;
    
    dataP2P_0[lastP2Pdata] = data01;
    dataP2P_1[lastP2Pdata++] = data11;
    if (!SELFRECORDER && lastP2Pdata >= NUM_P2P_POINTS)
    {
        lastP2Pdata = 0;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetDrawMode(DrawMode mode, pFuncVV func)
{
    funcOnHand = mode == DrawMode_Auto ? 0 : func;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetAddDrawFunction(pFuncVV func)
{
    funcAdditionDraw = func;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Clear()
{
    Painter::FillRegion(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 2, Color::BACK);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShiftScreen(int delta)
{
    SHIFT_IN_MEMORY += (int16)delta;
    Limitation<int16>(&SHIFT_IN_MEMORY, 0, (int16)(sMemory_GetNumPoints(false) - 282));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ChangedRShiftMarkers()
{
    gBF.drawRShiftMarkers = ALT_MARKERS_HIDE ? 0 : 1;
    Timer::SetAndEnable(kRShiftMarkersAutoHide, FuncOnTimerRShiftMarkersAutoHide, 5000);
}

static void FuncOnTimerRShiftMarkersAutoHide()
{
    Display::OnRShiftMarkersAutoHide();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::OnRShiftMarkersAutoHide()
{
    gBF.drawRShiftMarkers = 0;
    Timer::Disable(kRShiftMarkersAutoHide);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
int Display::FirstEmptyString()
{
    for(int i = 0; i < MAX_NUM_STRINGS; i++)
    {
        if(strings[i] == 0)
        {
            return i;
        }
    }
    return MAX_NUM_STRINGS;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
int Display::CalculateFreeSize()
{
    int firstEmptyString = FirstEmptyString();
    if(firstEmptyString == 0)
    {
        return SIZE_BUFFER_FOR_STRINGS;
    }
    return SIZE_BUFFER_FOR_STRINGS - (strings[firstEmptyString - 1] - bufferForStrings) - (int)strlen(strings[firstEmptyString - 1]) - 1;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DeleteFirstString()
{
    if(FirstEmptyString() < 2)
    {
        return;
    }
    int delta = (int)strlen(strings[0]) + 1;
    int numStrings = FirstEmptyString();
    for(int i = 1; i < numStrings; i++)
    {
        strings[i - 1] = strings[i] - delta;
    }
    for(int i = numStrings - 1; i < MAX_NUM_STRINGS; i++)
    {
        strings[i] = 0;
    }
    for(int i = 0; i < SIZE_BUFFER_FOR_STRINGS - delta; i++)
    {
        bufferForStrings[i] = bufferForStrings[i + delta];
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddString(const char *string)
{
    if(CONSOLE_IN_PAUSE)
    {
        return;
    }
    static int num = 0;
    char buffer[100];
    sprintf(buffer, "%d\x11", num++);
    strcat(buffer, string);
    int size = (int)(strlen(buffer) + 1);
    while(CalculateFreeSize() < size)
    {
        DeleteFirstString();
    }
    if(!strings[0])
    {
        strings[0] = bufferForStrings;
        strcpy(strings[0], buffer);
    }
    else
    {
        char *addressLastString = strings[FirstEmptyString() - 1];
        char *address = addressLastString + strlen(addressLastString) + 1;
        strings[FirstEmptyString()] = address;
        strcpy(address, buffer);
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::AddStringToIndicating(const char *string)
{
    if(FirstEmptyString() == MAX_NUM_STRINGS)
    {
        DeleteFirstString();
    }

    AddString(string);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetPauseForConsole(bool pause)
{
    if(pause)
    {
        lastStringForPause = FirstEmptyString() - 1;
    }
    else
    {
        lastStringForPause = -1;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::OneStringUp()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause > NUM_STRINGS - 1)
    {
        lastStringForPause--;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::OneStringDown()
{
    if(!CONSOLE_IN_PAUSE)
    {
    }
    else if(lastStringForPause < FirstEmptyString() - 1)
    {
        lastStringForPause++;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawConsole()
{
    int count = 0;
    Painter::SetFont(sDebug_GetSizeFontForConsole() == 5 ? TypeFont_5 : TypeFont_8);
    int height = Font::GetSize();

    int lastString = FirstEmptyString() - 1;
    int numString = NUM_STRINGS;
    if(height == 8 && numString > 22)
    {
        numString = 22;
    }
    int delta = 0;
    if(SHOW_STRING_NAVIGATION)
    {
        numString -= ((height == 8) ? 1 : 2);
        delta = 10;
    }
    int firstString = lastString - numString + 1;
    if(firstString < 0)
    {
        firstString = 0;
    }

    int dY = 0;
    
    for(numString = firstString; numString <= lastString; numString++)
    {
        int width = Font::GetLengthText(strings[numString]);
        Painter::FillRegion(Grid::Left() + 1, GRID_TOP + 1 + count * (height + 1) + delta, width, height + 1, Color::BACK);
        int y = GRID_TOP + 5 + count * (height + 1) - 4;
        if(Font::GetSize() == 5)
        {
            y -= 3;
        }
        Painter::DrawText(Grid::Left() + 2, y + dY + delta, strings[numString], Color::FILL);
        count++;
    }

    Painter::SetFont(TypeFont_8);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarn(const char *message)
{
    if (warnings[0] == 0)
    {
        Timer::SetAndEnable(kShowMessages, OnTimerShowWarning, 100);
    }
    bool alreadyStored = false;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIME_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIME_MS;
            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerShowWarning()
{
    uint time = TIME_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if ((int)(time - timeWarnings[i]) > TIME_MESSAGES * 1000)
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(kShowMessages);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ClearFromWarnings()
{
    Timer::Disable(kShowMessages);
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        warnings[i] = 0;
        timeWarnings[i] = 0;
    }
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarningBad(Warning warning)
{
    Painter::ResetFlash();
    ShowWarn(Tables_GetWarning(warning));
    Sound::WarnBeepBad();
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarningGood(Warning warning)
{
    Painter::ResetFlash();
    ShowWarn(Tables_GetWarning(warning));
    Sound::WarnBeepGood();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawStringInRectangle(int, int y, char const *text)
{
    int width = Font::GetLengthText(text);
    int height = 8;
    Painter::DrawRectangle(Grid::Left(), y, width + 4, height + 4, Color::FILL);
    Painter::DrawRectangle(Grid::Left() + 1, y + 1, width + 2, height + 2, Color::BACK);
    Painter::FillRegion(Grid::Left() + 2, y + 2, width, height, Color::FLASH_10);
    Painter::DrawText(Grid::Left() + 3, y + 2, text, Color::FLASH_01);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawWarnings()
{
    int delta = 12;
    int y = Grid::BottomMessages();
    for(int i = 0; i < 10; i++)
    {
        if(warnings[i] != 0)
        {
            DrawStringInRectangle(Grid::Left(), y, warnings[i]);
            y -= delta;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RunAfterDraw(pFuncVV func)
{
    funcAfterDraw = func;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarning(Warning)
{
    LOG_ERROR("????????");
}
