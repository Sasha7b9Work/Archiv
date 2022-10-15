#include "MenuControls.h"
#include "Device.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Keyboard/Buttons.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EmptyControl Page::emptyControl;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Page::Draw(int x, int y) const
{
    if (numControls)
    {
        if (Title())
        {
            painter.DrawBoundedRegion(x, y - 12, Control::Width(), 12, Color::BACK, Color::FILL);

            painter.DrawText(x + 3, y - 10, Title(), Color::FILL);
        }

        for (int control = firstControl; control < numControls + CONTROLS_ON_SUBPAGE; ++control)
        {
            if (control >= numControls)
            {
                break;
            }

            controls[control]->Draw(x, y);

            x += Control::Width();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Control::Width()
{
    return (device.CurrentMode() == Mode_Osci) ? (Grid::WIDTH / CONTROLS_ON_SUBPAGE) : (Display::WIDTH / CONTROLS_ON_SUBPAGE);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::PressKey(Key key) const
{
    if (MENU_IS_SHOWN)
    {
        ControlUnderKey(key)->PressKey(key);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const Control *Page::ControlUnderKey(Key key) const
{
    int numKey = key - K_1;

    int numControl = firstControl + numKey;

    return (numControl < numControls) ? controls[numControl] : &emptyControl;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Control::Title() const
{
    return title_[LANG];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Choice::Draw(int x, int y) const
{
    painter.DrawBoundedRegion(x, y, Control::Width(), Control::HEIGHT, Color::BACK, Color::FILL);

    painter.FillRegion(x + 1, y + 1, Control::Width() - 2, 10, Color::GRID);

    painter.DrawText(x + 3, y + 2, Title(), Color::BACK);

    painter.DrawText(x + 3, y + 13, NameItem(), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Choice::NameItem() const
{
    if(numAlternate == 0)
    {
        return (char *)items0[*(cells[numAlternate]) * 2 + (int)LANG];
    }
    
    return (char *)items1[*(cells[numAlternate]) * 2 + (int)LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::PressKey(Key) const
{
    CircleIncrease<uint8>(cells[numAlternate], 0, max[numAlternate]);
    funcOnChanged();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::SetAlternateMode(int numAlternate_)
{
    numAlternate = numAlternate_;
    funcOnChanged();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::PressKey(Key) const
{
    funcOnPress(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y) const
{
    painter.DrawBoundedRegion(x, y, Control::Width(), Control::HEIGHT, Color::BACK, Color::FILL);

    if (su.NumWords(Title()) == 1)
    {
        painter.DrawText(x + 3, y + 5, Title(), Color::FILL);
    }
    else
    {
        char buffer[20];
        painter.DrawText(x + 3, y + 3, su.GetWord(Title(), 0, buffer, 20), Color::FILL);
        painter.DrawText(x + 3, y + 12, su.GetWord(Title(), 1, buffer, 20), Color::FILL);
    }
}
