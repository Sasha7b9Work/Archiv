#pragma once
#include "Panel/Controls.h"


/** @defgroup Menu
 *  @{
 */


/// ������� ������ ���������� � ������� �����.
void Menu_UpdateInput(void);
/// ������� ��������� ��������� ������� ������ (����� 0.5 ���.).
void Menu_ShortPressureButton(PanelButton button);
/// ������� ��������� �������� ������� ������ (����� 0.5 ���.).
void Menu_LongPressureButton(PanelButton button);
/// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
void Menu_PressButton(PanelButton button);
/// ������� ����������, ����� ������ ��������� �� �������� � ������� ���������.
void Menu_ReleaseButton(PanelButton button);
/// ������� ��������� �������� ����� ��������� ������.
void Menu_RotateRegSetRight(void);
/// ������� ��������� �������� ����� ��������� �����.
void Menu_RotateRegSetLeft(void);
/// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
void Menu_SetAutoHide(bool active);
/// ���������� ����� ������ ����, ������������ ��� ������� � ������ ������ �������.
void* Menu_ItemUnderKey(void);
/// ���������� ���� � �������� ������ ���� � ��������� ����, ������� � ������ �� �����.
char* Menu_StringNavigation(char buffer[100]);

void Menu_OpenItemTime(void);

/** @}
 */
