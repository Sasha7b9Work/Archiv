// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tables.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "Settings/Settings.h"


namespace Tables
{
    // ������� �������� �������� � ������������ ��� ����� ������
    pchar symbolsAlphaBet[0x48] =
    {
        /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
        /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
        /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
        //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - ��� ���������� ����� �������� � ���� �������
        /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
        //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - ��� ����� ���������� �������� �������� � set_.memory.fileNameMask
    };
}


void Tables::DrawStr(int index, int x, int y)
{
    pchar str = symbolsAlphaBet[index];

    if (index == INDEX_SYMBOL_NAME_MASK)
    {
        Region(Font::GetLengthText(str), 9).Fill(x - 1, y, Color::BLACK_WHITE_0);
    }

    Text(symbolsAlphaBet[index]).Draw(x, y, (index == INDEX_SYMBOL_NAME_MASK) ?
        Color::BLACK_WHITE_1 : Color::FILL);
}
