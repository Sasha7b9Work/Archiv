// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Display/Font/Font_.h"
#include "common/Display/Painter/Primitives_.h"
#include "common/Display/Painter/Text_.h"
#include "common/Hardware/Sound_.h"
#include "common/Hardware/HAL/HAL_.h"
#include "common/Utils/Math_.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cstdio>
#include <cstring>


const char Holder::chars[4] =
{
    Symbol::S8::GOVERNOR_SHIFT_0,
    Symbol::S8::GOVERNOR_SHIFT_1,
    Symbol::S8::GOVERNOR_SHIFT_2,
    Symbol::S8::GOVERNOR_SHIFT_3
};

int Holder::angle = 0;


Key::E GetFuncButtonFromY(int _y)
{
    int y = Grid::Top() + Grid::Height() / 12;
    int step = Grid::Height() / 6;
    Key::E key = Key::None;
    for (int i = 0; i < 6; i++)
    {
        if (_y < y)
        {
            return key;
        }
        ++key;
        y += step;
    }
    return  Key::F5;
}

 
void GraphicGovernor::Draw(const Item *item, int x, int y)
{
    bool pressed = item->IsPressed();

    bool opened = item->IsOpened();

    int delta = pressed ? 1 : 0;
    int width = Item::WIDTH_VALUE;

    if (item->IsIP() && opened && ((IPaddress*)item)->OwnData()->port != 0)
    {
        width += IPaddress::OPENED_WIDTH;
    }

    HLine().Draw(y + 1, x, x + width + 3, Color::BorderMenu());

    item->DrawVolumeButton(x + 1, y + 2, width + 3, Item::HEIGHT_VALUE + 3, 2, Color::MENU_ITEM,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, pressed, item->IsActive());

    Text(item->Title()).Draw(x + 6 + delta, y + 6 + delta, COLOR_SCHEME_IS_WHITE_LETTERS ? Color::WHITE : Color::BLACK);

    if(item->IsCurrent())
    {
        Holder::Draw(x + Item::WIDTH - 13, y + 5 + (item->IsOpened() ? 0 : 15),
                              COLOR_SCHEME_IS_WHITE_LETTERS ? Color::BACK : Color::FILL);
    }
}


int GraphicGovernor::CalculatePosition(const Item *item)
{
    int value = 0;

    if (item->IsGovernor())
    {
        value = *item->ReinterpretToGovernor()->OwnData()->cell;
    }
    else if (item->IsChoice() || item->IsChoiceReg())
    {
        value = *item->ReinterpretToChoice()->OwnData()->cell;
    }
    else if (item->IsTime())
    {
        const DataTime* own = item->ReinterpretToTime()->OwnData();

        if (item->IsOpened() && (*own->curField != iEXIT) && (*own->curField != iSET))
        {
            int8 values[7] =
            {
                0,
                *own->day,
                *own->month,
                *own->year,
                *own->hours,
                *own->minutes,
                *own->seconds
            };

            value = values[*own->curField];
        }
    }

    while (value < 0)
    {
        value += NUM_STATES;
    }

    return value % NUM_STATES;
}


void Governor::DrawLowPart(int x, int y) const
{
    const DataGovernor *own = OwnData();
    
    Color colorTextDown = Color::BACK;

    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 2, Item::HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    x = Text(Symbol::S8::TRIANGLE_LEFT).Draw(x + 4, y + 21, colorTextDown);

    x++;

    if(!IsOpened())
    {
        float delta = Step();

        x--;

        if(delta == 0.0F) //-V550
        {
            x = Int(*own->cell).ToText(false, 1).Draw(x + 1, y + 21) + 1;
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y + 19;
            int limWidth = Item::WIDTH_VALUE;
            int limHeight = Item::HEIGHT_VALUE - 1;

            if(delta > 0.0F)
            {
                x = Int(*own->cell).ToText().DrawWithLimitation(drawX,
                    (int)(y + 21 - delta), Color::BACK, limX, limY, limWidth, limHeight);

                Int(NextValue()).ToText().DrawWithLimitation(drawX,
                    (int)(y + 21 + 10 - delta), Color::BACK, limX, limY, limWidth, limHeight);
            }

            if(delta < 0.0F)
            {
                x = Int(*own->cell).ToText().DrawWithLimitation(drawX,
                    (int)(y + 21 - delta), Color::BACK, limX, limY, limWidth, limHeight);

                Int(PrevValue()).ToText().DrawWithLimitation(drawX,
                    (int)(y + 21 - 10 - delta), Color::BACK, limX, limY, limWidth, limHeight);
            }
        }
    }
    else
    {
        x = Int(*own->cell).ToText().Draw(x + 1, y + 21, Color::FILL);
    }

    Text(Symbol::S8::TRIANGLE_RIGHT).Draw(x, y + 21, colorTextDown);
}

