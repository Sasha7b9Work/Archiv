// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Settings/SettingsTypes_.h"
#include "Data/TableAutoMeasures.h"
#include "Menu/Items/MenuItems.h"
#include "Settings/SettingsNRST.h"
#include "Settings/SettingsDefines.h"


 // Настройки каналов
struct SettingsChannel
{
    int16         rshift;
    ModeCouple::E mode_couple;                 // Режим по входу.
    Divider::E    divider;                     // Множитель.
    Range::E      range;                       // Масштаб по напряжению.
    bool          enabled;                     // Включён ли канал.
    bool          inversed;                    // Инвертирован ли канал.
    bool          filtr;                       // Фильтр
};


struct SettingsLAN
{
    uint8 mac0;
    uint8 mac1;
    uint8 mac2;
    uint8 mac3;
    uint8 mac4;
    uint8 mac5;

    uint8 ip0;
    uint8 ip1;
    uint8 ip2;
    uint8 ip3;

    uint16 port;

    uint8 mask0;
    uint8 mask1;
    uint8 mask2;
    uint8 mask3;

    uint8 gw0;
    uint8 gw1;
    uint8 gw2;
    uint8 gw3;

    bool enabled;
};

                                   
struct SettingsCommon
{
    int         countEnables;               // Количество включений. Увеличивается при каждом включении.
    int         countErasedFlashData;       // Сколько раз стирался первый сектор с ресурсами.
    int         countErasedFlashSettings;   // Сколько раз стирался сектор с настройкаи.
    int         workingTimeInSecs;          // Время работы в секундах.
    Language::E lang;                       // Язык меню.
};



struct StructMemoryLast
{
    bool isActiveModeSelect;
};


// Настройки МЕНЮ->ПАМЯТЬ
struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    EnumPointsFPGA::E   enum_points_fpga;                         // Число точек.
    ModeWork            mode_work;                                // Режим работы.
    FileNamingMode::E   file_naming_mode;                         // Режим именования файлов.
    char                file_name_mask[MAX_SYMBOLS_IN_FILE_NAME]; // Здесь маска для автоматического именования файлов\n
        // Правила именования.\n
        // %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47')
        // - секунды\n
        // %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет 
        // преобразовано в 007\n
        // Примеры\n
        // name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32\n
        // При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего,
        //этот параметр будет всегда равен 0001, т.к. для определения номера просматриваются только символы ДО него.
    char                file_name[MAX_SYMBOLS_IN_FILE_NAME];      // Имя файла для режима ручного задания
    int8                index_cur_symbol_name_mask;               // Индекс текущего символа в режиме задания маски или
                                                                  // выбора имени.
    StructMemoryLast    str_memory_last;
    ModeShowIntMem::E   mode_show_int_mem;                        // Какие сигналы показывать в режиме внутреннего ЗУ
    bool                flash_auto_connect;                       // Если true, при подлючении флеш автоматически
                                                                  // выводится NC (Нортон Коммандер)
    ModeBtnMemory::E    mode_button_memory;
    ModeSaveSignal::E   mode_save_signal;                         // В каком виде сохранять сигнал.
};


// Настройки изображения
struct SettingsDisplay
{
    int16               time_show_levels;           // Время, в течение которого нужно показывать уровни смещения.
    int16               shift_in_memory_in_points;  // Показывает смещение левого края сетки относительно нулевого байта
                                                    // памяти. Нужно для правильного отображения сигнала в окне.
    int16               time_messages;              // Время в секундах, на которое сообщение остаётся на экране.
    int16               brightness;                 // Яркость дисплея.
    int16               brightness_grid;            // Яркость сетки от 0 до 100.
    uint                colors[16];                 // Цвета.
    ModeDrawSignal::E   mode_draw_signal;           // Режим отрисовки сигнала.
    TypeGrid::E         type_grid;                  // Тип сетки.
    ENumAccumulation::E enum_accum;                 // Перечисление накоплений сигнала на экране.
    ENumAveraging::E    enum_ave;                   // Перечисление усреднений сигнала.
    ModeAveraging::E    mode_ave;                   // Тип усреднений по измерениям.
    ENumMinMax::E       enum_min_max;               // Число измерений для определения минимумов и максимумов.
    Smoothing::E        smoothing;                  // Число точек для скользящего фильтра.
    ENumSignalsInSec::E enum_fps;                   // Перечисление считываний сигнала в секунду.
    Chan::E             last_affected_channel;      // Здесь хранится номер последнего канала, которым управляли ручками
                                                    // Нужно для того, чтобы знать, какой сигнал рисовать наверху.
    ModeAccumulation::E mode_acum;                  // Задаёт режим накопления сигналов.
    AltMarkers::E       alt_markers;                // Режим отображения дополнительных боковых маркеров смещений.
    MenuAutoHide::E     menu_auto_hide;             // Через сколько времени после последнего нажатия клавиши прятать меню
    bool                show_full_memory_window;    // Показывать ли окно памяти вверху экрана. \todo Не используется.
    bool                show_string_navigation;     // Показывать ли строку текущего состояния меню..
    LinkingRShift::E    linking_rshift;             // Тип привязки смещения по вертикали.

