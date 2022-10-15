#pragma once


#include "../defines.h"
#include "../Settings/SettingsTypes.h"
#include "../Hardware/RTC.h"


void            DS_Clear(void);                                                                     // �������� ���� ���������� ���������
void            DS_AddData(uint8 *data0, uint8 *data1, DataSettings dss);                           // �������� ��������� ������. ��� ���� ����������� ��������� ������� ��������� �������.
int             DS_NumElementsWithSameSettings(void);                                               // ���������� ����� ����������� ���������, ������� � ����������, � ������ �� �����������, ��� � ����������.
int             DS_NumElementsWithCurrentSettings(void);                                            // ���������� ����� ����������� ���������, ������� � ����������, � �������� ����������� �������
int             DS_NumElementsInStorage(void);                                                      // ���������� ���������� ���������� ���������
bool            DS_GetDataFromEnd(int fromEnd, DataSettings **ds, uint8 **data0, uint8 **data1);    // �������� ��������� �� ������
uint8*          DS_GetData(Channel chan, int fromEnd);
uint8*          DS_GetAverageData(Channel chan);                                                    // �������� ���������� ������ �� ���������� ����������.
int             DS_AllDatas(void);                                                                  // ������� ����� ��������� ��������� � ������.
uint8*          DS_GetLimitation(Channel chan, int direction);                                      // �������� �������������� ����� ������� 0 - �����, 1 - ������.
int             DS_NumberAvailableEntries(void);
