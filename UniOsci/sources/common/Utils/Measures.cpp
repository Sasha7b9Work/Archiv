#include "defines.h"
#include "Measures.h"
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Grid.h"
#include "Hardware/Sound.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    const char *name;
    const char UGO;
} StructMeasure;

static const StructMeasure sMeas[Meas_NumMeasures] =
{
    {"",            '\x00'},
    {"U ����",      '\x20'},
    {"U ���",       '\x25'},
    {"U ���",       '\x2a'},
    {"U ���� ���",  '\x40'},
    {"U ��� ���",   '\x45'},
    {"U ����",      '\x4a'},
    {"U ��",        '\x60'},
    {"U ���",       '\x65'},
    {"������+",     '\x6a'},
    {"������-",     '\x80'},
    {"������",      '\x85'},
    {"�������",     '\x8a'},
    {"�� ������",   '\xa0'},
    {"�� �����",    '\xa5'},
    {"����+",       '\xaa'},
    {"����-",       '\xc0'},
    {"������+",     '\xc5'},
    {"������-",     '\xca'},
    {"��������\xa7",'\xe0'},
    {"��������\xa6",'\xe5'},
    {"����\xa7",    '\xe0'},
    {"����\xa6",    '\xe5'}
};

int8 posActive = 0;                 ///< ������� ��������� ��������� (�� ������� ������)
bool pageChoiceIsActive = false;    ///< ���� true - �������� �������� ������ ���������
int8 posOnPageChoice = 0;           ///< ������� ������� �� �������� ������ ���������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Measures::IsActive(int row, int col)
{
    if(posActive >= NumCols() * NumRows())
    {
        posActive = 0;
    }
    return (row * NumCols() + col) == posActive;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Measures::GetActive(int *row, int *col)
{
    *row = posActive / NumCols();
    *col = posActive - (*row) * NumCols();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Measures::SetActive(int row, int col)
{
    posActive = (int8)(row * NumCols() + col);
}

char Measures::GetChar(Meas measure)
{
    return sMeas[measure].UGO;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::GetDY()
{
    if(SOURCE_MEASURE_IS_A_B && SET_ENABLED_A && SET_ENABLED_B)
    {
        return 30;
    }
    return 21;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::GetDX()
{
    return GRID_WIDTH / 5; 
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Measures::Name(int row, int col)
{
    return sMeas[MEASURE(row * NumCols() + col)].name;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
Meas Measures::Type(int row, int col)
{
    return MEASURE(row * NumCols() + col);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::GetTopTable()
{
    if(NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2)
    {
        return GRID_BOTTOM - GetDY() * 6;
    }
    return GRID_BOTTOM - NumRows() * GetDY();
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::NumCols()
{
    const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[NUM_MEASURES];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::NumRows()
{
    int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[NUM_MEASURES];
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_6_1)
        {
            return GetDX();
        }
        else if(NUM_MEASURES_IS_6_2)
        {
            return GetDX() * 2;
        }
    }
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
int Measures::GetDeltaGridBottom()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_1_5)
        {
            return GetDY();
        }
        else if(NUM_MEASURES_IS_2_5)
        {
            return GetDY() * 2;
        }
        else if(NUM_MEASURES_IS_3_5)
        {
            return GetDY() * 3;
        }
    }
    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Measures::ShortPressOnSmallButonMarker()
{
    if(MEASURE(posActive) == MEAS_MARKED)
    {
        MEAS_MARKED = Meas_None;
    }
    else
    {
        MEAS_MARKED = MEASURE(posActive);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Measures::DrawPageChoice()
{
    if(!pageChoiceIsActive)
    {
        return;
    }
    int x = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? (Grid::Right() - 3 * GRID_WIDTH / 5) : Grid::Left();
    int y = GRID_TOP;
    int dX = GRID_WIDTH / 5;
    int dY = 22;
    int maxRow = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 8 : 5;
    int maxCol = (NUM_MEASURES_IS_6_1 || NUM_MEASURES_IS_6_2) ? 3 : 5;
    Meas meas = Meas_None;
    Painter::SetFont(TypeFont_UGO);
    for(int row = 0; row < maxRow; row++)
    {
        for(int col = 0; col < maxCol; col++)
        {
            if(meas >= Meas_NumMeasures)
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
            if(meas < Meas_NumMeasures)
            {
                Painter::SetFont(TypeFont_5);
                Painter::DrawTextRelativelyRight(x0 + dX, y0 + 12, sMeas[meas].name, active ? Color::FLASH_01 : Color::FILL);
                Painter::SetFont(TypeFont_UGO);
            }
            meas = (Meas)((int)meas + 1);    // meas++;
        }
    }
    Painter::SetFont(TypeFont_8);
}
