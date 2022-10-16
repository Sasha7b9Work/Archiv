#pragma once
#include "ControlPanel.h"

void Menu_Init(void);
void Menu_PressButton(enum Button _button);
void Menu_ReleaseButton(enum Button _button);
void Menu_RotateRegulatorRight(enum Regulator _reg);
void Menu_RotateRegulatorLeft(enum Regulator _reg);
