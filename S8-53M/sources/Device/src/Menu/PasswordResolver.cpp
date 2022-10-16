// 2021/07/22 14:51:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Panel/Controls_.h"
#include "Menu/Menu.h"
#include "Menu/PasswordResolver.h"
#include "Menu/Pages/Definition.h"
#include "Settings/Settings.h"


void PasswordResolver::ProcessEvent(const KeyboardEvent& event)
{
    static const int NUM_KEYS = 10;

    static const Key::E keys[NUM_KEYS] =
    {
        Key::F1, Key::F1,
        Key::F2, Key::F2,
        Key::F3, Key::F3,
        Key::F4, Key::F4,
        Key::F5, Key::F5
    };

    static int position = 0;		// Этот элемент массива будем сравнивать

    if (MENU_IS_SHOWN)
    {
        return;
    }

    if (event.action != Action::Down)
    {
        return;
    }

    if (event.key == keys[position])
    {
        position++;

        if (position == NUM_KEYS)
        {
            PageMain::EnablePageDebug();

            position = 0;
        }
    }
    else
    {
        position = 0;
    }
}
