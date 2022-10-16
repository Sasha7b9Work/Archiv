// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Painter/Painter_.h"
#include "Hardware/VCP/VCP_.h"
#include "Display/Display.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"
#include "SCPI/SCPI.h"


pchar SCPI::ProcessDISPLAY(pchar buffer)
{
    static const StructCommand commands[] =
    {
        {":AUTOSEND",    SCPI::DISPLAY::AUTOSEND},
        {":MAPPING",     SCPI::DISPLAY::MAPPING},    // �����������
        {":MAP",         SCPI::DISPLAY::MAPPING},
        {":ACCUMULATION",SCPI::DISPLAY::ACCUM},      // ����������
        {":ACCUM",       SCPI::DISPLAY::ACCUM},
        {":AVERAGE",     SCPI::DISPLAY::AVERAGE},    // ����������
        {":AVE",         SCPI::DISPLAY::AVERAGE},
        {":MINMAX",      SCPI::DISPLAY::MINMAX},     // ��� ����
        {":FILTR",       SCPI::DISPLAY::FILTR_},     // �����������
        {":FPS",         SCPI::DISPLAY::FPS},        // ������� ������
        {":WINDOW",      SCPI::DISPLAY::WINDOW},     // ���� ������
        {":GRID",        SCPI::DISPLAY::GRID},       // �����
        {0}
    };

    return SCPI::ProcessingCommand(commands, buffer);
}


pchar SCPI::DISPLAY::GRID(pchar buffer)
{
    static const StructCommand commands[] =
    {
        {":TYPE",        SCPI::DISPLAY::GRID_TYPE},
        {":BRIGHTNESS",  SCPI::DISPLAY::GRID_BRIGHTNESS},
        {0}
    };

    return SCPI::ProcessingCommand(commands, buffer);
}


pchar SCPI::DISPLAY::ACCUM(pchar buffer)
{
    static const StructCommand commands[] =
    {
        {":NUMBER",  SCPI::DISPLAY::ACCUM_NUMBER},
        {":NUM",     SCPI::DISPLAY::ACCUM_NUMBER},
        {":MODE",    SCPI::DISPLAY::ACCUM_MODE},
        {":CLEAR",   SCPI::DISPLAY::ACCUM_CLEAR},
        {0}
    };

    return SCPI::ProcessingCommand(commands, buffer);
}


pchar SCPI::DISPLAY::AVERAGE(pchar buffer)
{
    static const StructCommand commands[] =
    {
        {":NUMBER",  SCPI::DISPLAY::AVERAGE_NUMBER},
        {":NUM",     SCPI::DISPLAY::AVERAGE_NUMBER},
        {":MODE",    SCPI::DISPLAY::AVERAGE_MODE},
        {0}
    };

    return SCPI::ProcessingCommand(commands, buffer);
}


pchar SCPI::DISPLAY::AUTOSEND(pchar buffer)
{
    if      FIRST_SYMBOLS(" 1") { Display::Sender::SendPalette(); Display::Sender::SendFrame(); }   // ���� ������ ��� ������� ������� ������ � �������� � ��������
    else if FIRST_SYMBOLS(" 2") { Display::Sender::SendFrame(); }                                   // ���� ������ ��� ������� ����������� �������
    else if FIRST_SYMBOLS(" 3") { Display::Sender::SendFrame(); }                                   // ���� ������ ��� ������� ������� ������ ��� ������� � �������

    return buffer;
}


pchar SCPI::DISPLAY::MAPPING(pchar buffer)
{
    if      FIRST_SYMBOLS(" POINTS") { MODE_DRAW_SIGNAL = ModeDrawSignal::Points; }
    else if FIRST_SYMBOLS(" LINES")  { MODE_DRAW_SIGNAL = ModeDrawSignal::Lines;  }

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:MAPPING %s", MODE_DRAW_SIGNAL ? "LINES" : "POINTS"));

    return buffer;
}


pchar SCPI::DISPLAY::ACCUM_NUMBER(pchar buffer)
{
    static const MapElement map[] =
    {
        {" DIS",      0},
        {" 2",        1},
        {" 4",        2},
        {" 8",        3},
        {" 16",       4},
        {" 32",       5},
        {" 64",       6},
        {" 128",      7},
        {" INFINITY", 8},
        {0}
    };

    SCPI_CYCLE(ENUM_ACCUM = (ENumAccumulation::E)it->value);

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:ACCUMULATION:NUMBER %s", map[ENUM_ACCUM].key));

    return buffer;
}


