#include "defines.h"
#include "Strings.h"
#include <ctype.h>


static bool ChooseSymbols(const uint8 **string);    // ?????????? false, ???? ????? ?????????? - ?????? ?????????.
static bool ChooseSpaces(const uint8 **string);     // ?????????? false, ???? ????? ?????????? - ?????? ?????????.


int GetNumWordsInString(const uint8 *string)
{

    ChooseSpaces(&string);

    while (true)
    {
        int numWords = 0;
        
        if (ChooseSymbols(&string))
        {
            numWords++;
        }
        else
        {
            return numWords;
        }
        ChooseSpaces(&string);
    }
}

bool GetWord(const uint8 *string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = (uint8*)string;
            ChooseSymbols(&string);
            word->numSymbols = string - word->address;
            
            uint8 *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = (uint8)toupper((int8)*pointer);
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

bool WordEqualZeroString(Word *word, char* string)
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

#define  SYMBOL(x) (*(*(x)))

bool ChooseSymbols(const uint8 **string)
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

bool ChooseSpaces(const uint8 **string)
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

bool EqualsStrings(char *str1, char *str2, int size)
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

bool EqualsZeroStrings(char *str1, char *str2)
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
