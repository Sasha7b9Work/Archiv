// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panel/Panel.h"
#include "SCPI/SCPI.h"


pchar SCPI::CONTROL::KEY(pchar buffer)
{
    static const MapElement keys[] =
    {
        {" MENU",    13},
        {" 1",       14},
        {" 2",       15},
        {" 3",       16},
        {" 4",       17},
        {" 5",       18},
        {" CURSORS", 9},
        {" CURS",    9},
        {" MEASURES",10},
        {" MEAS",    10},
        {" DISPLAY", 4},
        {" DISPL",   4},
        {" HELP",    12},
        {" MEMORY",  6},
        {" MEM",     6},
        {" SERVICE", 2},
        {" SERV",    2},
        {" START",   8},
        {" CHANNEL1",1},
        {" CHAN1",   1},
        {" CHANNEL2",3},
        {" CHAN2",   3},
        {" TIME",    5},
        {" TRIG",    7},
        {0}
    };

    Word command;
    Word parameter;

    if (command.GetWord(buffer, 0) && parameter.GetWord(buffer, 1))
    {
        int numKey = 0;
        char *name = keys[numKey].key;
        while (name != 0)
        {
            if (command.WordEqualZeroString(name))
            {
                uint16 code = keys[numKey].value;
                if (parameter.WordEqualZeroString("DOWN"))
                {
                    code += 128;
                }

//              Panel::ProcessingCommandFromPIC(code);

                return buffer;
            }
            numKey++;
            name = keys[numKey].key;
        }
    }

    return buffer;
}


pchar SCPI::CONTROL::GOVERNOR(pchar buffer)
{
    static const MapElement governors[] =
    {
        {" RSHIFT1", 21},
        {" RSHIFT2", 23},
        {" RANGE1",  20},
        {" RANGE2",  22},
        {" SET",     27},
        {" TSHIFT",  25},
        {" TBASE",   24},
        {" TRIGLEV", 26},
        {0}
    };

    Word command;
    Word parameter;

    if (command.GetWord(buffer, 0) && parameter.GetWord(buffer, 1))
    {
        int numGov = 0;
        char *name = governors[numGov].key;
        while (name != 0) {
            if (command.WordEqualZeroString(name))
            {
                uint16 code = governors[numGov].value;
                if (parameter.WordEqualZeroString("RIGHT"))
                {
                    code += 128;
                }

//                Panel::ProcessingCommandFromPIC(code);
                return buffer;
            }
            numGov++;
            name = governors[numGov].key;
        }
    }

    return buffer;
}
