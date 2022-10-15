#pragma once
#include "FPGA/FPGATypes.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup DataSettings Data Settings
 *  @{
 */


struct PackedTime
{
    uint timeMS     : 32;   /// \brief ����� � ������������� �� ������ �������. �.�. ��������� ����������� �� ����� ���������� ������ � ���������, �� 
                            /// timeMS == 0 ��������, ��� ������ ������ � ������ ���������� ������ ��� �� ������
    uint hours      : 5;
    uint minutes    : 6;
    uint seconds    : 6;
    uint year       : 7;
    uint month      : 4;
    uint notUsed0   : 4;
    uint day        : 5;
    uint notUsed1   : 27;
};

struct DataSettings
{
    uint8      *addr;                   ///< ����� ������ �� ������� ���
    uint16      rShift[2];
    uint16      trigLev[2];
    int16       tShift;                 ///< �������� �� �������
    Range       range[2];               ///< ������� �� ���������� ����� �������.
    uint        tBase           : 5;    ///< ������� �� �������
    uint        enableA         : 1;    ///< ������� �� ����� A
    uint        enableB         : 1;    ///< ������� �� ����� B
    uint        coupleA         : 2;    ///< ����� ������ �� �����
    uint        coupleB         : 2;
    uint        peackDet        : 2;    ///< ������� �� ������� ��������
    uint        inverseA        : 1;
    uint        inverseB        : 1;
    uint        multiplierA     : 1;
    uint        multiplierB     : 1;
    uint        enumPoints      : 3;
    uint        notUsed         : 12;
    PackedTime  time;
    int BytesInChannel();
    void Fill();
};

#define RSHIFT(ds, ch)          ((ds)->rShift[ch])
#define RSHIFT_A(ds)            (RSHIFT(ds, Chan::A))
#define RSHIFT_B(ds)            (RSHIFT(ds, Chan::B))

#define TRIGLEV(ds, ch)         ((ds)->trigLev[ch])
#define TRIGLEV_A(ds)           (TRIGLEV(ds, Chan::A))
#define TRIGLEV_B(ds)           (TRIGLEV(ds, Chan::B))

#define TSHIFT(ds)              ((ds)->tShift)

#define Lval_RANGE(ds, ch)      ((ds)->range[ch])
#define Lval_RANGE_A(ds)        (Lval_RANGE(ds, Chan::A))
#define Lval_RANGE_B(ds)        (Lval_RANGE(ds, Chan::B))
#define RANGE(ds, ch)           (Lval_RANGE(ds, ch))
#define RANGE_A(ds)             (RANGE(ds, Chan::A))
#define RANGE_B(ds)             (RANGE(ds, Chan::B))

#define Lval_TBASE(ds)          ((ds)->tBase)
#define TBASE(ds)               (Lval_TBASE(ds))

#define Lval_ENABLED_A(ds)      ((ds)->enableA)
#define Lval_ENABLED_B(ds)      ((ds)->enableB)
#define Lval_ENABLED(ds, ch)    (ch.IsA() ? Lval_ENABLED_A(ds) : Lval_ENABLED_B(ds))
#define ENABLED_A(ds)           ((bool)Lval_ENABLED_A(ds))
#define ENABLED_B(ds)           ((bool)Lval_ENABLED_B(ds))
#define ENABLED(ds, ch)         (ch.IsA() ? ENABLED_A(ds) : ENABLED_B(ds))

#define Lval_COUPLE_A(ds)       ((ds)->coupleA)
#define Lval_COUPLE_B(ds)       ((ds)->coupleB)
#define COUPLE_A(ds)            ((ModeCouple)Lval_COUPLE_A(ds))
#define COUPLE_B(ds)            ((ModeCouple)Lval_COUPLE_B(ds))
#define COUPLE(ds, ch)          (ch.IsA() ? COUPLE_A(ds) : COUPLE_B(ds))

#define Lval_PEAKDET(ds)        ((ds)->peackDet)
#define PEAKDET(ds)             ((PeakDetMode)Lval_PEAKDET(ds))

#define INVERSE_A(ds)           ((ds)->inverseA)
#define INVERSE_B(ds)           ((ds)->inverseB)
#define INVERSE(ds, ch)         (ch.IsA() ? INVERSE_A(ds) : INVERSE_B(ds))

#define Lval_DIVIDER_A(ds)      ((ds)->multiplierA)
#define Lval_DIVIDER_B(ds)      ((ds)->multiplierB)

#define DIVIDER_A(ds)           ((Divider)Lval_DIVIDER_A(ds))
#define DIVIDER_B(ds)           ((Divider)Lval_DIVIDER_B(ds))
#define DIVIDER(ds, ch)         (ch.IsA() ? DIVIDER_A(ds) : DIVIDER_B(ds))

#define TIME_TIME(ds)           ((ds)->time)
#define TIME_DAY(ds)            ((ds)->time.day)
#define TIME_HOURS(ds)          ((ds)->time.hours)
#define TIME_MINUTES(ds)        ((ds)->time.minutes)
#define TIME_SECONDS(ds)        ((ds)->time.seconds)
#define TIME_MONTH(ds)          ((ds)->time.month)
#define TIME_YEAR(ds)           ((ds)->time.year)
#define TIME_MS_DS(ds)          ((ds)->time.timeMS)

#define ENUM_POINTS(ds)         ((ds)->enumPoints)
#define ENUM_BYTES(ds)          (ENUM_POINTS(ds) + ((PEAKDET(ds) ? 1 : 0)))
#define NUM_BYTES(ds)           ((ds)->BytesInChannel())


/** @}  @}
 */
