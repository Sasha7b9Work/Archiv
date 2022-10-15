#pragma once
#include "defines.h"
#include "SettingsTypes.h"
#include "SettingsMath.h"
#include "SettingsTime.h"
#include "Menu/MenuControls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TESTER_NUM_POINTS               (240)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_COUPLE(ch)                  (set.chan_couple[ch])
#define SET_COUPLE_A                    (SET_COUPLE(A))
#define SET_COUPLE_B                    (SET_COUPLE(B))
#define SET_RSHIFT(ch)                  (set.chan_shift[ch])
#define SET_RANGE(ch)                   (set.chan_range[ch])
#define SET_RANGE_A                     (SET_RANGE(A))
#define SET_RANGE_B                     (SET_RANGE(B))
#define LANG                            (set.serv_lang)
#define LANG_RU                         (LANG == Russian)
#define SET_TRIGLEV                     (set.trig_lev)
#define SET_TSHIFT                      (set.time_shift)
#define SET_TBASE                       (set.time_base)
#define IN_RANDOMIZE_MODE               (SET_TBASE < TBase_100ns)
#define CURRENT_PAGE                    (set.menu_currentPage)
#define SET_ENABLED(ch)                 (set.chan_enable[ch])

#define VIEW_MODE                       (set.disp_modeDrawSignal)
#define VIEW_MODE_IS_LINES              (VIEW_MODE == Mode_Lines)
#define THICKNESS_SIGNAL                (set.disp_thickness)
#define THICKNESS_SIGNAL_IS_3           (THICKNESS_SIGNAL == Thickness_3)

#define TRIG_MODE                       (set.trig_mode)
#define TRIGSOURCE                     (set.trig_source)
#define TRIG_POLARITY                   (set.trig_polarity)
#define TRIG_POLARITY_IS_RISING         (TRIG_POLARITY == Polarity_Rising)
#define TRIG_INPUT                      (set.trig_input)

#define TESTER_CONTROL                  (set.test_control)
#define TESTER_CONTROL_IS_U             (TESTER_CONTROL == Control_Voltage)
#define TESTER_POLARITY                 (set.test_polarity)
#define TESTER_POLARITY_IS_POSITITVE    (TESTER_POLARITY == Polarity_Positive)
#define TESTER_STEP_U                   (set.test_stepU)
#define TESTER_STEP_I                   (set.test_stepI)
#define TESTER_NUM_SMOOTH               (set.test_smooth)

#define MENU_IS_SHOWN                   (set.menu_show)

#define ENUM_POINTS                     (set.mem_enumPoints)
#define NUM_POINTS                      (1 << (ENUM_POINTS + 9))


#pragma pack(push, 1)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
    void Load();
    void Save();
    void Reset();

    uint            size;                       ///< Размер данной структуры в байтах
    uint            crc32;                      ///< \brief Контрольная сумма данной структуры с хранящимися в ней настройками. Контрольная сумма 
                                                ///< вычисляется от первого байта, следующего за этим полем
    // ДИСПЛЕЙ
    ModeDrawSignal  disp_modeDrawSignal;
    ThicknessSignal disp_thickness;             ///< Толщина линии, которой рисуется сигнал
    // КАНАЛ 1,2
    uint16          chan_shift[NumChannels];    ///< Сдвиг канала по вертикали
    Range           chan_range[NumChannels];    ///< Масштаб канала по вертикали
    Couple          chan_couple[NumChannels];   ///< Связь по входу
    bool            chan_enable[NumChannels];   ///< Включен/выключен канал
    // СИНХР
    Channel         trig_source;
    TrigInput       trig_input;
    TrigPolarity    trig_polarity;
    uint16          trig_lev;
    ModeTrig        trig_mode;
    // СЕРВИС
    Language        serv_lang;
    // Развертка
    uint16          time_shift;
    TBase           time_base;
    // Тестер-компонент
    TesterControl   test_control;
    TesterPolarity  test_polarity;
    TesterStepU     test_stepU;
    TesterStepI     test_stepI;
    uint8           test_smooth;                ///< Количество сглаживаний
    // Память
    EnumPoints      mem_enumPoints;             ///< Число точек
    // Меню
    const Page     *menu_currentPage;           ///< Указатель на открытую страницу меню
    bool            menu_show;                  ///< Если true, то нужно показывать текущую страницу
};

#pragma pack(pop)


extern Settings set;
