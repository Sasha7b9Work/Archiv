#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Utils/StringUtils.h"
#include "Values.h"
#include <string.h>
#include <ctype.h>
#endif

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint length = strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *SU::Db2String(float value, int numDigits, char bufferOut[20])
{
    bufferOut[0] = 0;
    char buffer[20];
    strcat(bufferOut, Float(value).ToString(false, numDigits, buffer));
    strcat(bufferOut, "��");
    return bufferOut;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}


#define  SYMBOL(x) (*(*(x)))

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::ChooseSymbols(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::ChooseSpaces(const char **string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::GetWord(const char *string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = (char *)string;
            ChooseSymbols(&string);
            word->numSymbols = (int8)(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = (char)toupper(*pointer);
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::WordEqualZeroString(Word *word, char* string)
{
    char *ch = string;
    char *w = (char*)(word->address);

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::EqualsStrings(char *str1, char *str2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SU::EqualsStrings(char *str1, char *str2)
{
    uint size = strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}
