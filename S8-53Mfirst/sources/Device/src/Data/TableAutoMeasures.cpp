// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Utils/Math_.h"
#include "Data/ProcessingData.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


namespace TableAutoMeasures
{
    struct StructMeasure
    {
        pchar name;
        const char UGO;
    };

    static const StructMeasure measures[TypeMeasure::Count] =
    {
        {"",            '\x00'},
        {"U макс",      '\x20'},
        {"U мин",       '\x25'},
        {"U пик",       '\x2a'},
        {"U макс уст",  '\x40'},
        {"U мин уст",   '\x45'},
        {"U ампл",      '\x4a'},
        {"U ср",        '\x60'},
        {"U скз",       '\x65'},
        {"Выброс+",     '\x6a'},
        {"Выброс-",     '\x80'},
        {"Период",      '\x85'},
        {"Частота",     '\x8a'},
        {"Вр нараст",   '\xa0'},
        {"Вр спада",    '\xa5'},
        {"Длит+",       '\xaa'},
        {"Длит-",       '\xc0'},
        {"Скважн+",     '\xc5'},
        {"Скважн-",     '\xca'},
        {"Задержка\xa7",'\xe0'},
        {"Задержка\xa6",'\xe5'},
        {"Фаза\xa7",    '\xe0'},
        {"Фаза\xa6",    '\xe5'}
    };


    static int8 posActive = 0;                  // Позиция активного измерения (на котором курсор)
    static int8 posOnPageChoice = 0;            // Позиция курсора на странице выбора измерения

}


bool TableAutoMeasures::IsActive(int row, int col)
{
    if(posActive >= TableAutoMeasures::NumCols() * TableAutoMeasures::NumRows())
    {
        posActive = 0;
    }
    return (row * TableAutoMeasures::NumCols() + col) == posActive;
}

void TableAutoMeasures::GetActive(int *row, int *col)
{
    *row = posActive / TableAutoMeasures::NumCols();
    *col = posActive - (*row) * TableAutoMeasures::NumCols();
}

void TableAutoMeasures::SetActive(int row, int col)
{
    posActive = (int8)(row * TableAutoMeasures::NumCols() + col);
}

char TableAutoMeasures::GetChar(TypeMeasure::E measure)
{
    return measures[measure].UGO;
}

int TableAutoMeasures::GetDY()
{
    if(MEASURES_SOURCE_IS_A_B)
    {
        return 30;
    }
    return 21;
}

int TableAutoMeasures::GetDX()
{
    return Grid::WIDTH / 5; 
}

Text TableAutoMeasures::Name(int row, int col)
{
    int numMeasure = row * TableAutoMeasures::NumCols() + col;
    return Text(measures[MEASURE(numMeasure)].name);
}

TypeMeasure::E TableAutoMeasures::Type(int row, int col)
{
    int numMeasure = row * TableAutoMeasures::NumCols() + col;
    return MEASURE(numMeasure);
}


int TableAutoMeasures::GetTopTable()
{
    if(MEASURES_NUMBER_IS_6_1 || MEASURES_NUMBER_IS_6_2)
    {
        return Grid::Bottom() - TableAutoMeasures::GetDY() * 6 - 1;
    }

    return Grid::Bottom() - TableAutoMeasures::NumRows() * TableAutoMeasures::GetDY() - 1;
}


int TableAutoMeasures::NumCols()
{
    static const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[MEASURES_NUMBER];
}


int TableAutoMeasures::NumRows()
{
    static const int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[MEASURES_NUMBER];
}


int TableAutoMeasures::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MEASURES_MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(MEASURES_NUMBER_IS_6_1)
        {
            return TableAutoMeasures::GetDX();
        }
        else if(MEASURES_NUMBER_IS_6_2)
        {
            return TableAutoMeasures::GetDX() * 2;
        }
    }

    return 0;
}


int TableAutoMeasures::GetDeltaGridBottom()
{
    if(SHOW_MEASURES && MEASURES_MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(MEASURES_NUMBER_IS_1_5)
        {
            return TableAutoMeasures::GetDY();
        }
        else if(MEASURES_NUMBER_IS_2_5)
        {
            return TableAutoMeasures::GetDY() * 2;
        }
        else if(MEASURES_NUMBER_IS_3_5)
        {
            return TableAutoMeasures::GetDY() * 3;
        }
    }
    return 0;
}


void TableAutoMeasures::Handler::RegSet(int angle)
{
    static const int8 step = 3;
    static int8 currentAngle = 0;
    currentAngle += (int8)angle;

    if (currentAngle < step && currentAngle > -step)
    {
        return;
    }

    if (PageMeasures::choiceMeasuresIsActive)
    {
        posOnPageChoice += (int8)Math::Sign(currentAngle);

        Holder::RotateSwitch(angle);

        if (posOnPageChoice < 0)
        {
            posOnPageChoice = TypeMeasure::Count - 1;
        }
        else if (posOnPageChoice == TypeMeasure::Count)
        {
            posOnPageChoice = 0;
        }

        MEASURE(posActive) = (TypeMeasure::E)posOnPageChoice;
        Color::ResetFlash();
    }
    else
    {
        int row = 0;
        int col = 0;
        TableAutoMeasures::GetActive(&row, &col);
        col += Math::Sign(currentAngle);
        if (col < 0)
        {
            col = TableAutoMeasures::NumCols() - 1;
            row--;
            if (row < 0)
            {
                row = TableAutoMeasures::NumRows() - 1;
            }
        }
        else if (col == TableAutoMeasures::NumCols())
        {
            col = 0;
            row++;
            if (row >= TableAutoMeasures::NumRows())
            {
                row = 0;
            }
        }
        TableAutoMeasures::SetActive(row, col);
        Holder::RotateSwitch(angle);
    }
    currentAngle = 0;
}


