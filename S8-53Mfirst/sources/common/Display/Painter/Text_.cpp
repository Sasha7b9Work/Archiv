// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Utils/Math_.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>


static bool ByteFontNotEmpty(int eChar, int byte)
{
    return (byte < 8) ? (Font::font->symbol[eChar].bytes[byte] != 0) : false;
}


static bool BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;
    static int prevChar = -1;
    static int prevNumByte = -1;

    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = Font::font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }

    return prevByte & (1 << bit);
}


static void DrawCharInColorDisplay(int eX, int eY, uchar symbol)
{
    int8 width = (int8)(Font::font->symbol[symbol].width);
    int8 height = (int8)(Font::font->height);

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Point().Draw(x, y);
                }
                x++;
            }
        }
    }
}


static void DrawCharHardCol(int x, int y, char symbol)
{
    char str[2] = { symbol, 0 };
    Text(str).Draw(x, y);
}


int Char::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    if (Font::GetSize() == 5)
    {
        DrawCharHardCol(x, y + 3, symbol);
    }
    else if (Font::GetSize() == 8)
    {
        DrawCharHardCol(x, y, symbol);
    }
    else
    {
        DrawCharInColorDisplay(x, y, (uint8)(symbol));
    }
    return x + Font::GetLengthSymbol((uint8)(symbol));
}


int Char::Draw(int x, int y, TypeFont::E typeFont, const Color &color) const
{
    Font::Set(typeFont);

    int result = Draw(x, y, color);

    Font::Set(TypeFont::S8);

    return result;
}


Text::Text(const char symbol)
{
    char temp_buffer[2] = { symbol, '\0' };

    SetString(temp_buffer);
}


Text::Text(pchar format, ...)
{
    char temp_buffer[1024];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(temp_buffer, format, args);
    va_end(args);

    SetString(temp_buffer);
}


int Text::DrawChar(int eX, int eY, uint8 symbol) const
{
    int8 width = (int8)Font::font->symbol[symbol].width;
    int8 height = (int8)Font::font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Point().Draw(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


static uint GetLenghtSubString(char *text)
{
    uint result = 0;

    while (((*text) != ' ') && ((*text) != '\0'))
    {
        result += Font::GetLengthSymbol((uint8)(*text));
        text++;
    }

    return result;
}


static int DrawSubString(int x, int y, char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Char(*text).Draw(x, y);
        numSymbols++;
        text++;
    }
    return numSymbols;
}


static int DrawSpaces(int x, int y, char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = Char(*text).Draw(x, y);
        text++;
        (*numSymbols)++;
    }
    return x;
}


void Text::DrawInRect(int x, int y, uint width, uint)
{
    int xStart = x;
    int xEnd = xStart + (int)width;

    char *t = c_str();

    while (*t != 0)
    {
        int length = (int)GetLenghtSubString(t);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol();
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, t);
        t += numSymbols;
        x += length;
        x = DrawSpaces(x, y, t, &numSymbols);
        t += numSymbols;
    }
}


static int DrawBigChar(int eX, int eY, uint size, char symbol)
{
    uint8 width = Font::font->symbol[(uint8)symbol].width;
    uint8 height = (uint8)Font::font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = (int)(eY + b * size + 9 - height);
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (uint i = 0; i < size; i++)
                    {
                        for (uint j = 0; j < size; j++)
                        {
                            Point().Draw((int)(x + i), (int)(y + j));
                        }
                    }
                }
                x += size;
            }
        }
    }

    return (int)(eX + width * size);
}


void Text::DrawBig(int eX, int eY, uint size)
{
    int numSymbols = (int)std::strlen(c_str());

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, (*this)[i]);
        x += size;
    }
}


void Char::Draw4SymbolsInRect(int x, int y, TypeFont::E typeFont, const Color &color)
{
    Font::Set(typeFont);

    Draw4SymbolsInRect(x, y, color);

    Font::Set(TypeFont::S8);
}


