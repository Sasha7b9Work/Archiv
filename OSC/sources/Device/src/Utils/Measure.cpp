#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Utils/Math.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StructMeasure
{
    const char *name;
    const char UGO;
    uint8      notUsed0;
    uint8      notUsed1;
    uint8      notUsed2;
};

#define DEF_STRUCT_MEASURE(name, ugo) {name, ugo, 0, 0, 0}

static const StructMeasure sMeas[Measure::Type::Number] =
{
    DEF_STRUCT_MEASURE("",            '\x00'),
    DEF_STRUCT_MEASURE("U ����",      '\x20'),
    DEF_STRUCT_MEASURE("U ���",       '\x25'),
    DEF_STRUCT_MEASURE("U ���",       '\x2a'),
    DEF_STRUCT_MEASURE("U ���� ���",  '\x40'),
    DEF_STRUCT_MEASURE("U ��� ���",   '\x45'),
    DEF_STRUCT_MEASURE("U ����",      '\x4a'),
    DEF_STRUCT_MEASURE("U ��",        '\x60'),
    DEF_STRUCT_MEASURE("U ���",       '\x65'),
    DEF_STRUCT_MEASURE("������+",     '\x6a'),
    DEF_STRUCT_MEASURE("������-",     '\x80'),
    DEF_STRUCT_MEASURE("������",      '\x85'),
    DEF_STRUCT_MEASURE("�������",     '\x8a'),
    DEF_STRUCT_MEASURE("�� ������",   '\xa0'),
    DEF_STRUCT_MEASURE("�� �����",    '\xa5'),
    DEF_STRUCT_MEASURE("����+",       '\xaa'),
    DEF_STRUCT_MEASURE("����-",       '\xc0'),
    DEF_STRUCT_MEASURE("������+",     '\xc5'),
    DEF_STRUCT_MEASURE("������-",     '\xca'),
    DEF_STRUCT_MEASURE("��������\xa7",'\xe0'),
    DEF_STRUCT_MEASURE("��������\xa6",'\xe5'),
    DEF_STRUCT_MEASURE("����\xa7",    '\xe0'),
    DEF_STRUCT_MEASURE("����\xa6",    '\xe5')
};