void TableAutoMeasures::Handler::ButtonSettings()
{
    PageMeasures::choiceMeasuresIsActive = !PageMeasures::choiceMeasuresIsActive;

    if(PageMeasures::choiceMeasuresIsActive)
    {
        posOnPageChoice = (int8)(MEASURE(posActive));
    }
}


void TableAutoMeasures::Draw::PageChoice()
{
    if(!PageMeasures::choiceMeasuresIsActive)
    {
        return;
    }

    bool num61or62 = MEASURES_NUMBER_IS_6_1 || MEASURES_NUMBER_IS_6_2;

    int x = num61or62 ? (Grid::Right() - 3 * Grid::WIDTH / 5) : Grid::Left();
    int y = Grid::Top();
    int dX = Grid::WIDTH / 5;
    int dY = 22;
    int maxRow = num61or62 ? 8 : 5;
    int maxCol = num61or62 ? 3 : 5;
    TypeMeasure::E meas = TypeMeasure::None;

    for(int row = 0; row < maxRow; row++)
    {
        for(int col = 0; col < maxCol; col++)
        {
            if(meas >= TypeMeasure::Count)
            {
                break;
            }

            int x0 = x + col * dX;
            int y0 = y + row * dY;
            bool active = meas == posOnPageChoice;

            Rectangle(dX + 1, dY + 1).Draw(x0, y0, Color::FILL);
            Region(dX - 1, dY - 1).Fill(x0 + 1, y0 + 1, active ? Color::BLACK_WHITE_1 : Color::BACK);

            (active ? Color::BLACK_WHITE_0 : Color::FILL).SetAsCurrent();

            Char(TableAutoMeasures::GetChar(meas)).Draw10SymbolsInRect(x0 + 2, y0 + 1, TypeFont::UGO);

            Font::Set(TypeFont::S5);

            Text(measures[meas].name).DrawRelativelyRight(x0 + dX, y0 + 12);
            ++meas;
        }
    }

    Font::Set(TypeFont::S8);
}


TypeMeasure::E& operator++(TypeMeasure::E &measure)
{
    measure = (TypeMeasure::E)((int)measure + 1);
    return measure;
}


void TableAutoMeasures::Draw::All()
{
    if (!SHOW_MEASURES)
    {
        return;
    }

    if (MEASURES_FIELD_IS_HAND)
    {
        int x0 = MEASURES_POS_CUR_T_0 - SHIFT_IN_MEMORY_IN_POINTS + Grid::Left();
        int y0 = MEASURES_POS_CUR_U_0 + Grid::Top();
        int x1 = MEASURES_POS_CUR_T_1 - SHIFT_IN_MEMORY_IN_POINTS + Grid::Left();
        int y1 = MEASURES_POS_CUR_U_1 + Grid::Top();
        Math::Sort(&x0, &x1);
        Math::Sort(&y0, &y1);
        Rectangle(x1 - x0, y1 - y0).Draw(x0, y0, Color::FILL);
    }

    int x0 = Grid::Left() - TableAutoMeasures::GetDeltaGridLeft();
    int y0 = TableAutoMeasures::GetTopTable();

    int dX = TableAutoMeasures::GetDX();
    int dY = TableAutoMeasures::GetDY();

    int numRows = TableAutoMeasures::NumRows();
    int numCols = TableAutoMeasures::NumCols();

    for (int str = 0; str < numRows; str++)
    {
        for (int elem = 0; elem < numCols; elem++)
        {
            int x = x0 + dX * elem;
            int y = y0 + str * dY;
            bool active = TableAutoMeasures::IsActive(str, elem) && Menu::GetNameOpenedPage() == NamePage::MeasuresTune_SB;
            TypeMeasure::E meas = TableAutoMeasures::Type(str, elem);

            if (meas != TypeMeasure::None)
            {
                Region(dX + 1, dY + 1).FillBounded(x, y, Color::BACK, Color::FILL);
            }

            if (active)
            {
                Region(dX - 3, dY - 3).Fill(x + 2, y + 2, Color::FILL);
            }

            if (meas != TypeMeasure::None)
            {
                TableAutoMeasures::Name(str, elem).Draw(x + 4, y + 2, active ? Color::BACK : Color::FILL);

                if (meas == MEASURE_MARKED)
                {
                    Region(dX - 1, 10).Fill(x + 1, y + 1, active ? Color::BACK : Color::FILL);
                    TableAutoMeasures::Name(str, elem).Draw(x + 4, y + 2, active ? Color::FILL : Color::BACK);
                }

                if (MEASURES_SOURCE_IS_A)
                {
                    Processing::GetStringMeasure(meas, Chan::A).Draw(x + 2, y + 11, ChA.GetColor());
                }
                else if (MEASURES_SOURCE_IS_B)
                {
                    Processing::GetStringMeasure(meas, Chan::B).Draw(x + 2, y + 11, ChB.GetColor());
                }
                else
                {
                    Processing::GetStringMeasure(meas, Chan::A).Draw(x + 2, y + 11, ChA.GetColor());
                    Processing::GetStringMeasure(meas, Chan::B).Draw(x + 2, y + 20, ChB.GetColor());
                }
            }
        }
    }

    if (Menu::GetNameOpenedPage() == NamePage::MeasuresTune_SB)
    {
        TableAutoMeasures::Draw::PageChoice();
    }
}


void TableAutoMeasures::Handler::OnButonMarker()
{
    if (MEASURE(posActive) == MEASURE_MARKED)
    {
        MEASURE_MARKED = TypeMeasure::None;
    }
    else
    {
        MEASURE_MARKED = MEASURE(posActive);
    }
}
