#include "Menu/MenuControls.h"
#include "Settings/Settings.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------
void OnPress_ResetSettings(bool active)
{
    if (active)
    {
        set.Reset();
        set.Save();
    }
}

static const Button bResetSettings("—брос настроек", "Reset settings", OnPress_ResetSettings);

//----------------------------------------------------------------------------------------------------------------------------------- —≈–¬»— - язык --
static const char *itemsLanguage[] = { "–усский",    "Russian",
                                       "јнглийский", "English" };

static const Choice cLanguage("язык", "Language", itemsLanguage, &LANG, 2);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// —≈–¬»— //
static const Control *controlsService[] = {&bResetSettings, &cLanguage};

Page pageService("—≈–¬»—", "SERVICE", controlsService, 2);
