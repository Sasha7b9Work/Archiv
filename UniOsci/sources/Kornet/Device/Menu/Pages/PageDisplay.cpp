#include "Menu/MenuControls.h"
#include "Settings/Settings.h"


//------------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Отображ --
static const char *itemsModeView[] = { "Вектор", "Vector",
                                       "Точки",  "Points" };

const Choice cModeView("Отображ", "View mode", itemsModeView, &VIEW_MODE, 2);

//------------------------------------------------------------------------------------------------------------------------------- ДИСПЛЕЙ - Толщина --
static const char *itemsThickness[] = { "x1", "x1",
                                        "x3", "x3" };

const Choice cThickness("Толщина", "Thickness", itemsThickness, &THICKNESS_SIGNAL, 2);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ДИСПЛЕЙ ///
const Control *controlsDisplay[] = {&cModeView, &cThickness};

Page pageDisplay("ДИСПЛЕЙ", "DISPLAY", controlsDisplay, 2);
