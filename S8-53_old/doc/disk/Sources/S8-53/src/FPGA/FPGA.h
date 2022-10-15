#pragma once
#include "defines.h"
#include "FPGA_Types.h"
#include "Panel/Controls.h"
#include "DataStorage.h"
#include "Settings/SettingsTrig.h"
#include "Settings/SettingsService.h"


void	    FPGA_Init(void);
void	    FPGA_SetNumSignalsInSec(int numSigInSec);   // ���������� ���������� ����������� �������� � �������.
void        FPGA_Update(void);
void        FPGA_WriteToHardware				        // ������ � ������� ���� ������ ��������.
			    			(uint8 *address,            // ����� ��������.
				    		uint8 value,                // ������������ ��������.
					    	bool restart                // true ��������, ��� ����� ������ ����� �������� ����� ���������, ���� �� ����� ������ �� ��������� � ������ �����.
						    );
StateWorkFPGA FPGA_CurrentStateWork(void);
void        FPGA_OnPressStartStop(void);                // ��������� ���� ����� ����������.
void        FPGA_Start(void);							// ������ �������� ����� ����������.
void        FPGA_Stop(bool pause);                      // ��������� ������� ����� ����������.
bool        FPGA_IsRunning(void);                       // ���������� true, ���� ������ ��������� �� � �������� ����� ����������.
void        FPGA_SaveState(void);                       // ��������� ������� ���������. ����� �� ����� ������������ �������� FPGA_RestoreState().
void        FPGA_RestoreState(void);                    // ��������������� ���������, ����� ���������� �������� FPGA_SaveState().
float       FPGA_GetFreq(void);                         // �������� �������� �������.
void        FPGA_ClearData(void);                       // ������� ������. ����� ��� ������ �������������, ��� ���������� ������� ����� �� �������� ���������������.
void        FPGA_SetAdditionShift(int shift);           // ���������� �������������� ��������. ����� ��� ���������� ����������� ����� � ������ �������������.
bool        FPGA_AllPointsRandomizer(void);             // ���������� true,���� ��� ����� �������� � ������ �������������.
void        FPGA_SetNumberMeasuresForGates(int number); // ���������� ���������� ���������, �� ������� ����� �������������� ������ � ������ �������������.
void        FPGA_SwitchingTrig(void);                   // ������������� ��������� �������������.
void        FPGA_StartAutoFind(void);					// ��������� ������� ������ �������.
void        FPGA_TemporaryPause(void);                  // ���������� ��������� ����� ����� ��������� ����� - ����� ��������� ������ ������������� �� ��������� �����
void        FPGA_FillDataPointer(DataSettings *dp);
void        FPGA_FindAndSetTrigLevel(void);             // ����� � ���������� ������� ������������� �� ���������� ���������� �������
    
void        FPGA_LoadSettings(void);                                  // ��������� ��������� � ���������� ����� �� ���������� ��������� SSettings.
void        FPGA_SetModeCouple(Channel chan, ModeCouple modeCoupe);   // ���������� ����� ������ �� �����.
void        FPGA_EnableChannelFiltr(Channel chan, bool enable);       // ��������/��������� ������ �� ����� ������.
void        FPGA_SetRange(Channel chan, Range range);                 // ���������� ������� �� ����������.
bool        FPGA_RangeIncrease(Channel chan);                         // ��������� ������� �� ����������.
bool        FPGA_RangeDecrease(Channel chan);                         // ��������� ������� �� ����������.
void        FPGA_SetTBase(TBase tBase);                               // ���������� ������� �� �������.
void        FPGA_TBaseDecrease(void);                                 // ��������� ������� �� �������.
void        FPGA_TBaseIncrease(void);                                 // ��������� ������� �� �������.
void        FPGA_SetRShift(Channel chan, int16 rShift);               // ���������� ������������� �������� �� ����������.
void        FPGA_SetTShift(int tShift);                               // ���������� ������������� �������� �� �������.
void        FPGA_SetDeltaTShift(int16 shift);                         // ���������� ���������� �������� �� ������� ��� ������ �������������. � ������ �������� ��� �������� ������ ���� ������.
void        FPGA_SetPeackDetMode(PeackDetMode peackDetMode);          // ��������/��������� ����� �������� ���������.
void        FPGA_SetCalibratorMode(CalibratorMode calibratorMode);    // ��������/��������� ����������.
void        FPGA_LoadKoeffCalibration(Channel chan);                  // ��������� � ������� ������������ ���������� �������.

void        FPGA_SetTrigLev(TrigSource chan, int16 trigLev);          // ���������� ������������� ������� �������������.
void        FPGA_SetTrigSource(TrigSource trigSource);                // ���������� �������� �������������.
void        FPGA_SetTrigPolarity(TrigPolarity polarity);              // ���������� ���������� �������������.
void        FPGA_SetTrigInput(TrigInput trigInput);                   // ���������� ����� ����� �������������.

const char* FPGA_GetTShiftString(int16 tShiftRel, char buffer[20]);                    // ���������� ������������� �������� �� ������� � ��������� ����, ��������� ��� ������ �� �����.