pchar SCPI::DISPLAY::ACCUM_MODE(pchar buffer)
{
    if      FIRST_SYMBOLS(" NORESET") { MODE_ACCUM = ModeAccumulation::NoReset; }
    else if FIRST_SYMBOLS(" RESET")   { MODE_ACCUM = ModeAccumulation::Reset; }

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:ACCUMULATION:MODE %s", MODE_ACCUM_IS_RESET ? "RESET" : "NORESET"));

    return buffer;
}


pchar SCPI::DISPLAY::ACCUM_CLEAR(pchar buffer)
{
    PageDisplay::PageAccumulation::OnPress_Clear();

    return buffer;
}


pchar SCPI::DISPLAY::AVERAGE_NUMBER(pchar buffer)
{
    static const MapElement map[] =
    {
        {" DIS", 0},
        {" 2",   1},
        {" 4",   2},
        {" 8",   3},
        {" 16",  4},
        {" 32",  5},
        {" 64",  6},
        {" 128", 7},
        {" 256", 8},
        {" 512", 9},
        {0}
    };

    SCPI_CYCLE(ENUM_AVERAGE = (ENumAveraging::E)it->value);

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:AVERAGE:NUMBER%s", map[ENUM_AVERAGE].key));

    return buffer;
}


pchar SCPI::DISPLAY::AVERAGE_MODE(pchar buffer)
{
    if      FIRST_SYMBOLS(" ACCURACY")    { MODE_AVERAGE = ModeAveraging::Accurately; }
    else if FIRST_SYMBOLS(" APPROXIMATE") { MODE_AVERAGE = ModeAveraging::Around; }

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:AVARAGE:MODE %s",
        (MODE_AVERAGE == ModeAveraging::Accurately) ? "ACCURACY" : "APPROXIMATE"));

    return buffer;
}


pchar SCPI::DISPLAY::MINMAX(pchar buffer)
{
    static const MapElement map[] =
    {
        {" 1",   0},
        {" 2",   1},
        {" 4",   2},
        {" 8",   3},
        {" 16",  4},
        {" 32",  5},
        {" 64",  6},
        {" 128", 7},
        {0}
    };

    SCPI_CYCLE(ENUM_MIN_MAX = (ENumMinMax::E)it->value);

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:MINMAX%s", map[ENUM_MIN_MAX].key));

    return buffer;
}


pchar SCPI::DISPLAY::FILTR_(pchar buffer)
{
    static const MapElement map[] =
    {
        {" 1", 0},
        {" 2", 1},
        {" 3", 2},
        {" 4", 3},
        {" 5", 4},
        {" 6", 5},
        {" 7", 6},
        {" 8", 7},
        {" 9", 8},
        {" 10", 9},
        {0}
    };

    SCPI_CYCLE(SMOOTHING = (Smoothing::E)it->value);

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:FILTR%s", map[SMOOTHING].key));

    return buffer;
}


pchar SCPI::DISPLAY::FPS(pchar buffer)
{
    static const MapElement map[] =
    {
        {" 25",  0},
        {" 10",  1},
        {" 5",   2},
        {" 2",   3},
        {" 1",   4},
        {0}
    };

    SCPI_CYCLE(ENUM_FPS = (ENumSignalsInSec::E)it->value; PageDisplay::OnChanged_RefreshFPS(true));

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:FPS%s", map[ENUM_FPS].key));

    return buffer;
}


pchar SCPI::DISPLAY::WINDOW(pchar buffer)
{
    if      FIRST_SYMBOLS(" STANDARD") { SHOW_FULL_MEMORY_WINDOW = true; }
    else if FIRST_SYMBOLS(" SIMPLE")   { SHOW_FULL_MEMORY_WINDOW = false; }

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:WINDOW %s", SHOW_FULL_MEMORY_WINDOW ? "STANDARD" : "SIMPLE"));

    return buffer;
}


pchar SCPI::DISPLAY::GRID_BRIGHTNESS(pchar buffer)
{
    if FIRST_SYMBOLS("?")
    {
        PageDisplay::PageGrid::colorType.Init();
        SCPI::SendFormat(":DISPLAY:GRID:BRIGHTNESS %d", (int)(PageDisplay::PageGrid::colorType.brightness * 100.0F));
    }
    else
    {
        Int value(buffer);

        if (value.IsValid() && value >= 0 && value <= 100)
        {
            BRIGHTBESS_GRID = (int16)value;
        }
    }

    return buffer;
}


pchar SCPI::DISPLAY::GRID_TYPE(pchar buffer)
{
    static const MapElement map[] =
    {
        {" 1", 0},
        {" 2", 1},
        {" 3", 2},
        {" 4", 3},
        {0}
    };

    SCPI_CYCLE(TYPE_GRID = (TypeGrid::E)it->value);

    IF_REQUEST(SCPI::SendFormat(":DISPLAY:GRID:TYPE%s", map[TYPE_GRID].key));

    return buffer;
}
