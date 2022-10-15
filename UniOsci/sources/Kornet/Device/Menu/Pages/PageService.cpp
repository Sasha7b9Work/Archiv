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

static const Button bResetSettings("����� ��������", "Reset settings", OnPress_ResetSettings);

//----------------------------------------------------------------------------------------------------------------------------------- ������ - ���� --
static const char *itemsLanguage[] = { "�������",    "Russian",
                                       "����������", "English" };

static const Choice cLanguage("����", "Language", itemsLanguage, &LANG, 2);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������ //
static const Control *controlsService[] = {&bResetSettings, &cLanguage};

Page pageService("������", "SERVICE", controlsService, 2);
