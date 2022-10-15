#include "Decoder.h"
#include "Hardware/FSMC.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/Keyboard.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool(*pFuncBU8)(uint8);
/// ����������� �������
static pFuncBU8 curFunc;
int      Decoder::step = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Decoder::AddData(uint8 data)
{
    static const struct StructFunc
    {
        pFuncBU8 func;
        StructFunc(pFuncBU8 f) : func(f) {};
    }
    command[Command::Number] =
    {
        &Decoder::EmptyFunc,
        &Decoder::InButtonPress,
        &Decoder::BeginScene,
        &Decoder::EndScene,
        &Decoder::SetColor,
        &Decoder::FillRegion,
        &Decoder::DrawText,
        &Decoder::SetPalette,
        &Decoder::DrawRectangle,
        &Decoder::DrawVLine,
        &Decoder::DrawHLine,
        &Decoder::SetFont,
        &Decoder::SetPoint,
        &Decoder::DrawLine,
        &Decoder::DrawTesterPoints
    };

    if (step == 0)
    {
        if (data < Command::Number)
        {
            curFunc = command[data].func;
        }
        else
        {
            FinishCommand();
            return;     // ����������� ������ �� ������
        }
    }

    if (curFunc(data))
    {
        FinishCommand();
    }
    else
    {
        step++;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::InButtonPress(uint8)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        return false;
    }
    if (step == 2)
    {
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::BeginScene(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Painter::BeginScene((Color)data);
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawTesterPoints(uint8 data)
{
    // ����� �������� ������� ����������� ����. ����� �� ����� 2400
    static int numPoint = 0;
    static Color color = Color::FILL;
    static uint8 mode = 0;

    static uint8 buffer[480];

    if(step == 0)
    {
        numPoint = 0;
    }
    else if(step == 1)
    {
        mode = data;
    }
    else if(step == 2)
    {
        color = Color(data);
    }
    else
    {
        buffer[numPoint++] = data;

        if(numPoint == 480)
        {
            Painter::DrawTesterData(mode, color, buffer, buffer + 240);
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::EndScene(uint8)
{
    Painter::EndScene();
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetColor(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    if (step == 1)
    {
        Painter::SetColor((Color)data);
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::FillRegion(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            Painter::FillRegion(x, y, width, height);
            return true;
        default:
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawRectangle(uint8 data)
{
    static int x;
    static int y;
    static int width;
    static int height;

    switch (step)
    {
        case 0:                                     break;
        case 1:     x = data;                       break;
        case 2:     x += (((int)data) << 8);        break;
        case 3:     y = data;                       break;
        case 4:     width = data;                   break;
        case 5:     width += (((int)data) << 8);    break;
        case 6:     height = (int)data;
            Painter::DrawRectangle(x, y, width, height);
            return true;
        default:
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawVLine(uint8 data)
{
    static int x;
    static int y0;
    static int y1;

    switch (step)
    {
        case 0:                             break;
        case 1: x = data;                   break;
        case 2: x += (((int)data) << 8);    break;
        case 3: y0 = data;                  break;
        case 4: y1 = data;
            Painter::DrawVLine(x, y0, y1);
            return true;
        default:
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawHLine(uint8 data)
{
    __IO static int y;  /// \todo ��� ����� __IO ��������� ������, ��� ��� ��� c ������������ ����� ������ �� ����� ��������. ���� ����� �����������
    __IO static int x0;
    __IO static int x1;

    switch (step)
    {
        case 0:                             break;
        case 1:     y = data;               break;
        case 2:     x0 = data;              break;
        case 3:     x0 += (int)data << 8;   break;
        case 4:     x1 = data;              break;
        case 5:     x1 += (int)data << 8;
            Painter::DrawHLine(y, x0, x1);
            return true;
        default:
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawLine(uint8 data)
{
    __IO static int x0;
    __IO static int y0;
    __IO static int x1;

    switch (step)
    {
        case 0:                         break;
        case 1: x0 = data;              break;
        case 2: x0 += ((int)data << 8); break;
        case 3: y0 = data;              break;
        case 4: x1 = data;              break;
        case 5: x1 += ((int)data << 8); break;
        case 6: Painter::DrawLine(x0, y0, x1, data);
            return true;
        default:
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetPoint(uint8 data)
{
    static int x = 0;

    switch (step)
    {
        case 0:                         break;
        case 1: x = data;               break;
        case 2: x += ((int)data << 8);  break;
        case 3: Painter::SetPoint(x, data);
            return true;
        default:
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::DrawText(uint8 data)
{
    static int x;
    static int y;
    static int numSymbols;
    static int readingSymbols;
    static char *buffer;

    switch (step)
    {
        case 0:                             break;
        case 1:     x = data;               break;
        case 2:     x += (int)data << 8;    break;
        case 3:     y = data;               break;
        case 4:
            numSymbols = data;
            readingSymbols = 0;
            buffer = new char[(uint)(numSymbols + 1)];
            break;
        default:
            buffer[readingSymbols++] = (char)data;
            if (readingSymbols == numSymbols)
            {
                buffer[readingSymbols] = 0;
                Painter::DrawText(x, y, buffer);
                delete [] buffer;
                return true;
            }
            break;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetPalette(uint8 data)
{
    static uint8 numColor;
    static uint valueColor;

    switch (step)
    {
        case 0:                                 break;
        case 1: numColor = data;                break;
        case 2: valueColor = data;              break;
        case 3: valueColor |= (uint)data << 8;  break;
        case 4: valueColor |= (uint)data << 16; break;
        case 5: valueColor |= (uint)data << 24;
            Painter::SetColorValue(Color(numColor), valueColor);
            return true;
        default:
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Decoder::SetFont(uint8 data)
{
    if (step == 0)
    {
        return false;
    }
    Painter::SetFont((Font::Type::E)data);

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Decoder::FinishCommand()
{
    step = 0;
    curFunc = 0;
}
