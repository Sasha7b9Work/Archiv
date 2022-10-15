#include "Menu/MenuControls.h"
#include "Settings/Settings.h"


//------------------------------------------------------------------------------------------------------------------------------- ������� - ������� --
static const char *itemsModeView[] = { "������", "Vector",
                                       "�����",  "Points" };

const Choice cModeView("�������", "View mode", itemsModeView, &VIEW_MODE, 2);

//------------------------------------------------------------------------------------------------------------------------------- ������� - ������� --
static const char *itemsThickness[] = { "x1", "x1",
                                        "x3", "x3" };

const Choice cThickness("�������", "Thickness", itemsThickness, &THICKNESS_SIGNAL, 2);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ������� ///
const Control *controlsDisplay[] = {&cModeView, &cThickness};

Page pageDisplay("�������", "DISPLAY", controlsDisplay, 2);
