#include "Log.h"
#include "Ethernet/Ethernet.h"
#include "Ethernet/TcpSocket.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "FlashDrive/FileManager.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool inverseColors = false;
static Color currentColor = Color::NUMBER;
static bool framesElapsed = false;


#define TRANSMIT_NEED_FOR_FIRST     (stateTransmit == StateTransmit_NeedForTransmitFirst)
#define TRANSMIT_NEED_FOR_SECOND    (stateTransmit == StateTransmit_NeedForTransmitSecond)
#define TRANSMIT_IS_FREE            (stateTransmit == StateTransmit_Free)
#define TRANSMIT_IN_PROCESS         (stateTransmit == StateTransmit_InProcess)


static enum StateTransmit
{
    StateTransmit_Free,
    StateTransmit_NeedForTransmitFirst,  ///< ��� ����� ����� �������� ������ ���� - ���������� ������.
    StateTransmit_NeedForTransmitSecond, ///< ��� ����� ����� �������� ������ � ����������� ����� - ������ �� ����������.
    StateTransmit_InProcess
} stateTransmit = StateTransmit_Free;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnTimerFlashDisplay();
static void CalculateColor(uint8 *color);
static uint8 Read2points(int x, int y);

// ��� ������� �� ������������, �� ��������� ��� �������
//static void Get4Bytes(uint8 bytes[4]);
//static Color GetColor(int x, int y);
//static void Get8Points(int x, int y, uint8 buffer[4]);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color color)
{
    if (TRANSMIT_NEED_FOR_FIRST || TRANSMIT_NEED_FOR_SECOND)
    {
        bool needForLoadFonts = TRANSMIT_NEED_FOR_FIRST;
        stateTransmit = StateTransmit_InProcess;
        if (needForLoadFonts)
        {
            LoadPalette();
            LoadFont(TypeFont_5);
#define dT 100
            Ethernet::Update(dT);            /// \todo ��������. ���������� ����� ������� � TCPSocket

            LoadFont(TypeFont_8);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO);
            Ethernet::Update(dT);

            LoadFont(TypeFont_UGO2);
            Ethernet::Update(dT);
        }
    }

    FillRegion(0, 0, 319, 239, color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    if (!framesElapsed)
    {
        framesElapsed = true;
        return;
    }
    uint8 command[4] = {INVALIDATE};
    SendToDisplay(command, 4);
    SendToInterfaces(command, 1);
    if (TRANSMIT_IN_PROCESS)
    {
        VCP::Flush();
        stateTransmit = StateTransmit_Free;
    }

    RunDisplay();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendFrame(bool first)
{
    if (TRANSMIT_IS_FREE)
    {
        stateTransmit = (first ? StateTransmit_NeedForTransmitFirst : StateTransmit_NeedForTransmitSecond);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::ResetFlash()
{
    Timer::SetAndEnable(kFlashDisplay, OnTimerFlashDisplay, 400);
    inverseColors = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    if (color != currentColor && color != Color::NUMBER)
    {
        currentColor = color;
        if (currentColor > Color::NUMBER)
        {
            CalculateColor((uint8 *)(&(color)));
        }
        uint8 command[4] = {SET_COLOR, color.value};
        SendToDisplay(command, 4);
        SendToInterfaces(command, 2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Color Painter::GetColor()
{
    return currentColor;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::LoadPalette()
{
    for (uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        SetPalette((Color)i);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPalette(Color color)
{
    uint8 command[4] = {SET_PALETTE_COLOR};
    WRITE_BYTE(1, color.value);
    WRITE_SHORT(2, COLOR(color.value));

    SendToDisplay(command, 4);
    SendToInterfaces(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    uint8 command[4] = {DRAW_PIXEL};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);

    SendToDisplay(command, 4);
    SendToInterfaces(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }

    CalculateCurrentColor();

    uint8 command[8] = {DRAW_HLINE};
    WRITE_BYTE(1, y);
    WRITE_SHORT(2, x0);
    WRITE_SHORT(4, x1);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }
    CalculateCurrentColor();

    uint8 command[8] = {DRAW_VLINE};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y0);
    WRITE_BYTE(4, y1);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 5);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += (int)delta)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVPointLine(int x, int y0, int y1, float delta)
{
    for (int y = y0; y <= y1; y += (int)delta)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiVPointLine(int numLines, int y, uint16 x[], int delta, int count, Color color)
{
    ASSERT_RET(numLines > 20, "����� ����� ������� ������� %d", numLines);

    SetColor(color);

    uint8 command[60] = {DRAW_MULTI_VPOINT_LINE, (uint8)numLines, (uint8)y, (uint8)count, (uint8)delta, 0};

    uint8 *pointer = command + 6;
    for (int i = 0; i < numLines; i++)
    {
        *((uint16 *)pointer) = x[i];
        pointer += 2;
    }
    int numBytes = 1 + 1 + 1 + numLines * 2 + 1 + 1;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 1 + 1 + 1 + 1 + 1 + numLines * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiHPointLine(int numLines, int x, uint8 y[], int delta, int count, Color color)
{
    if (numLines > 20)
    {
        LOG_ERROR_TRACE("����� ����� ������� ������� %d", numLines);
        return;
    }
    SetColor(color);

    uint8 command[30] = {DRAW_MULTI_HPOINT_LINE};
    WRITE_BYTE(1, numLines);
    WRITE_SHORT(2, x);
    WRITE_BYTE(4, count);
    WRITE_BYTE(5, delta);

    uint8 *pointer = command + 6;
    for (int i = 0; i < numLines; i++)
    {
        *pointer = y[i];
        pointer++;
    }
    int numBytes = 1 +  // command
        1 +             // numLines
        2 +             // x
        numLines +      // numLines
        1 +
        1;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 1 + 2 + 1 + 1 + numLines);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x1, int y1, int x2, int y2, Color color)
{
    SetColor(color);
    /// \todo ������� ���, ����� ���� ����� ���������� � �������� �������.
    if (x1 == x2)
    {
        DrawVLine(x1, y1, y2);
    }
    else if (y1 == y2)
    {
        DrawHLine(y1, x1, x2);
    }
    else
    {
#ifdef S8_54
        uint8 command[8] = {DRAW_LINE};
        WRITE_SHORT(1, x1);
        WRITE_BYTE(3, y1);
        WRITE_SHORT(4, x2);
        WRITE_BYTE(6, y2);

        SendToDisplay(command, 8);
        SendToInterfaces(command, 7);
#endif
#ifdef S8_53
        if ((x2 - x1) == 0 && (y2 - y1) == 0)
        {
            ++x1;
        }
        int x = x1;
        int y = y1;
        int dx = Abs(x2 - x1);
        int dy = Abs(y2 - y1);
        int s1 = Sign(x2 - x1);
        int s2 = Sign(y2 - y1);
        int temp;
        int exchange = 0;
        if (dy > dx)
        {
            temp = dx;
            dx = dy;
            dy = temp;
            exchange = 1;
        }
        int e = 2 * dy - dx;
        int i = 0;
        for (; i <= dx; i++)
        {
            SetPoint(x, y);
            while (e >= 0)
            {
                if (exchange)
                {
                    x += s1;
                }
                else
                {
                    y += s2;
                }
                e = e - 2 * dx;
            }
            if (exchange)
            {
                y += s2;
            }
            else
            {
                x += s1;
            }
            e = e + 2 * dy;
        }
#endif
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR_TRACE("������������ �������� deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // ���� ����� ����� �������� �� � ������ ������
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            DrawHLine(y, x0, x - 1);
        }
    }

    while (x < x1)
    {
        DrawHLine(y, x, x + deltaFill - 1);
        x += (deltaFill + deltaEmpty);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart)
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR_TRACE("������������ �������� deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // ���� ����� ����� �������� �� � ������ ������
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // ���� ������ ����� ���������� �� �����
        {
            DrawVLine(x, y0, y - 1);
        }
    }

    while (y < y1)
    {
        DrawVLine(x, y, y + deltaFill - 1);
        y += (deltaFill + deltaEmtpy);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    DrawHLine(y, x, x + width);
    DrawVLine(x, y, y + height);
    DrawHLine(y + height, x, x + width);
    if (x + width < SCREEN_WIDTH)
    {
        DrawVLine(x + width, y, y + height);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color)
{
    if (color != Color::NUMBER)
    {
        SetColor(color);
    }

    CalculateCurrentColor();

    uint8 command[8] = {FILL_REGION};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);
    WRITE_SHORT(4, width);
    WRITE_BYTE(6, height);

    SendToDisplay(command, 8);
    SendToInterfaces(command, 7);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright, Color dark, bool isPressed, bool isShade)
{
    FillRegion(x + thickness, y + thickness, width - thickness * 2, height - thickness * 2, normal);
    if (isPressed && !isShade)
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLine(y + i, x + i, x + width - i, dark);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLine(x + width - i, y + 1 + i, y + height - i, bright);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            DrawHLine(y + i, x + i, x + width - i, bright);
            DrawVLine(x + i, y + 1 + i, y + height - i);
            DrawVLine(x + width - i, y + 1 + i, y + height - i, dark);
            DrawHLine(y + height - i, x + 1 + i, x + width - i);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetBrightnessDisplay(int16 brightness)
{
    float recValue = 1601.0f;
    if (brightness < 100)
    {
        recValue = 64.0f + (600.0f - 63.0f) / 100.0f / 100.0f * brightness * brightness;
    }
    uint8 command[4] = {SET_BRIGHTNESS};
    WRITE_SHORT(1, recValue);

    SendToDisplay(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Painter::ReduceBrightness(uint16 colorValue, float newBrightness)
{
    int red = (int)(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, 0, 31);
    int green = (int)(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, 0, 63);
    int blue = (int)(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, 0, 31);
    return MAKE_COLOR(red, green, blue);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color)
{
    SetColor(color);

    uint8 command[255 * 2 + 4 + 4] = {DRAW_VLINES_ARRAY};
    WRITE_SHORT(1, x);

    if (numLines > 255)
    {
        numLines = 255;
    }

    WRITE_BYTE(3, numLines);

    for (int i = 0; i < numLines; i++)
    {
        WRITE_BYTE(4 + i * 2,     *(y0y1 + i * 2));
        WRITE_BYTE(4 + i * 2 + 1, *(y0y1 + i * 2 + 1));
    }
    int numBytes = numLines * 2 + 4;
    while (numBytes % 4)
    {
        numBytes++;
    }
    SendToDisplay(command, numBytes);
    SendToInterfaces(command, 1 + 2 + 1 + 2 * numLines);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawSignal(int x, uint8 data[281], bool modeLines)
{
    uint8 command[284] = {(uint8)(modeLines ? DRAW_SIGNAL_LINES : DRAW_SIGNAL_POINTS)};
    WRITE_SHORT(1, x);
    for (int i = 0; i < 281; i++)
    {
        WRITE_BYTE(3 + i, data[i]);
    }

    SendToDisplay(command, 284);
    SendToInterfaces(command, 284);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawPicture(int x, int y, int width, int height, uint8 *address)
{
    uint8 command[4] = {LOAD_IMAGE};
    WRITE_SHORT(1, x);
    WRITE_BYTE(3, y);

    SendToDisplay(command, 4);

    WRITE_SHORT(0, width);
    WRITE_BYTE(2, height);
    WRITE_BYTE(3, 0);

    SendToDisplay(command, 4);

    for (int i = 0; i < width * height / 2 / 4; i++)
    {
        WRITE_BYTE(0, *address++);
        WRITE_BYTE(1, *address++);
        WRITE_BYTE(2, *address++);
        WRITE_BYTE(3, *address++);

        SendToDisplay(command, 4);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Painter::SaveScreenToFlashDrive()
{

#pragma pack(1)
    typedef struct
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    } BITMAPFILEHEADER;
    // 14

    typedef struct
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    } BITMAPINFOHEADER;
    // 54
#pragma pack(4)

    BITMAPFILEHEADER bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240 / 2,
        0,
        0,
        14 + 40 + 1024
    };

    StructForWrite structForWrite;
    char fileName[255];

    if (!FileManager::GetNameForNewFile(fileName))
    {
        return false;
    }

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    BITMAPINFOHEADER bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        4,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = {0};

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 16; i++)
    {
        uint16 color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color) / 31.0f * 255.0f);
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color) / 63.0f * 255.0f);
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color) / 31.0f * 255.0f);
        colorStruct.rgbReserved = 0;
        ((RGBQUAD*)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    FSMC_SET_MODE(ModeFSMC_Display);

    for (int y = 239; y >= 0; y--)
    {
        for (int x = 1; x < 320; x += 2)
        {
            uint8 color = Read2points(x, y);

            while (color != Read2points(x, y))      // ���� ���� ����� ������, ��� ������ ����� ����������� ����������� - 
            {                                       // ��������, ��� ��������� ������ ������ ��� �� ����.
                color = Read2points(x, y);
            }

            buffer[x / 2] = (uint8)(((color & 0x0f) << 4) + (color >> 4));
        }

        FDrive::WriteToFile(buffer, 160, &structForWrite);
    }

    FSMC_RESTORE_MODE();

    FDrive::CloseFile(&structForWrite);

    return true;
}

#define EQU(x) (bytes[0] == x)

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
        {
        };
    
        Timer::PauseOnTicks(75);    /// \todo ����� ����� �������� ��������� �� ��������� � �8-53 (��� ������� 120���, ����� - 180���)
    
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendToInterfaces(uint8 *pointer, int size)
{
    if (TRANSMIT_IN_PROCESS)
    {
        VCP::SendDataSynch(pointer, size);
        SocketTCP::Send((const char *)pointer, size);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::RunDisplay()
{
    uint8 command[4] = {RUN_BUFFER};

    SendToDisplay(command, 4);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerFlashDisplay()
{
    inverseColors = !inverseColors;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateColor(uint8 *color)
{
    currentColor = (Color)*color;
    if (*color == Color::FLASH_10.value)
    {
        *color = inverseColors ? Color::BACK.value : Color::FILL.value;
    }
    else if (*color == Color::FLASH_01.value)
    {
        *color = inverseColors ? Color::FILL.value : Color::BACK.value;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::CalculateCurrentColor()
{
    if (currentColor == Color::FLASH_10)
    {
        SetColor(inverseColors ? Color::BACK : Color::FILL);
    }
    else if (currentColor == Color::FLASH_01)
    {
        SetColor(inverseColors ? Color::FILL : Color::BACK);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 Read2points(int x, int y)
{
    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
    {
    };
    Timer::PauseOnTicks(12);               /// \todo �������� ��������� ����� �������� - �� ������ ������

    *ADDR_CDISPLAY = GET_PIXEL;
    *ADDR_CDISPLAY = (uint8)x;
    *ADDR_CDISPLAY = (uint8)(x >> 8);
    *ADDR_CDISPLAY = (uint8)y;

    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
    {
    };
    /// \todo �������� ��������� ����� �������� - �� ������ ������
    //Timer_PauseOnTicks(6);

    return *ADDR_CDISPLAY;
}



//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Get4Bytes(uint8 bytes[4])
{
    while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET) {};
    bytes[0] = *ADDR_CDISPLAY;
    bytes[1] = *ADDR_CDISPLAY;
    bytes[2] = *ADDR_CDISPLAY;
    bytes[3] = *ADDR_CDISPLAY;
}
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static Color GetColor(int x, int y)
{
    uint8 command[4];
    command[0] = GET_PIXEL;
    *((int16 *)(command + 1)) = (int16)x;
    *(command + 3) = (int8)y;
    SendToDisplay(command, 4);
    Get4Bytes(command);
    return (Color)(command[0] & 0x0f);
}
*/


//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Get8Points(int x, int y, uint8 buffer[4])
{
    uint8 command[4];
    command[0] = GET_PIXEL;
    *((int16 *)(command + 1)) = (int16)x;
    *(command + 3) = (int8)y;
    SendToDisplay(command, 4);
    Get4Bytes(buffer);
}
*/