void Char::Draw4SymbolsInRect(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    for (int i = 0; i < 2; i++)
    {
        Char((char)(symbol + i)).Draw(x + 8 * i, y);
        Char((char)(symbol + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


int Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, const Color &color)
{
    color.SetAsCurrent();

    int lenght = Font::GetLengthText(c_str());
    int height = Font::GetHeightSymbol();
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return Draw(x, y);
}


void Text::DrawRelativelyRight(int xRight, int y, const Color &color)
{
    color.SetAsCurrent();

    int lenght = Font::GetLengthText(c_str());
    Draw(xRight - lenght, y);
}


int Text::DrawOnBackground(int x, int y, const Color &colorBackground)
{
    int width = Font::GetLengthText(c_str());
    int height = Font::GetSize();

    Color colorText = Color::Get();
    Region(width, height).Fill(x - 1, y, colorBackground);
    colorText.SetAsCurrent();

    return Draw(x, y);
}


void Char::Draw10SymbolsInRect(int x, int y, TypeFont::E typeFont)
{
    Font::Set(typeFont);

    Draw10SymbolsInRect(x, y);

    Font::Set(TypeFont::S8);
}


void Char::Draw10SymbolsInRect(int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        Char((char)(symbol + i)).Draw(x + 8 * i, y);
        Char((char)(symbol + i + 16)).Draw(x + 8 * i, y + 8);
    }
}


static int DrawCharWithLimitation(int eX, int eY, uchar symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int8 width = (int8)(Font::font->symbol[symbol].width);
    int8 height = (int8)(Font::font->height);

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}


int Text::DrawWithLimitation(int x, int y, const Color &color, int limitX, int limitY, int limitWidth, int limitHeight)
{
    color.SetAsCurrent();
    int retValue = x;

    char *t = c_str();

    while (*t)
    {
        x = DrawCharWithLimitation(x, y, (uint8)(*t), limitX, limitY, limitWidth, limitHeight);
        retValue += Font::GetLengthSymbol((uint8)(*t));
        t++;
    }
    return retValue + 1;
}


static bool IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
    };

    return isLetter[(uint8)(symbol)];
}


static char *GetWord(pchar firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}


static bool IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)(symbol)];
}


static bool CompareArrays(const bool *array1, const bool *array2, int numElems)
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}


// ������� ��������� �������. C letters ���������� ����� �����, ��� ����� ����� �������, � lettersInSyllable �����
// �������� ����� ���� � ��������� �����. ���� ����� �����������, ������� ���������� false
static bool FindNextTransfer(pchar letters, int8 *lettersInSyllable)
{

#define VOWEL       0   // �������
#define CONSONANT   1   // ���������

    *lettersInSyllable = (int8)(std::strlen(letters)); //-V1029

    if (*lettersInSyllable <= 3)
    {
        return false;
    }

    static const bool template1[3] = { false, true, true };               // 011   2   // ����� ������� ������� �������
    static const bool template2[4] = { true, false, true, false };        // 1010  2
    static const bool template3[4] = { false, true, false, true };        // 0101  3
    static const bool template4[4] = { true, false, true, true };         // 1011  3
    static const bool template5[4] = { false, true, false, false };       // 0100  3
    static const bool template6[4] = { true, false, true, true };         // 1011  3
    static const bool template7[5] = { true, true, false, true, false };  // 11010 3
    static const bool template8[6] = { true, true, false, true, true };   // 11011 4

    bool consonant[20];

    uint size = std::strlen(letters);
    for (uint i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (std::strlen(letters) < 5)
    {
        return false;
    }
    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) ||
        CompareArrays(template5, consonant, 4) || CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (std::strlen(letters) < 6)
    {
        return false;
    }
    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }
    return false;
}


static int8 *BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;
    if (std::strcmp(word, "���������") == 0)
    {
        int8 lengths[] = { 5, 2, 2, 0 };
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "���������������") == 0)
    {
        int8 lenghts[] = { 4, 3, 4, 5, 3, 0 };
        std::memcpy(lengthSyllables, lenghts, 6);
    }
    return lengthSyllables;
}


// ���������� ����� ����� �� ����� numSyllable(�����������) ������ �� ������ ��������
static char *PartWordForTransfer(char *word, const int8 *lengthSyllables, int, int numSyllable, char buffer[30])
{
    int length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += lengthSyllables[i];
    }
    std::memcpy((void *)(buffer), (void *)(word), (uint)(length));
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


// ���� draw == false, �� �������� ������ �� ����, ������ ������������ ������ ��� ����������
static int DrawPartWord(char *word, int x, int y, int xRight, bool draw)
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, numSyllabels, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                Text(subString).Draw(x, y);
            }
            return (int)(std::strlen(subString)) - 1;
        }
    }

    return 0;
}