void IPaddress::DrawLowPart(int x, int y) const
{
    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 2, Item::HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    const DataIPaddress *own = OwnData();

    Text("%03d.%03d.%03d.%03d", *own->ip0, *own->ip1, *own->ip2, *own->ip3).
        Draw(x + 4, y + 21, IsOpened() ? Color::FILL : Color::BACK);
}

void MACaddress::DrawLowPart(int x, int y) const
{
    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 2, Item::HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    const DataMACaddress *own = OwnData();

    Text("%02X.%02X.%02X.%02X.%02X.%02X", *own->mac0, *own->mac1, *own->mac2, *own->mac3, *own->mac4, *own->mac5).
        Draw(x + 4, y + 21, IsOpened() ? Color::FILL : Color::BACK);
}

void Formula::WriteText(int x, int y, bool) const
{
    const DataFormula *own = OwnData();

    if (*own->function != Function::Mul && *own->function != Function::Sum)
    {
        return;
    }

    bool funcIsMul = (*own->function == Function::Mul);
    int8 koeff1 = funcIsMul ? *own->koeff1mul : *own->koeff1add;
    int8 koeff2 = funcIsMul ? *own->koeff2mul : *own->koeff2add;
    if (koeff1 != 0)
    {
       Char(koeff1 < 0 ? '-' : '+').Draw(x, y);
    }
    Char((char)(koeff1 + 0x30)).Draw(x + 5, y);
    Char('*').Draw(x + 10, y);
    Text("K1").Draw(x + 14, y);
    Char(funcIsMul ? '*' : '+').Draw(x + 27, y);
    if (koeff2 != 0)
    {
       Char(koeff2 < 0 ? '-' : '+').Draw(x + 30, y);
    }
    Char((char)(koeff2 + 0x30)).Draw(x + 39, y);
    Char('*').Draw(x + 44, y);
    Text("K2").Draw(x + 48, y);
}

void Formula::DrawLowPart(int x, int y) const
{
    Color colorTextDown = Color::BACK;

    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 2, Item::HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    colorTextDown.SetAsCurrent();
    WriteText(x + 6, y + 21, false);
}


void Governor::DrawClosed(int x, int y) const
{
    DrawLowPart(x, y);
    GraphicGovernor::Draw(this, x, y);
}


void IPaddress::DrawClosed(int x, int y) const
{
    DrawLowPart(x, y);
    GraphicGovernor::Draw(this, x, y);
}


void MACaddress::DrawClosed(int x, int y) const
{
    DrawLowPart(x, y);
    GraphicGovernor::Draw(this, x, y);
}


void Formula::DrawClosed(int x, int y) const
{
    DrawLowPart(x, y);
    GraphicGovernor::Draw(this, x, y);
}


// ���� selPos == -1, ������������ �� �����
static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull)
{
    int firstValue = value;
    int height = hLine ? 9 : 8;

    for (int i = 0; i < numDigits; i++)
    {
        int rest = value % 10;
        value /= 10;

        if (selPos == i)
        {
            Region(6, height).Fill(x - 1, y, Color::BLACK_WHITE_1);
        }

        if (!(rest == 0 && value == 0) || (firstValue == 0 && i == 0))
        {
            Char((char)(rest + 48)).Draw(x, y, selPos == i ? Color::BLACK_WHITE_0 : Color::FILL);
        }
        else if (fillNull)
        {
            Char('0').Draw(x, y, selPos == i ? Color::BACK : Color::FILL);
        }

        if (hLine)
        {
            Line().Draw(x, y + 9, x + 3, y + 9, Color::FILL);
        }

        x -= 6;
    }
}

