#pragma once


/** @addtogroup Settings
 *  @{
 *  @addtogroup SettingsTrig
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE              (set.trig.startMode)                    ///< SettingsTrig.startMode
#define START_MODE_IS_SINGLE    (START_MODE == StartMode_Single)        ///< \c true, ���� ����������� ����� �������.
#define START_MODE_IS_AUTO      (START_MODE == StartMode_Auto)          ///< \c true, ���� �������������� ����� �������.

#define TRIGSOURCE              (set.trig.source)                       ///< SettingsTrig.source
#define TRIGSOURCE_IS_EXT       (TRIGSOURCE == TrigSource_Ext)          ///< \c true, ���� ������� �������� �������������.

#define TRIG_POLARITY           (set.trig.polarity)                     ///< SettingsTrig.polarity
#define TRIG_POLARITY_IS_FRONT  (TRIG_POLARITY == TrigPolarity_Front)   ///< \c true, ���� ������������� �� ������.

#define TRIG_INPUT              (set.trig.input)                        ///< SettingsTrig.input
#define TRIG_INPUT_IS_AC        (TRIG_INPUT == TrigInput_AC)            ///< \c true, ���� �������� ���� �������������.

#define SET_TRIGLEV(source)     (set.trig.levelRel[source])             ///< SettingsTrig.levelRel
#define SET_TRIGLEV_SOURCE      (SET_TRIGLEV(TRIGSOURCE))               /// set.trig.levelRel[set.trig.source]
#define SET_TRIGLEV_A           (SET_TRIGLEV(A))                        ///< set.trig.levelRel[A]
#define SET_TRIGLEV_B           (SET_TRIGLEV(B))                        ///< set.trig.levelRel[B]

#define MODE_LONG_PRESS_TRIG    (set.trig.modeLongPressTrig)            ///< SettingsTrig.modeLongPressTrig
/// ���� \c true, �� ���������� ������� ������ \b ����� ���������� ������� ������������� � 0.
#define MODE_LONG_PRESS_TRIG_IS_LEVEL0  (MODE_LONG_PRESS_TRIG == ModeLongPRessTrig_Level0)

#define TRIG_MODE_FIND          (set.trig.modeFind)                     ///< SettingsTrig.modeFind
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == TrigModeFind_Auto)   ///< \c true, ���� �������������� ��������� �������������.
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == TrigModeFind_Hand)   ///< \c true, ���� ������������� ��������������� �������.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ����� �������.
typedef enum
{
    StartMode_Auto,             ///< ��������������.
    StartMode_Wait,             ///< ������.
    StartMode_Single            ///< �����������.
} StartMode;

/// �������� �������������.
typedef enum
{
    TrigSource_A,               ///< ����� 1.
    TrigSource_B,               ///< ����� 2.
    TrigSource_Ext              ///< �������.
} TrigSource;

/// ��� ������������
typedef enum
{
    TrigPolarity_Front,         ///< ������������� �� ������.
    TrigPolarity_Back           ///< ������������� �� �����.
} TrigPolarity;

/// ���� �������������.
typedef enum
{
    TrigInput_Full,             ///< ������ ������.
    TrigInput_AC,               ///< ����������.
    TrigInput_LPF,              ///< ���.
    TrigInput_HPF               ///< ���.
} TrigInput;

/// ����� ����������� ������� ������ �����.
typedef enum
{
    ModeLongPRessTrig_Level0,   ///< ����� ������ ������������� � 0.
    ModeLongPressTrig_Auto      ///< �������������� ����� ������������� - ������� ��������������� ���������� ����� ���������� � ���������.
} ModeLongPressTrig;


/// ����� ��������� �������������.
typedef enum
{
    TrigModeFind_Hand,          ///< ������� ������������� ��������������� �������.
    TrigModeFind_Auto           ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
} TrigModeFind;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ��������� �������������
typedef struct
{
    StartMode           startMode;          ///< ����� �������.
    TrigSource          source;             ///< ��������.
    TrigPolarity        polarity;           ///< ��� �������������.
    TrigInput           input;              ///< ���� �������������.
    int16               levelRel[3];        ///< ������� ������������� ��� ��� ����������.
    ModeLongPressTrig   modeLongPressTrig;  ///< ����� ������ ����������� ������� ������ �����.
    TrigModeFind        modeFind;           ///< ����� ������������� - ������� ��� �������������.
} SettingsTrig;


/** @}  @}
 */