// ���������� ������ ������, ������� ����� ����� text, ��� ������ �� left �� right � ���������� height.
// ���� bool == false, �� ����� �� ������ �� ����� 
static bool GetHeightTextWithTransfers(int left, int top, int right, pchar text, int *height)
{
    char buffer[20];
    int numSymb = (int)(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x += Font::GetLengthSymbol((uint8)(symbol));
            }
            else                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymbols = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    *height = Math::Limitation(y - top + 4, 0, 239);

    return curSymbol == numSymb;
}


int Text::DrawInBoundedRectWithTransfers(int x, int y, int width, const Color &colorBackground, const Color &colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, c_str(), &height);

    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    DrawInRectWithTransfers(x + 3, y + 3, width - 8, height, colorFill);
    return y + height;
}


int Text::DrawInRectWithTransfers(int eX, int eY, int eWidth, int eHeight, const Color &color)
{
    color.SetAsCurrent();

    int top = eY;
    int left = eX;
    int right = eX + eWidth;
    int bottom = eY + eHeight;

    char buff[20];
    int numSymb = (int)Size();

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < bottom && curSymbol < numSymb)
    {
        while (x < right - 1 && curSymbol < numSymb)
        {
            int length = 0;
            char *word = GetWord(c_str() + curSymbol, &length, buff);

            if (length <= 1)                            // ��� ��������� �������� ��� ����, �.�. ����� �� �������
            {
                char symbol = (*this)[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x = Char(symbol).Draw(x, y);
            }
            else                            // � ����� ������� �� ������� ���� ��� ��������� �������, �.�. ������� �����
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymbols = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymbols;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = Text(word).Draw(x, y);
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


void Text::DrawInCenterRectOnBackground(int x, int y, int width, int height, const Color &colorText, int widthBorder,
    const Color &colorBackground)
{
    int lenght = Font::GetLengthText(c_str());
    int eX = DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Fill(eX - lenght - widthBorder, y - widthBorder);
    DrawInCenterRect(x, y, width, height, colorText);
}


int Text::DrawInCenterRectAndBoundIt(int x, int y, int width, int height, const Color &colorBackground,
    const Color &colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Region(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    colorFill.SetAsCurrent();
    return DrawInCenterRect(x, y, width, height);
}


Text::~Text()
{
}


void Char::Draw2SymbolsInPosition(int x, int y, char symbol2, const Color &color1, const Color &color2)
{
    Char(symbol).Draw(x, y, color1);
    Char(symbol2).Draw(x, y, color2);
}


int Text::Draw(int x, int y, TypeFont::E typeFont, const Color &color) const
{
    Font::Set(typeFont);

    int result = Draw(x, y, color);

    Font::Set(TypeFont::S8);

    return result;
}


int Text::Draw(int x, int y, const Color &color) const
{
    color.SetAsCurrent();

    return Draw(x, y);
}


int Text::Draw(int x, int y, TypeFont::E typeFont) const
{
    Font::Set(typeFont);

    int result = Draw(x, y);

    Font::Set(TypeFont::S8);

    return result;
}


int Text::Draw(int x, int y) const
{
    char *pointer = c_str();

    while (*pointer != '\0')
    {
        x = DrawChar(x, y, (uint8)(*pointer)) + 1;
        pointer++;
    }

    return x;
}


static int DrawBigCharInBuffer(int eX, int eY, int size, char symbol, uint8 *buffer)
{
    int8 width = (int8)Font::GetLengthSymbol((uchar)symbol);
    int8 height = (int8)Font::GetHeightSymbol();

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            int fullX = x + i;
                            int fullY = y + j;

                            if (fullX >= 0 && fullX < 320 && fullY >= 0 && fullY < 240)
                            {
                                *(buffer + fullY * 320 + fullX) = 1;
                            }
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void Text::DrawBigTextInBuffer(int eX, int eY, int size, pchar text, uint8 *buffer)
{
    for (int x = 0; x < 320; x++)
    {
        for (int y = 0; y < 240; y++)
        {
            *(buffer + (y * 320) + x) = 0;
        }
    }

    int numSymbols = (int)std::strlen(text);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigCharInBuffer(x, eY, size, text[i], buffer);
        x += size;
    }
}