void Governor::DrawValue(int x, int y) const
{
    const DataGovernor *own = OwnData();

    int startX = x + 40;
    int16 value = *own->cell;
    int signGovernor = *own->cell < 0 ? -1 : 1;

    if(signGovernor == -1)
    {
        value = -value;
    }

    bool sign = own->minValue < 0;
    Int(own->maxValue).ToText(sign).Draw(x + 55, y - 5, TypeFont::S5, Color::FILL);
    Int(own->minValue).ToText(sign).Draw(x + 55, y + 2, TypeFont::S5);

    DrawValueWithSelectedPosition(startX, y, value, NumDigits(), gCurDigit, true, true);

    if(sign)
    {
       Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 1, y);
    }
}

void IPaddress::DrawValue(int x, int y) const
{
    const DataIPaddress *own = OwnData();

    if (gCurDigit > (own->port == 0 ? 11 : 16))
    {
        gCurDigit = 0;
    }

    uint8 *bytes = own->ip0;

    x += 15;

    y += 1;

    int numIP = 0;
    int selPos = 0;

    IPaddress::GetNumPosIPvalue(&numIP, &selPos);

    for (int i = 0; i < 4; i++)
    {
        DrawValueWithSelectedPosition(x, y, bytes[i], 3, numIP == i ? selPos : -1, false, true);
        if (i != 3)
        {
            Char('.').Draw(x + 5, y, Color::FILL);
        }
        x += 19;
    }

    if (own->port != 0)
    {
        Char(':').Draw(x - 13, y, Color::FILL);
        DrawValueWithSelectedPosition(x + 14, y, *own->port, 5, numIP == 4 ? selPos : -1, false, true);
    }
}

void MACaddress::DrawValue(int x, int y) const
{
    if (gCurDigit > 5)
    {
        gCurDigit = 0;
    }
    uint8 *bytes = OwnData()->mac0;
    x += Item::OPENED_WIDTH - 14;
    y++;
    for (int num = 5; num >= 0; num--)
    {
        int value = (int)(*(bytes + num));
        if (gCurDigit == num)
        {
            Region(10, 8).Fill(x - 1, y, Color::FILL);
        }
        Text("%02X", value).Draw(x, y, gCurDigit == num ? Color::BACK : Color::FILL);
        x -= 12;
    }
}

