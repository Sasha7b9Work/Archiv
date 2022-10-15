#pragma once
#include "Utils/Measures.h"


/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMeasures
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_MEASURES                    (set.measures.number)                           ///< SettingsMeasuers.number
#define NUM_MEASURES_IS_1               (NUM_MEASURES == MN_1)
#define NUM_MEASURES_IS_1_5             (NUM_MEASURES == MN_1_5)
#define NUM_MEASURES_IS_2_5             (NUM_MEASURES == MN_2_5)
#define NUM_MEASURES_IS_3_5             (NUM_MEASURES == MN_3_5)
#define NUM_MEASURES_IS_6_1             (NUM_MEASURES == MN_6_1)
#define NUM_MEASURES_IS_6_2             (NUM_MEASURES == MN_6_2)

#define SOURCE_MEASURE                  (set.measures.source)                           ///< SettingsMeasures.source
#define SOURCE_MEASURE_IS_A             (SOURCE_MEASURE == A)
#define SOURCE_MEASURE_IS_B             (SOURCE_MEASURE == B)
#define SOURCE_MEASURE_IS_A_B           (SOURCE_MEASURE == A_B)

#define MODE_VIEW_SIGNALS               (set.measures.modeViewSignals)                 ///< SettingsMeasures.modeViewSignals
#define MODE_VIEW_SIGNALS_IS_COMPRESS   (MODE_VIEW_SIGNALS == ModeViewSignals_Compress)

#define MEAS_MARKED                     (set.measures.markedMeasure)                    ///< SettingsMeasures.markedMeasure
#define MEAS_MARKED_IS_NONE             (MEAS_MARKED == Meas_None)

#define MEASURE(num)                    (set.measures.measures[num])                    ///< SettingsMeasures.measure
#define MEASURE_IS_MARKED(num)          (MEASURE(num) == MEAS_MARKED)

#define SHOW_MEASURES                   (set.measures.show)                             ///< SettingsMeasures.show

#define MEAS_FIELD                      (set.measures.field)                            ///< SettingsMeaseres.field
#define MEAS_FIELD_IS_HAND              (MEAS_FIELD == MeasuresField_Hand)

#define MEAS_POS_CUR_U(num)             (set.measures.posCurU[num])                     ///< SettingsMeasures.posCurU
#define MEAS_POS_CUR_U0                 (MEAS_POS_CUR_U(0))
#define MEAS_POS_CUR_U1                 (MEAS_POS_CUR_U(1))

#define MEAS_POS_CUR_T(num)             (set.measures.posCurT[num])                     ///< SettingsMeasures.posCurT
#define MEAS_POS_CUR_T0                 (MEAS_POS_CUR_T(0))
#define MEAS_POS_CUR_T1                 (MEAS_POS_CUR_T(1))

#define MEAS_CURS_CNTRL_U               (set.measures.cntrlU)                           ///< SettingsMeasures.cntrlU

#define MEAS_CURS_CNTRL_T               (set.measures.cntrlT)                           ///< SettingsMeasures.cntrlT

#define MEAS_CURS_ACTIVE                (set.measures.cursActive)                       ///< SettingsMeasures.cursActive
#define MEAS_CURS_ACTIVE_IS_T           (MEAS_CURS_ACTIVE == CursActive_T)



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���������� � ������������ �� ������ ����������� ���������.
typedef enum
{
    MN_1,                       ///< 1 ��������� ����� �����.
    MN_2,                       ///< 2 ��������� ����� �����.
    MN_1_5,                     ///< 1 ������ � 5 �����������.
    MN_2_5,                     ///< 2 ������ �� 5 ���������.
    MN_3_5,                     ///< 3 ������ �� 5 ���������.
    MN_6_1,                     ///< 6 ����� �� 1 ���������.
    MN_6_2                      ///< 6 ����� �� 2 ���������.
} MeasuresNumber;

/// ����, �� ������� ��������� ��������
typedef enum
{
    MeasuresField_Screen,       ///< ��������� ����� ������������� �� ��� ����� �������, ������� ����� �� ������.
    MeasuresField_AllMemory,    ///< ��������� ����� ������������� �� ����� �������.
    MeasuresField_Hand          ///< ��������� ����� ������������� �� ����, ����������� �������������.
} MeasuresField;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ��������� ���� ���������
typedef struct
{
    MeasuresNumber  number;             ///< ������� ��������� ��������.
    Channel         source;             ///< ��� ����� ������� �������� ���������.
    ModeViewSignals modeViewSignals;    ///< ������� �� ������� ��� ������ ���������.
    Meas            measures[15];       ///< ��������� ��� ��������� ���������.
    bool            show;               ///< ���������� �� ���������.
    MeasuresField   field;              ///< ����� �������, �� ������� ������� �������� ��� ������� ���������.
    int16           posCurU[2];         ///< ������� ��������, ������� ������ �������, �� ������� ������� �������� ��� ������� ��������� ��� field == MeasuresField_Hand.
    int16           posCurT[2];         ///< ������� ��������, ������� ������ �������, �� ������� ������� �������� ��� ������� ��������� ��� field == MeasuresField_Hand.
    CursCntrl       cntrlU;             ///< �������� ������� ����������.
    CursCntrl       cntrlT;             ///< �������� ������� �������.
    CursActive      cursActive;         ///< ����� ������� ������� - �� ������� ��� ����������.
    Meas            markedMeasure;      ///< ���������, �� ������� ����� �������� �������.
} SettingsMeasures;



/** @}  @}
 */
