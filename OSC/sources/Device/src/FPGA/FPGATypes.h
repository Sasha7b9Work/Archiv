#pragma once
#include "RShift.h"
#include "TShift.h"
#include "TrigLev.h"


#define ADDR_ALTERA             ((uint8*)NOR_MEMORY_ADRESS1)


// ������ �������
#define WR_START                (ADDR_ALTERA + 0)           ///< �����
#define WR_TBASE                (ADDR_ALTERA + 1)           ///< ��������
#define WR_PRED_LO              (ADDR_ALTERA + 2)           ///< ����������
#define WR_PRED_HI              (ADDR_ALTERA + 3)
#define WR_POST_LO              (ADDR_ALTERA + 4)           ///< �����������
#define WR_POST_HI              (ADDR_ALTERA + 5)

#define WR_UPR                  (ADDR_ALTERA + 6)           ///< ����������
#define BIT_UPR_RAND            0                           ///< ���. ������������
#define BIT_UPR_PEAK            1                           ///< ������� ��������
#define BIT_UPR_CALIBR          2                           ///< ����������

#define WR_FREQMETER            (ADDR_ALTERA + 8)           ///< ���������� ������������
#define WR_TRIG                 (ADDR_ALTERA + 9)           ///< ���������� ��������������/��������
#define WR_START_ADDR           (ADDR_ALTERA + 11)          ///< \brief ����� ������. ����� ����������� �� ����� ������, ����� ������ ������ � 
                                                            ///  ������������� ������ (������� �� ���������� � ����������)
#define WR_RESET_COUNTER_FREQ   (ADDR_ALTERA + 12)          ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������
#define WR_RESET_COUNTER_PERIOD (ADDR_ALTERA + 13)          ///< ���������� ���� ����� ��������� ��������� ����������� ��� ��������� �������� �������

#define RD_DATA_A               (ADDR_ALTERA + 0)           ///< ������ ������� ������
#define RD_DATA_A_PEAK_MAX      (RD_DATA_A)                 ///< ������ �������� ��������� ������� ������
#define RD_DATA_A_PEAK_MIN      (RD_DATA_A_PEAK_MAX + 1)
#define RD_DATA_B               (ADDR_ALTERA + 2)           ///< ������ ������� ������
#define RD_DATA_B_PEAK_MAX      (RD_DATA_B)                 ///< ������ ������� ��������� ������� ������
#define RD_DATA_B_PEAK_MIN      (RD_DATA_B_PEAK_MAX + 1)
#define RD_LAST_RECORD_LO       (ADDR_ALTERA + 4)           ///< ����� ��������� ������
#define RD_LAST_RECORD_HI       (ADDR_ALTERA + 5)
#define RD_FREQ_BYTE_0          (ADDR_ALTERA + 8)           ///< ���������� �������� �������
#define RD_FREQ_BYTE_1          (ADDR_ALTERA + 9)
#define RD_FREQ_BYTE_2          (ADDR_ALTERA + 12)
#define RD_FREQ_BYTE_3          (ADDR_ALTERA + 13)
#define RD_PERIOD_BYTE_0        (ADDR_ALTERA + 16)          ///< ���������� �������� �������
#define RD_PERIOD_BYTE_1        (ADDR_ALTERA + 17)
#define RD_PERIOD_BYTE_2        (ADDR_ALTERA + 20)
#define RD_PERIOD_BYTE_3        (ADDR_ALTERA + 21)
#define RD_FLAG_LO              (ADDR_ALTERA + 24)          ///< ������� ���� �����
#define RD_FLAG_HI              (ADDR_ALTERA + 25)          ///< ������� ���� �����


#define FPGA_MAX_NUM_POINTS     (16 * 1024)

#define AVE_VALUE               127
#define MIN_VALUE               (AVE_VALUE - 125)
#define MAX_VALUE               (AVE_VALUE + 125)

/// ���� �������� == 0, ������, ��� ���. ��� ����� ��� ������� ������������� � ����������� ������ p2p, � ����� ��� tShift ����� ���������� �������
#define NONE_VALUE  0