    // Возвращает номер первой и последней точки на экране.
    static BitSet64 PointsOnDisplay();

    // Возвращает номера первого и последнего байтов на экране
    static BitSet64 BytesOnDisplay();
};


// Настройки курсорных измерений.
struct SettingsCursors
{
    CursCntrl::E    cntrlU[Chan::Count];    // Активные курсоры напряжения.
    CursCntrl::E    cntrlT[Chan::Count];    // Активные курсоры напряжения.
    Chan::E         source;                 // Источник - к какому каналу относятся курсоры.
    float           posU[Chan::Count][2];   // Текущие позиции курсоров напряжения обоих каналов.
    float           posT[Chan::Count][2];   // Текущие позиции курсоров времени обоих каналов.
    float           dU_100percents[2];      // Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float           dT_100percents[2];      // Расстояние между курсорами времени для 100%, для обоих каналов.
    CursMovement::E movement;               // Как перемещаться курсорам - по точкам или по процентам.
    CursActive::E   active;                 // Какие курсоры сейчас активны.
    CursLookMode::E look_mode[2];           // Режимы слежения за курсорами для двух пар курсоров.
    bool            show_freq;              // Установленное в true значение, что нужно показывать на экране значение
                                            // 1/dT между курсорами.
    bool            show;                   // Показывать ли курсоры.
};


struct Cursors
{
    // Получить позицию курсора напряжения.
    static float GetPosU(const Chan &ch, int num);

    // Возвращает true,если нужно рисовать курсоры.
    static bool NecessaryDraw();

    // Получить строку курсора напряжения.
    static Voltage GetVoltage(const Chan &source, int num_cur);
    static Voltage GetVoltage(float shift_cur, Range::E range, int16 rshift);

    // Получить строку курсора времени.
    static Time GetTime(const Chan &source, int numCur);
    static Time GetTime(float shift_cur, TBase::E tbase);

    // Получить строку процентов курсоров напряжения.
    static Text GetPercentsU(const Chan &source);

    // Получить строку процентов курсоров времени.
    static Text GetPercentsT(const Chan &source);

    // Нарисовать курсоры курсорных измерений.
    static void Draw();

private:

    // Нарисовать вертикальный курсор курсорных измерений.
    // x         - числовое значение курсора
    // y_tearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения
    static void DrawVertical(int x, int yTearing);

    // Нарисовать горизонтальный курсор курсорных измерений
    // y         - числовое значение курсора
    // x_tearing - координата места, в котором необходимо сделать разрыв для квадрата пересечения
    static void DrawHorizontal(int y, int x_tearing);
};


// Настройки меню ИЗМЕРЕНИЯ
struct SettingsMeasures
{
    MeasuresNumber::E  number;            // Сколько измерений выводить.
    Chan::E            source;            // Для каких каналов выводить измерения.
    ModeViewSignals::E mode_view_signals; // Сжимать ли сигналы при выводе измерений.
    TypeMeasure::E     measures[15];      // Выбранные для индикации измерения.
    int8               show;              // Показывать ли измерения.
    MeasuresField::E   field;             // Задаёт область, из которой берутся значения для расчёта измерений.
    int16              posU[2];           // Позиции курсоров, которые задают область, из которой берутся значения для
                                          // расчёта измерений при field == MeasuresField::Hand.
    int16              posT[2];           // Позиции курсоров, которые задают область, из которой берутся значения для
                                          // расчёта измерений при field == MeasuresField::Hand.
    CursCntrl::E       cntrlU;            // Активные курсоры напряжения.
    CursCntrl::E       cntrlT;            // Активные курсоры времени.
    CursActive::E      curs_active;       // Какие курсоры активны - по времени или напряжению.
    TypeMeasure::E     marked;            // Измерение, на которое нужно выводить маркеры.
};


struct SettingsMath
{
    struct FFT
    {
        ScaleFFT::E  scale;          // Масштаб шкалы для отображения спектра.
        SourceFFT::E source;         // Источник сигнала для расчёта и отображения спектра.
        WindowFFT::E window;         // Задаёт вид окна, на которое умножается сигнал перед расчётом спектра.
        MaxDbFFT::E  max_DB;         // Минимальное значение на вертикальной координате графика спектра.
        uint8        cursor;         // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
        uint8        pos_cur[2];     // Позиция курсора спектра. Изменятеся 0...256.
        bool         enabled;        // \c true, если включён спектр.
    } fft;
    Function::E      func;           // Включённая математическая функция.
    int8             koeff1add;      // Коэффициент при первом слагаемом для сложения.
    int8             koeff2add;
    int8             koeff1mul;
    int8             koeff2mul;
    ModeDrawMath::E  modeDraw;       // Раздельный или общий дисплей в режиме математической функции.
    ModeRegSet::E    modeRegSet;     // Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Range::E         range;          // Масштаб по напряжению математического сигнала.
    Divider::E       multiplier;     // Множитель масштаба математического сигнала.
    int16            rShift;         // Смещение по вертикальной оси математического сигнала.