void Governor::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void IPaddress::Draw(int x, int y, bool opened) const
{
    if (opened)
    {
        DrawOpened(x - (OwnData()->port == 0 ? 0 : IPaddress::OPENED_WIDTH), y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void MACaddress::Draw(int x, int y, bool opened) const
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void Formula::Draw(int x, int y, bool opened) const
{
    if (opened)
    {
        
    }
    else
    {
        DrawClosed(x, y);
    }
}

void GovernorColor::DrawValue(int x, int y, int delta) const
{
    const DataGovernorColor *own = OwnData();

    ColorType *ct = own->colorType;
    int8 field = ct->currentField;
    char *texts[4] = {"��", "��", "��", "��"};
    uint color = COLOR(ct->color);
    int red = (int)R_FROM_COLOR(color);
    int green = (int)G_FROM_COLOR(color);
    int blue = (int)B_FROM_COLOR(color);
    if(!ct->alreadyUsed)
    {
        ct->Init();
    }

    int vals[4] = { (int)(ct->brightness * 100), blue, green, red };

    Region(Item::WIDTH + delta - 2, Item::HEIGHT / 2 - 3).Fill(x, y, Color::BLACK);
    x += 92;
    
    for(int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(30, 11).Fill(x - 1, y + 1, colorBack);
        Text(texts[i]).Draw(x, y + 2, colorDraw);
        Int(vals[i]).ToText().Draw(x + 14, y + 2);
        x -= 30;
    }
    
}

void GovernorColor::DrawOpened(int x, int y) const
{
    static const int delta = 44;
    x -= delta;
    OwnData()->colorType->Init();
    Rectangle(Item::WIDTH + delta, Item::HEIGHT).Draw(x, y, Color::MENU_TITLE);
    DrawVolumeButton(x + 1, y + 1, Item::WIDTH_VALUE + 2 + delta, Item::HEIGHT_VALUE + 3, 2, Color::MENU_ITEM,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, IsPressed(), IsActive());
    HLine().Draw(y + Item::HEIGHT / 2 + 2, x, x + Item::WIDTH + delta - 1, Color::MENU_TITLE);
    Text(Title()).DrawInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), Item::WIDTH + delta,
        Item::HEIGHT / 2 + 2, Color::WHITE);
    DrawValue(x + 1, y + 19, delta);
}

void GovernorColor::DrawClosed(int x, int y) const
{
    OwnData()->colorType->Init();
    GraphicGovernor::Draw(this, x, y);
    Region(Item::WIDTH_VALUE + 3, Item::HEIGHT_VALUE + 2).Fill(x + 1, y + 19, OwnData()->colorType->color);
}

void GovernorColor::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void Choice::DrawOpened(int x, int y) const
{
    int height = HeightOpened();

    GraphicGovernor::Draw(this, x - 1, y - 1);
    Rectangle(Item::TITLE_WIDTH + 1, height + 1).Draw(x - 1, y, Color::MENU_TITLE);
 
    HLine().Draw(y + Item::OPENED_HEIGHT_TITLE - 1, x, x + Item::OPENED_WIDTH);

    DrawVolumeButton(x, y + Item::OPENED_HEIGHT_TITLE, Item::OPENED_WIDTH - 1, height - Item::OPENED_HEIGHT_TITLE, 1,
        Color::BLACK, Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, false, true);

    int index = *((int8*)OwnData()->cell);

    for(int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + Item::OPENED_HEIGHT_TITLE + i * Item::OPENED_HEIGHT + 1;
        bool pressed = (i == index);

        if(pressed)
        {
            DrawVolumeButton(x + 1, yItem, Item::OPENED_WIDTH - 2 , Item::OPENED_HEIGHT - 2, 2, Color::MENU_FIELD,
                Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, pressed, true);
        }

        Text(NameSubItem(i)).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : Color::MENU_FIELD);
    }
}

void TimeItem::DrawOpened(int x, int y) const
{
    int width = Item::WIDTH_VALUE + 3;
    int height = 61;

    Rectangle(width + 2, height + 3).Draw(x - 1, y - 1, Color::BACK);

    GraphicGovernor::Draw(this, x - 1, y - 1);

    Rectangle(width + 1, height + 1).Draw(x - 1, y, Color::MENU_TITLE);

    HLine().Draw(y + Item::OPENED_HEIGHT_TITLE - 1, x, x + Item::OPENED_WIDTH);

    DrawVolumeButton(x, y + Item::OPENED_HEIGHT_TITLE, Item::OPENED_WIDTH - 1, height - Item::OPENED_HEIGHT_TITLE, 1,
        Color::BLACK, Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, false, false);

    int y0 = 21;
    int y1 = 31;
    int y2 = 41;
    int y3 = 51;

    int x0 = 41;
    int dX = 13;
    int wS = 11;

    struct StructPaint
    {
        int x;
        int y;
        int width;
    } strPaint[8] =
    {
        {3,             y3, 60},    // �� ���������
        {x0,            y0, wS},    // ����
        {x0 + dX,       y0, wS},    // �����
        {x0 + 2 * dX,   y0, wS},    // ���
        {x0,            y1, wS},    // ����
        {x0 + dX,       y1, wS},    // ���
        {x0 + 2 * dX,   y1, wS},    // ���
        {3,             y2, 47}     // ���������
    };

    const DataTime *own = OwnData();

    char strI[8][20];
    std::strcpy(strI[iEXIT], LANG_RU ? "�� ���������" : "Do not save");
    std::strcpy(strI[iDAY],   Int(*own->day).ToText(false, 2).c_str());
    std::strcpy(strI[iMONTH], Int(*own->month).ToText(false, 2).c_str());
    std::strcpy(strI[iYEAR],  Int(*own->year).ToText(false, 2).c_str());
    std::strcpy(strI[iHOURS], Int(*own->hours).ToText(false, 2).c_str());
    std::strcpy(strI[iMIN],   Int(*own->minutes).ToText(false, 2).c_str());
    std::strcpy(strI[iSEC],   Int(*own->seconds).ToText(false, 2).c_str());
    std::strcpy(strI[iSET], LANG_RU ? "���������" : "Save");

    Text("� � � - ").Draw(x + 3, y + y0, Color::FILL);
    Text("� � � - ").Draw(x + 3, y + y1);

    for (int i = 0; i < 8; i++)
    {
        if (*own->curField == i)
        {
            Region(strPaint[i].width, 9).Fill(x + strPaint[i].x - 1, y + strPaint[i].y, Color::BLACK_WHITE_1);
        }

        Text(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, (*own->curField == i) ? Color::BLACK_WHITE_0 : Color::FILL);
    }
}

