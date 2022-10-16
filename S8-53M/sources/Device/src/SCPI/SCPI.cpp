// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/VCP/VCP_.h"
#include "common/Utils/StringUtils_.h"
#include "common/Utils/Containers/String_.h"
#include "Hardware/LAN/LAN.h"
#include "SCPI/SCPI.h"
#include "SCPI/BufferSCPI.h"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstdarg>
#include <cstdio>


namespace SCPI
{
	static pchar ParseNewCommand(pchar buffer);

	static BufferSCPI input;
}


void SCPI::AppendNewData(const uint8 *data, int length)
{
    input.Append(data, length);
}


void SCPI::Update()
{
    input.RemoveFirstDividers();

    while (input.ExistDivider())
    {
        String command = input.ExtractCommand();

        String temp(command);

        for (int i = 0; i < command.Size(); i++)
        {
            command[i] = (char)(std::toupper((int8)(command[i])));
        }

        pchar result = SCPI::ParseNewCommand(command.c_str());

        if (*result)
        {
            SCPI::SendFormat("Error !!! Invalid sequency \"%s\"", result);
        }
    }
}


pchar SCPI::ParseNewCommand(pchar buffer)
{
    static const StructCommand commands[] =
    {
        {"*IDN?",       SCPI::COMMON::IDN},
        {":RUN",        SCPI::COMMON::RUN},
        {":STOP",       SCPI::COMMON::STOP},
        {"*RST",        SCPI::COMMON::RESET},
        {":AUTOSCALE",  SCPI::COMMON::AUTOSCALE},
        {":REQUEST ?",  SCPI::COMMON::REQUEST},

        {":DISPLAY",    ProcessDISPLAY},       // ������� ������ ��� ������ �����, ����� ����������.
        {":DISP",       ProcessDISPLAY},       // ��� ����� ��� ����������� ��������.

        {":CHANNEL1",   ProcessCHANNEL},
        {":CHAN1",      ProcessCHANNEL},

        {":CHANNEL2",   ProcessCHANNEL},
        {":CHAN2",      ProcessCHANNEL},

        {":TRIGGER",    ProcessTRIG},
        {":TRIG",       ProcessTRIG},

        {":TBASE",      ProcessTBASE},
        {":TBAS" ,      ProcessTBASE},

        {":KEY",        SCPI::CONTROL::KEY},
        {":GOVERNOR",   SCPI::CONTROL::GOVERNOR},
        {0}
    };

    return SCPI::ProcessingCommand(commands, buffer);
}



pchar SCPI::ProcessingCommand(const StructCommand *commands, pchar buffer)
{
    for (const StructCommand *command = commands; command->name != 0; command++)
    {
        uint size_name = std::strlen(command->name);

        if (SU::EqualsStrings(buffer, command->name, size_name))
        {
            return command->func(buffer + size_name);
        }
    }

    return buffer;
}


bool SCPI::FirstSymbols(pchar *data, pchar word)
{
    pchar buffer = *data;

    while (*word && *word == *buffer)
    {
        word++;
        buffer++;
    }

    if (*word)
    {
        return false;
    }

    *data = buffer;

    return true;
}


void SCPI::SendBuffer(const uint8 *buffer, int size)
{
    VCP::SendBuffer(buffer, size);

    LAN::SendBuffer(buffer, size);
}


void SCPI::SendFormat(pchar format, ...)
{
    char buffer[128];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\n");

    VCP::SendMessage(buffer);

    LAN::SendMessage(buffer);
}


uint8 SCPI::MapElement::GetValue(Word *eKey) const
{
    const MapElement *map = this;

    int numKey = 0;
    char *_key = map[numKey].key;

    while (_key != 0)
    {
        if (eKey->WordEqualZeroString(_key))
        {
            return map[numKey].value;
        }

        numKey++;
        _key = map[numKey].key;
    }
    return 255;
}
