#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Cursors.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������� ��������� ������ ������ �� ������ ������ ����.
void DrawSB_Exit(int x, int y);
/// ������������� ������� ��������� ������� ������ ������ �� ������ ������ ����.
void OnPressSB_Exit();

void DrawMenuCursVoltage(int x, int y, bool top, bool bottom);

void DrawMenuCursTime(int x, int y, bool left, bool right);
/// ������������ ������� ��������� ��� ����� ������ ���������� ������� ���������.
void CalculateConditions(int16 pos0, int16 pos1, Cursors::Control cursCntrl, bool *cond0, bool *cond1);