static void GovernorIpCommon_DrawOpened(const Item *item, int x, int y, int dWidth)
{
    int height = 34;
    Rectangle(Item::TITLE_WIDTH + 1 + dWidth, height + 1).Draw(x - 1, y, Color::MENU_TITLE);
    HLine().Draw(y + Item::OPENED_HEIGHT_TITLE - 1, x, x + Item::OPENED_WIDTH + dWidth);
    GraphicGovernor::Draw(item, x - 1, y - 1);
    item->DrawVolumeButton(x, y + Item::OPENED_HEIGHT_TITLE, Item::OPENED_WIDTH - 1 + dWidth,
        height - Item::OPENED_HEIGHT_TITLE, 1, Color::BLACK, Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, false, true);
}

void Governor::DrawOpened(int x, int y) const
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

void IPaddress::DrawOpened(int x, int y) const
{
    GovernorIpCommon_DrawOpened(this, x, y, (OwnData()->port == 0) ? 0 : IPaddress::OPENED_WIDTH);
    DrawValue(x, y + 22);
}

void MACaddress::DrawOpened(int x, int y) const
{
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

void Choice::DrawClosed(int x, int y) const
{
    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 3, Item::HEIGHT_VALUE + 4, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    float deltaY = Step();
    Color colorText = Color::BACK;
    colorText.SetAsCurrent();

    if(deltaY == 0.0F) //-V550
    {
        Text(NameCurrentSubItem()).Draw(x + 4, y + 21);
    }
    else
    {
        Text(NameCurrentSubItem()).DrawWithLimitation(x + 4, (int)(y + 21 - deltaY), colorText, x, y + 19,
            Item::WIDTH_VALUE, Item::HEIGHT_VALUE - 1);

        HLine().Draw((int)(y + (deltaY > 0 ? 31 : 19) - deltaY), x + 3, x + Item::WIDTH_VALUE + 1, Color::BLACK);

        Text(deltaY > 0 ? NameNextSubItem() : NamePrevSubItem()).DrawWithLimitation(x + 4,
            (int)(y + (deltaY > 0 ? 33 : 9) - deltaY), colorText, x, y + 19, Item::WIDTH_VALUE, Item::HEIGHT_VALUE - 1);
    }

    HLine().Draw(y + Item::HEIGHT + 1, x, x + Item::WIDTH, Color::BorderMenu());

    GraphicGovernor::Draw(this, x, y);

    FuncForDraw(x, y);
}

void Choice::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

void TimeItem::DrawClosed(int x, int y) const
{
    GraphicGovernor::Draw(this, x, y);

    DrawVolumeButton(x + 1, y + 17, Item::WIDTH_VALUE + 2, Item::HEIGHT_VALUE + 3, 2, Color::MENU_FIELD,
        Color::MENU_ITEM_BRIGHT, Color::MENU_ITEM_DARK, true, true);

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = HAL_RTC::GetPackedTime();
    Int((int)time.hours).ToText(false, 2).Draw(x + startX, y, Color::BACK);
    Text(":").Draw(x + startX + deltaField, y);
    Int((int)time.minutes).ToText(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    Text(":").Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Int((int)time.seconds).ToText(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);

    startX = 44;
    Int((int)time.day).ToText(false, 2).Draw(x + startX, y);
    Text(":").Draw(x + startX + deltaField, y);
    Int((int)time.month).ToText(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    Text(":").Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Int((int)time.year).ToText(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);
}


void TimeItem::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}


void Button::Draw(int x, int y, bool) const
{
    bool isActive = IsActive();

    HLine().Draw(y + 1, x, x + Item::WIDTH, Color::MENU_TITLE);
    Region(Item::WIDTH - 1, Item::HEIGHT - 1).Fill(x + 1, y + 2, Color::MENU_ITEM);

    DrawVolumeButton(x + 4, y + 5, Item::WIDTH - 8, Item::HEIGHT - 8, 3, Color::MENU_ITEM, Color::MENU_ITEM_BRIGHT,
        Color::MENU_ITEM_DARK, IsPressed(), isActive);

    Text(Title()).DrawInCenterRect(x + 2, y + 2, Item::WIDTH, Item::HEIGHT, isActive ? Color::WHITE : Color::BLACK);
}


void SmallButton::Draw(int x, int y, bool) const
{
    if (IsActive())
    {
        if (IsPressed())
        {
            Region(SmallButton::WIDTH, SmallButton::WIDTH).Fill(x, y, Color::FILL);
            Color::BACK.SetAsCurrent();
        }
        else
        {
            Rectangle(SmallButton::WIDTH, SmallButton::WIDTH).Draw(x, y, Color::FILL);
            Color::FILL.SetAsCurrent();
        }
        OwnData()->funcOnDraw(x, y);
    }
    else
    {
        Rectangle(SmallButton::WIDTH, SmallButton::WIDTH).Draw(x, y, Color::FILL);
    }
}


void Page::Draw(int x, int y, bool) const
{
    bool isPressed = IsPressed();

    HLine().Draw(y + 1, x, x + Item::WIDTH, Color::BorderMenu());

    DrawVolumeButton(x + 1, y + 2, Item::WIDTH - 1, Item::HEIGHT - 1, 2, Color::MENU_ITEM, Color::MENU_ITEM_BRIGHT,
        Color::MENU_ITEM_DARK, isPressed, IsActive());

    Color colorText = Color::BLACK;
    int delta = 0;

    if(isPressed)
    {
        colorText = Color::FILL;
        delta = 1;
    }

    Text(Title()).DrawInCenterRect(x + delta, y + delta, Item::WIDTH, Item::HEIGHT, colorText);
}


void Page::DrawOpened(int yTop)
{
    if (CurrentItem()->IsOpened())
    {
        int8 posCurItem = CurrentItem()->GetPosition();

        Menu::itemsForButtons[posCurItem % 5] = GetItem(posCurItem);

        const Item *item = GetItem(posCurItem);

        item->Draw(X() + 1, item->OpenedPosY(), true);
    }
    else
    {
        DrawTitle(yTop);
        DrawItems(yTop + Item::TITLE_HEIGHT);
    }

    if (OwnData()->funcOnDraw)
    {
        OwnData()->funcOnDraw();
    }
}


// ����� �� �������� ������ ���������
static bool NeedDrawRegSet(const Page *page)
{
    return page->NumSubPages() > 1 &&
        !Menu::CurrentItem()->IsChoiceReg() &&
        !Menu::CurrentItem()->IsGovernor() &&
        Menu::CurrentItem()->IsPage();

}


void Page::DrawTitle(int ytop)
{
    int x = X();

    if (IsPageSB())
    {
        SmallButonFrom(0)->Draw(SmallButton::LEFT, ytop + 3);

        return;
    }

    int height = HeightOpened();
    bool shade = CurrentItem()->IsOpened();

    Rectangle(Item::TITLE_WIDTH + 2, height + 2).Draw(x, ytop, Color::BorderMenu());
    
    if (shade)
    {
        Region(Item::TITLE_WIDTH - 1, Item::TITLE_HEIGHT - 1).Fill(x + 1, ytop + 1, Color::MENU_TITLE_DARK);
        Region(Item::TITLE_WIDTH - 5, Item::TITLE_HEIGHT - 7).Fill(x + 4, ytop + 4, Color::MENU_TITLE_DARK);
    }
    else
    {
        DrawVolumeButton(x + 1, ytop + 1, Item::TITLE_WIDTH, Item::TITLE_HEIGHT, 3, Color::MENU_TITLE,
            Color::MENU_TITLE_BRIGHT, Color::MENU_TITLE_DARK, false, true);
    }

    int delta = NeedDrawRegSet(this) ? -10 : 0;

    Color colorText = shade ? Color::LightShadingText() : Color::BLACK;
    x = Text(Title()).DrawInCenterRect(x, ytop, Item::TITLE_WIDTH + 2 + delta, Item::TITLE_HEIGHT, colorText);

    if (NeedDrawRegSet(this))
    {
        Holder::Draw(x + 4, ytop + 11, colorText);
    }
    
    delta = 0;
    
    colorText.SetAsCurrent();

    DrawPagesUGO(X() + Item::TITLE_WIDTH - 3 + delta, ytop + Item::TITLE_HEIGHT - 2 + delta); //-V2007
}


void Page::DrawItems(int yTop) const
{
    int posFirstItem = PosItemOnTop();
    
    int posLastItem = Math::Limitation(posFirstItem + Page::ITEMS_ON_SUBPAGE - 1, 0, NumItems() - 1);
    
    int count = 0;

    for (int posItem = posFirstItem; posItem <= posLastItem; posItem++)
    {
        const Item *item = GetItem(posItem);

        Menu::itemsForButtons[count] = (Item *)item;

        int top = yTop + Item::HEIGHT * count;

        int x = X();

        item->Draw(x, top);

        count++;
    }
}


Item *Menu::ItemUnderKey(Key::E key)
{
    return Key::IsFunctional(key) ? itemsForButtons[key - 1] : &Item::empty;
}


void Page::DrawPagesUGO(int right, int bottom) const
{
    int size = 5;
    int delta = 2;

    int allPages = (NumItems() - 1) / Page::ITEMS_ON_SUBPAGE + 1;
    int currentPage = GetCurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for (int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if (p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}


void SmallButton::DrawHints(int x, int y, int width)
{
    Region(width, 239 - y).Fill(x, y, Color::BACK);
    Rectangle(width, 239 - y).Draw(x, y, Color::FILL);
    const StructHelpSmallButton *structHelp = &(*OwnData()->hintUGO)[0];
    x += 3;
    y += 3;
    while (structHelp->funcDrawUGO)
    {
        Rectangle(SmallButton::WIDTH, SmallButton::WIDTH).Draw(x, y);
        structHelp->funcDrawUGO(x, y);
        int yNew = Text(structHelp->helpUGO[LANG]).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);
        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}


void Item::DrawVolumeButton(int x, int y, int width, int height, int thickness, Color normal, Color bright,
    Color dark, bool isPressed, bool isActive) const
{
    Region rect(width - thickness * 2 + 1, height - thickness * 2 + 1);

    int x0 = x + thickness;
    int y0 = y + thickness;

    rect.Fill(x0, y0, normal);

    if (isPressed)
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, dark);
            VLine().Draw(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, bright);
            HLine().Draw(y + height - i, x + 1 + i, x + width - i);
        }
    }
    else
    {
        for (int i = 0; i < thickness; i++)
        {
            HLine().Draw(y + i, x + i, x + width - i, bright);
            VLine().Draw(x + i, y + 1 + i, y + height - i);
            VLine().Draw(x + width - i, y + 1 + i, y + height - i, dark);
            HLine().Draw(y + height - i, x + 1 + i, x + width - i);
        }
    }

    if (!isActive)
    {
        int delta = 0;

        Point point;

        dark.SetAsCurrent();

        for (int j = y0; j < y0 + rect.Height(); j++)
        {
            delta = (delta + 1) % 2;

            for (int i = x0 + delta; i < x0 + rect.Width(); i += 2)
            {
                point.Draw(i, j);
            }
        }
    }
}


int Page::X() const
{
    if (IsPageSB())
    {
        return Display::WIDTH - SmallButton::WIDTH - 1;
    }

    return Grid::Right() - Item::TITLE_WIDTH - 2;
}


void Holder::RotateGovernor(int delta)
{
    Rotate(delta);

    Sound::GovernorChangedValue();
}


void Holder::RotateSwitch(int delta)
{
    Rotate(delta);

    Sound::RegulatorSwitchRotate();
}


void Holder::Rotate(int delta)
{
    if (delta > 0)
    {
        Math::CircleIncrease<int>(&angle, 0, 3);
    }
    else
    {
        Math::CircleDecrease<int>(&angle, 0, 3);
    }
}


void Holder::Draw(int x, int y, const Color &color)
{
    Char(chars[angle]).Draw4SymbolsInRect(x, y, color);
}