    static bool IsMathEnabled();
};



// Настройки меню СЕРВИС.
struct SettingsService
{
    CalibratorMode::E calibrator_mode;     // Режим работы калибратора.
    ColorScheme::E    colorScheme;         // Цветовая схеама.
    bool              sound_enabled;       // Включены ли звуки.
    int8              IPaddress;           // IP-адрес (временно).
};


// Настройки синхронизации
struct SettingsTrig
{
    StartMode::E         start_mode;         // Режим запуска.
    TrigSource::E        source;             // Источник.
    TrigPolarity::E      polarity;           // Тип синхронизации.
    TrigInput::E         input;              // Вход синхронизации.
    int16                levelRel[3];        // Уровень синхронизации для трёх источников.
    ModeLongPressTrig::E modeLongPressTrig;  // Режим работы длительного нажатия кнопки СИНХР.
    TrigModeFind::E      modeFind;           // Поиск синхронизации - вручную или автоматически.
};


// Настройки оси X.
struct SettingsTime
{
    TBase::E          base;             // Масштаб по времени.
    FunctionTime::E   function_time;
    TPos::E           t_pos;            // Привязка синхронизации к памяти.
    SampleType::E     sample_type;      // Тип выборки для режима рандомизатора.
    PeakDetMode::E    peak_det;         // Режим работы пикового детектора
    EnumPointsFPGA::E old_num_points;   // \brief Когда переключаемся в режим пикового детектора, устанавливаем
                                        // количество точек в 1024, а сюда  записываем то, что было, чтобы потом
                                        // восстановить.
    int               shift;            // Смещение по времени.
    bool              self_recorder;    // Включен ли режим самописца.
};


struct SettingsMenu
{
    bool        isShown;            // Меню показано.
    NamePage::E currentPage_;        // Последняя открытая страница (она будет показана при нажатии на кнопку МЕНЮ)
    int         currentSubPage;     // Открытая подстраница текущей страницы
};


// Струкура хранит все настройки прибора.
struct Settings
{
    uint             size;
    uint             crc32;
    uint             number;
    SettingsDisplay  display;               // настройки изображения          (меню ДИСПЛЕЙ).
    SettingsChannel  chan[Chan::Count];     // настройки каналов              (меню КАНАЛ 1 и КАНАЛ 2).
    SettingsTrig     trig;                  // настройки синхронизации        (меню СИНХР).
    SettingsTime     time;                  // временнЫе настройки            (меню РАЗВЁРТКА).
    SettingsCursors  cursors;               // настройки курсорных измерений  (меню КУРСОРЫ).
    SettingsMemory   memory;                // настройки режимов памяти       (меню ПАМЯТЬ).
    SettingsMeasures measures;              // настройки измерений            (меню ИЗМЕРЕНИЯ).
    SettingsMath     math;                  // настройки режима математических измерений.
    SettingsService  service;               // дополнительные настройки       (меню СЕРВИС).
    SettingsLAN      LAN;                   // настройки для соединения по локальной сети.
    SettingsCommon   common;                // системные настройки.
    SettingsMenu     menu;                  // состояние меню.
    uint             unused_crc32;

    void Load();

    static void Save();

    void Reset();

    // Рассчитать и записать контрольную сумму
    uint CalcWriteCRC32();

    static bool DebugModeEnabled() { return true; }

    bool operator!=(const Settings &rhs);
};


extern Settings set;


#define MAC_ADDR0       (set.LAN.mac0)
#define MAC_ADDR1       (set.LAN.mac1)
#define MAC_ADDR2       (set.LAN.mac2)
#define MAC_ADDR3       (set.LAN.mac3)
#define MAC_ADDR4       (set.LAN.mac4)
#define MAC_ADDR5       (set.LAN.mac5)

#define IP_ADDR0        (set.LAN.ip0)
#define IP_ADDR1        (set.LAN.ip1)
#define IP_ADDR2        (set.LAN.ip2)
#define IP_ADDR3        (set.LAN.ip3)

#define NETMASK_ADDR0   (set.LAN.mask0)
#define NETMASK_ADDR1   (set.LAN.mask1)
#define NETMASK_ADDR2   (set.LAN.mask2)
#define NETMASK_ADDR3   (set.LAN.mask3)

#define GW_ADDR0        (set.LAN.gw0)
#define GW_ADDR1        (set.LAN.gw1)
#define GW_ADDR2        (set.LAN.gw2)
#define GW_ADDR3        (set.LAN.gw3)


#define LANG            (set.common.lang)       // SettingsMain.lang
#define LANG_RU         (LANG == Language::Russian)
#define LANG_EN         (LANG == Language::English)
