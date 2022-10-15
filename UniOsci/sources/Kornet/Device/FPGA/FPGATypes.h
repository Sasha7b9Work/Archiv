#pragma once


#define ADDR_ALTERA         ((uint8*)NOR_MEMORY_ADRESS1)

#define RShiftMin   20          ///< \brief   Минимальное значение смещения канала по напряжению, засылаемое в аналоговую часть. Соответствует 
                                /// смещению 10 клеток вниз от центральной линии.
#define RShiftZero  500         ///< \brief Среднее значение смещения канала по напряжению, засылаемое в аналоговую часть. Соответствует 
                                /// расположению марера по центру экрана.
#define RShiftMax   980         ///< \brief Максимальное значение смещения канала по напряжению, засылаемое в аналоговую часть. Соответствует 
                                ///смещению 10 клеток вверх от центральной лиини.

/// На столько единиц нужно изменить значение смещения, чтобы маркер смещения по напряжению передвинулся на одну точку.
#define STEP_RSHIFT     (((RShiftMax - RShiftMin) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT

// Адреса альтеры
#define WR_START            (ADDR_ALTERA + 0)           ///< Старт
#define WR_TBASE            (ADDR_ALTERA + 1)           ///< Развёртка
#define WR_PRED_LO          (ADDR_ALTERA + 2)           ///< Предзапуск
#define WR_PRED_HI          (ADDR_ALTERA + 3)
#define WR_POST_LO          (ADDR_ALTERA + 4)           ///< Послезапуск
#define WR_POST_HI          (ADDR_ALTERA + 5)

#define WR_UPR              (ADDR_ALTERA + 6)           ///< Управление
#define BIT_UPR_RAND        0                           ///< Вкл. рандомизатор
#define BIT_UPR_PEAK        1                           ///< Пиковый детектор
#define BIT_UPR_CALIBR      2                           ///< Калибратор

#define WR_FREQMETER        (ADDR_ALTERA + 7)           ///< Управление частотомером
#define WR_TRIG             (ADDR_ALTERA + 9)           ///< Управление синхронизацией/запуском
#define WR_START_ADDR       (ADDR_ALTERA + 11)          ///< \brief Старт адреса. Нужно записываеть по этому адресу, чтобы начать чтение с 
                                                        ///  произвольного адреса (который мы записываем в предзапуск)

#define RD_DATA_A           (ADDR_ALTERA + 0)           ///< Данные первого канала
#define RD_DATA_A_PEAK_MAX  (RD_DATA_A)                 ///< Данные пикового детектора первого канала
#define RD_DATA_A_PEAK_MIN  (RD_DATA_A_PEAK_MAX + 1)
#define RD_DATA_B           (ADDR_ALTERA + 2)           ///< Данные второго канала
#define RD_DATA_B_PEAK_MAX  (RD_DATA_B)                 ///< Данные пиквого детектора второго канала
#define RD_DATA_B_PEAK_MIN  (RD_DATA_B_PEAK_MAX + 1)
#define RD_LAST_RECORD_LO   (ADDR_ALTERA + 4)           ///< Адрес последней записи
#define RD_LAST_RECORD_HI   (ADDR_ALTERA + 5)
#define RD_FREQ_BYTE_0      (ADDR_ALTERA + 8)           ///< Измеренное значение частоты
#define RD_FREQ_BYTE_1      (ADDR_ALTERA + 9)
#define RD_FREQ_BYTE_2      (ADDR_ALTERA + 12)
#define RD_FREQ_BYTE_3      (ADDR_ALTERA + 13)
#define RD_PERIOD_BYTE_0    (ADDR_ALTERA + 16)          ///< Измеренное значение периода
#define RD_PERIOD_BYTE_1    (ADDR_ALTERA + 17)
#define RD_PERIOD_BYTE_2    (ADDR_ALTERA + 20)
#define RD_PERIOD_BYTE_3    (ADDR_ALTERA + 21)
#define RD_FLAG_LO          (ADDR_ALTERA + 24)          ///< Флаг
#define BIT_FLAG_DATA_READY 0                           ///< Данные готовы для считывания
#define BIT_FLAG_PRED       2                           ///< Если 1, то предзапуск отсчитал, можно давать принудительный запуск
#define RD_FLAG_HI          (ADDR_ALTERA + 25)


#define FPGA_MAX_NUM_POINTS (8 * 1024)

#define AVE_VALUE           127
#define MIN_VALUE           (AVE_VALUE - 125)
#define MAX_VALUE           (AVE_VALUE + 125)