int8 Measure::posActive = 0;
bool Measure::pageChoiceIsActive = false;
int8 Measure::posOnPageChoice = 0;
int  Measure::Graphics::top = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Measure::IsActive()
{
    if(posActive >= NumCols() * NumRows())
    {
        posActive = 0;
    }
    return (row * NumCols() + col) == posActive;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure Measure::GetActive()
{
    int row = posActive / NumCols();
    int col = posActive - row * NumCols();

    return Measure(row, col);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetActive(int row, int col)
{
    posActive = (int8)(row * NumCols() + col);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Measure::GetChar(Measure::Type measure)
{
    return sMeas[measure].UGO;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ChangeActive(int delta)
{
    Measure measure = GetActive();

    int row = measure.row;
    int col = measure.col;

    col += Math::Sign(delta);

    if (col < 0)
    {
        col = NumCols() - 1;
        row--;
        if (row < 0)
        {
            row = NumRows() - 1;
        }
    }
    else if (col == NumCols())
    {
        col = 0;
        row++;
        if (row >= NumRows())
        {
            row = 0;
        }
    }

    SetActive(row, col);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::DY()
{
    if(VIEW_MEASURES_BOTH)
    {
        return 30;
    }
    return 21;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::DX()
{
    return Grid::Width() / 5; 
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Measure::Name()
{
    return sMeas[GetType()].name;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure::Type Measure::GetType()
{
    return set.meas_measures[row * NumCols() + col];
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::GetTopTable()
{
    if(NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2)
    {
        return Grid::Bottom() - DY() * 6;
    }

    int y = Grid::Bottom() - NumRows() * DY();

    if(Menu::IsShown())
    {
        y -= 3;
    }

    return y;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::NumCols()
{
    const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[NUM_MEASURES];
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::NumRows()
{
    int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[NUM_MEASURES];
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_6_1)
        {
            return DX();
        }
        else if(NUM_MEASURES_IS_6_2)
        {
            return DX() * 2;
        }
    }
    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::GetDeltaGridBottom()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_1_5)
        {
            return DY();
        }
        else if(NUM_MEASURES_IS_2_5)
        {
            return DY() * 2;
        }
        else if(NUM_MEASURES_IS_3_5)
        {
            return DY() * 3;
        }
    }
    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ShortPressOnSmallButonMarker()
{
    if(set.meas_measures[posActive] == MEAS_MARKED)
    {
        MEAS_MARKED = Measure::Type::None;
    }
    else
    {
        MEAS_MARKED = set.meas_measures[posActive];
    }
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::DrawPageChoice()
{
    if(!pageChoiceIsActive)
    {
        return;
    }
    int x = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? (Grid::Right() - 3 * Grid::Width() / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::Width() / 5;
    int dY = 22;
    int maxRow = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 8 : 5;
    int maxCol = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 3 : 5;
    Measure::Type meas = Measure::Type::None;
    Painter::SetFont(Font::Type::_UGO);
    for(int row = 0; row < maxRow; row++)
    {
        for(int col = 0; col < maxCol; col++)
        {
            if(meas >= Measure::Type::Number)
            {
                break;
            }
            int x0 = x + col * dX;
            int y0 = y + row * dY;
            bool active = meas == posOnPageChoice;
            Painter::DrawRectangle(x0, y0, dX, dY, Color::WHITE);
            Painter::FillRegion(x0 + 1, y0 + 1, dX - 2, dY - 2, (active ? Color::FLASH_10 : Color::BACK));
            Painter::SetColor(active ? Color::FLASH_01 : Color::FILL);
            Painter::Draw10SymbolsInRect(x0 + 2, y0 + 1, GetChar(meas));
            if(meas < Measure::Type::Number)
            {
                Painter::SetFont(Font::Type::_5);
                Painter::DrawTextRelativelyRight(x0 + dX, y0 + 12, sMeas[meas].name, active ? Color::FLASH_01 : Color::FILL);
                Painter::SetFont(Font::Type::_UGO);
            }
            meas = (Measure::Type)((int)meas + 1);    // meas++;
        }
    }
    Painter::SetFont(Font::Type::_8);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::Graphics::Draw()
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    top = Grid::Bottom();

    Processing::CalculateMeasures();

    int x0 = Grid::Left() - Measure::GetDeltaGridLeft();
    int dX = Measure::DX();
    int dY = Measure::DY();
    int y0 = Measure::GetTopTable();

    int numRows = Measure::NumRows();
    int numCols = Measure::NumCols();

    for (int str = 0; str < numRows; str++)
    {
        for (int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            Measure measure = Measure::Get(str, elem);

            bool active = measure.IsActive() && Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune;
            Color color = active ? Color::BACK : Color::FILL;

            Measure::Type type = measure.GetType();
            if (type != Measure::Type::None)
            {
                Painter::FillRegion(x, y, dX, dY, Color::BACK);
                Painter::DrawRectangle(x, y, dX, dY, Color::FILL);
                top = Math::Min(top, y);
            }
            if (active)
            {
                Painter::FillRegion(x + 2, y + 2, dX - 4, dY - 4, Color::FILL);
            }
            if (type != Measure::Type::None)
            {
#define SIZE_BUFFER 20
                char buffer[SIZE_BUFFER];

                Painter::DrawText(x + 4, y + 2, measure.Name(), color);
                if (type == MEAS_MARKED)
                {
                    Painter::FillRegion(x + 1, y + 1, dX - 2, 9, active ? Color::BACK : Color::FILL);
                    Painter::DrawText(x + 4, y + 2, measure.Name(), active ? Color::FILL : Color::BACK);
                }
                if(VIEW_MEASURES_BOTH)
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER),
                                      Color::Channel(Chan::A));
                    Painter::DrawText(x + 2, y + (SET_ENABLED_A ? 20 : 11), Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER),
                                      Color::Channel(Chan::B));
                }
                else if (VIEW_MEASURES_A)
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(type, Chan::A, buffer, SIZE_BUFFER), Color::Channel(Chan::A));
                }
                else if (VIEW_MEASURES_B)
                {
                    Painter::DrawText(x + 2, y + 11, Processing::GetStringMeasure(type, Chan::B, buffer, SIZE_BUFFER), Color::Channel(Chan::B));
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == Page::Name::Measures_Auto_Tune)
    {
        Measure::DrawPageChoice();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Measure::Graphics::GetTop()
{
    return 10;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerVoltage(Chan ch, int num, float value)
{
    Processing::markerVoltage[ch][num] = (int)value;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerTime(Chan ch, int num, int value)
{
    Processing::markerTime[ch][num] = value;
}
