// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once


/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsCursors
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CURS_CNTRL_U(ch)            (set.cursors.cntrlU[ch])                    ///< SettingsCursors.cntrlU
#define CURS_CNTRL_U_IS_DISABLE(ch) (CURS_CNTRL_U(ch) == CursCntrl_Disable)

#define CURS_CNTRL_T(ch)            (set.cursors.cntrlT[ch])                    ///< SettingsCursors.cntrlT
#define CURS_CNTRL_T_IS_DISABLE(ch) (CURS_CNTRL_T(ch) == CursCntrl_Disable)

#define CURS_SOURCE                 (set.cursors.source)                        ///< SettingsCursors.source
#define CURS_SOURCE_IS_A            (CURS_SOURCE == A)

#define CURS_POS_U(ch, num)         (set.cursors.posCurU[ch][num])              ///< SettingsCursors.posCurU
#define CURS_POS_U0(ch)             (CURS_POS_U(ch, 0))
#define CURS_POS_U1(ch)             (CURS_POS_U(ch, 1))

#define CURS_POS_T(ch, num)         (set.cursors.posCurT[ch][num])              ///< SettingsCursors.posCurT
#define CURS_POS_T0(ch)             (CURS_POS_T(ch, 0))
#define CURS_POS_T1(ch)             (CURS_POS_T(ch, 1))

#define DELTA_U100(ch)              (set.cursors.deltaU100percents[ch])

#define DELTA_T100(ch)              (set.cursors.deltaT100percents[ch])

#define CURS_MOVEMENT               (set.cursors.movement)                      ///< SettingsCursors.movement
#define CURS_MOVEMENT_IS_PERCENTS   (CURS_MOVEMENT == CursMovement_Percents)

#define CURS_ACTIVE                 (set.cursors.active)                        ///< SettingsCursors.active
#define CURS_ACTIVE_IS_T            (CURS_ACTIVE == CursActive_T)
#define CURS_ACTIVE_IS_U            (CURS_ACTIVE == CursActive_U)

#define CURS_LOOKMODE(num)          (set.cursors.lookMode[num])                 ///< SettingsCursors.lookMode
#define CURS_LOOKMODE_0             (CURS_LOOKMODE(0))
#define CURS_LOOKMODE_1             (CURS_LOOKMODE(1))

#define CURS_SHOWFREQ               (set.cursors.showFreq)                      ///< SettingsCursors.showFreq

#define CURS_SHOW                   (set.cursors.showCursors)                   ///< SettingsCursors.showCursors


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ����� �������� ���������.
typedef enum
{
    CursCntrl_1,            ///< ������.
    CursCntrl_2,            ///< ������.
    CursCntrl_1_2,          ///< ������.
    CursCntrl_Disable       ///< �������.
} CursCntrl;


/// ������������ ����������� ��������.
typedef enum
{
    CursMovement_Points,    ///< �� ������.
    CursMovement_Percents   ///< �� ���������.
} CursMovement;

/// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
typedef enum
{
    CursActive_U,
    CursActive_T,
    CursActive_None
} CursActive;

/// ����� �������� ��������.
typedef enum
{
    CursLookMode_None,      ///< ������� �� ������.
    CursLookMode_Voltage,   ///< ������� ������ �� ����������� �������������.
    CursLookMode_Time,      ///< ������� ������ �� �������� �������������.
    CursLookMode_Both       ///< ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
} CursLookMode;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ��������� ��������� ���������.
typedef struct
{
    CursCntrl       cntrlU[NumChannels];        ///< �������� ������� ����������.
    CursCntrl       cntrlT[NumChannels];        ///< �������� ������� ����������.
    Channel         source;                     ///< �������� - � ������ ������ ��������� �������.
    float           posCurU[NumChannels][2];    ///< ������� ������� �������� ���������� ����� �������.
    float           posCurT[NumChannels][2];    ///< ������� ������� �������� ������� ����� �������.
    float           deltaU100percents[2];       ///< ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float           deltaT100percents[2];       ///< ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    CursMovement    movement;                   ///< ��� ������������ �������� - �� ������ ��� �� ���������.
    CursActive      active;                     ///< ����� ������� ������ �������.
    CursLookMode    lookMode[2];                ///< ������ �������� �� ��������� ��� ���� ��� ��������.
    bool            showFreq;                   ///< ������������� � true ��������, ��� ����� ���������� �� ������ �������� 1/dT ����� ���������.
    bool            showCursors;                ///< ���������� �� �������.
} SettingsCursors;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �������� ������� ������� ����������.
float sCursors_GetCursPosU(Channel chan, int numCur);
/// ���������� true,���� ����� �������� �������.
bool sCursors_NecessaryDrawCursors(void);
/// �������� ������ ������� ����������.
const char* sCursors_GetCursVoltage(Channel source, int numCur, char buffer[20]);
/// �������� ������ ������� �������.
const char* sCursors_GetCursorTime(Channel source, int numCur, char buffer[20]);
/// �������� ������ ��������� �������� ����������.
const char* sCursors_GetCursorPercentsU(Channel source, char buffer[20]);
/// �������� ������ ��������� �������� �������.
const char* sCursors_GetCursorPercentsT(Channel source, char buffer[20]);


/** @}  @}
 */
