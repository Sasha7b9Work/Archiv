#pragma once 


#include "../defines.h"


void    Panel_Init(void);
void    Panel_EnableLEDRegSet(bool enable);
void    Panel_EnableLEDChannel0(bool enable);       // ��������/��������� ��������� ����� 1.
void    Panel_EnableLEDChannel1(bool enable);       // ��������/��������� ��������� ����� 2.
void    Panel_EnableLEDTrig(bool enable);           // ��������/��������� ��������� �����.
void    Panel_TransmitData(uint16 data);            // �������� ������� � �� ������ ����������.
void    Panel_Disable(void);                        // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
void    Panel_Enable(void);
PanelButton   Panel_WaitPressingButton(void);       // ������� ������� �������.
bool    Panel_ProcessingCommandFromPIC(uint16 command);
uint16  Panel_NextData(void);
