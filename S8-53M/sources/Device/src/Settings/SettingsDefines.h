// 2021/08/05 11:26:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define FUNCTION_REG_TIME                      set.time.function_time
#define FUNCTION_REG_TIME_SHIFT_IN_MEMORY      (FUNCTION_REG_TIME == FunctionTime::ShiftInMemory)

#define MEASURES_MODE_VIEW_SIGNALS             set.measures.mode_view_signals
#define MEASURES_MODE_VIEW_SIGNALS_IS_COMPRESS (MEASURES_MODE_VIEW_SIGNALS == ModeViewSignals::Compress)

#define BRIGHTNESS_DISPLAY              set.display.brightness
#define BRIGHTBESS_GRID                 set.display.brightness_grid

#define CALIBRATOR_MODE                 set.service.calibrator_mode

#define COLOR_SCHEME                    set.service.colorScheme
#define COLOR_SCHEME_IS_WHITE_LETTERS   (COLOR_SCHEME == ColorScheme::WhiteLetters)

#define CURS_100_PERCENTS_DT(num)       set.cursors.dT_100percents[num]
#define CURS_100_PERCENTS_DU(num)       set.cursors.dU_100percents[num]

#define CURS_ACTIVE                     set.cursors.active
#define CURS_ACTIVE_IS_T                (CURS_ACTIVE == CursActive::T)
#define CURS_ACTIVE_IS_U                (CURS_ACTIVE == CursActive::U)

#define CURS_LOOK_MODE(num)             set.cursors.look_mode[num]
#define CURS_LOOK_MODE_0                CURS_LOOK_MODE(0)
#define CURS_LOOK_MODE_1                CURS_LOOK_MODE(1)

#define CURS_MOVEMENT                   set.cursors.movement
#define CURS_MOVEMENT_IS_PERCENTS       (CURS_MOVEMENT == CursMovement::Percents)

#define CURS_SHOW                       set.cursors.show

#define CURS_SHOW_FREQ                  set.cursors.show_freq

#define CURS_SOURCE                     set.cursors.source

#define CURS_T_POS(src, num)            set.cursors.posT[src][num]
#define CURS_U_POS(src, num)            set.cursors.posU[src][num]

#define CURS_T_CONTROL(ch)              set.cursors.cntrlT[ch]
#define CURS_T_CONTROL_IS_DISABLED(ch)  (CURS_T_CONTROL(ch) == CursCntrl::Disabled)

#define CURS_U_CONTROL(ch)              set.cursors.cntrlU[ch]
#define CURS_U_CONTROL_IS_DISABLED(ch)  (CURS_U_CONTROL(ch) == CursCntrl::Disabled)

#define DIVIDER(ch)                     set.chan[ch].divider
#define DIVIDER_A                       DIVIDER(0)
#define DIVIDER_B                       DIVIDER(1)

#define ENABLED(ch)                     set.chan[ch].enabled
#define ENABLED_A                       ENABLED(0)
#define ENABLED_B                       ENABLED(1)

#define ENUM_ACCUM                      set.display.enum_accum
#define ENUM_ACCUM_IS_INFINITY          (ENUM_ACCUM == ENumAccumulation::Infinity)
#define ENUM_ACCUM_IS_NONE              (ENUM_ACCUM == ENumAccumulation::_1)

#define ENUM_AVERAGE                    set.display.enum_ave

#define ENUM_FPS                        set.display.enum_fps

#define ENUM_MIN_MAX                    set.display.enum_min_max

#define ENUM_POINTS_FPGA                set.memory.enum_points_fpga

#define FFT_IS_ENABLED                  set.math.fft.enabled

#define FFT_MAX_DB                      set.math.fft.max_DB

#define FFT_NUM_CURSOR                  set.math.fft.cursor

#define FFT_POS_CUR(num)                set.math.fft.pos_cur[num]
#define FFT_POS_CUR_0                   FFT_POS_CUR(0)
#define FFT_POS_CUR_1                   FFT_POS_CUR(1)

#define FFT_SCALE                       set.math.fft.scale
#define FFT_SCALE_IS_LINEAR             (FFT_SCALE == ScaleFFT::Linear)
#define FFT_SCALE_IS_LOG                (FFT_SCALE == ScaleFFT::Log)

#define FFT_SOURCE                      set.math.fft.source
#define FFT_SOURCE_IS_A                 (FFT_SOURCE == SourceFFT::A_)
#define FFT_SOURCE_IS_B                 (FFT_SOURCE == SourceFFT::B_)

#define FFT_WINDOW                      set.math.fft.window
#define FFT_WINDOW_IS_BLACKMAN          (FFT_WINDOW == WindowFFT::Blackman)
#define FFT_WINDOW_IS_HAMMING           (FFT_WINDOW == WindowFFT::Hamming)
#define FFT_WINDOW_IS_HANN              (FFT_WINDOW == WindowFFT::Hann)

#define FILE_NAME                       set.memory.file_name

#define FILE_NAME_MASK                  set.memory.file_name_mask

#define FILE_NAMING_MODE                set.memory.file_naming_mode
#define FILE_NAMING_MODE_IS_HAND        (FILE_NAMING_MODE == FileNamingMode::Manually)
#define FILE_NAMING_MODE_IS_MASK        (FILE_NAMING_MODE == FileNamingMode::Mask)

#define FILTR(ch)                       set.chan[ch].filtr
#define FILTR_A                         FILTR(0)
#define FILTR_B                         FILTR(1)

#define FLASH_AUTO_CONNECT              set.memory.flash_auto_connect

#define INDEX_SYMBOL_NAME_MASK          set.memory.index_cur_symbol_name_mask

#define INVERSED(ch)                    set.chan[ch].inversed
#define INVERSED_A                      INVERSED(0)
#define INVERSED_B                      INVERSED(1)

#define LAST_AFFECTED_CHANNEL           set.display.last_affected_channel
#define LAST_AFFECTED_CHANNEL_IS_A      (LAST_AFFECTED_CHANNEL == Chan::A)

#define LINKING_RSHIFT                  set.display.linking_rshift
#define LINKING_RSHIFT_IS_VOLTAGE       (LINKING_RSHIFT == LinkingRShift::Voltage)

#define MATH_FUNC                       set.math.func
#define MATH_FUNC_IS_MUL                (MATH_FUNC == Function::Mul)
#define MATH_FUNC_IS_SUM                (MATH_FUNC == Function::Sum)

#define MATH_MODE_REG_SET               set.math.modeRegSet
#define MATH_MODE_REG_SET_IS_RANGE      (MATH_MODE_REG_SET == ModeRegSet::Range)
#define MATH_MODE_REG_SET_IS_RSHIFT     (MATH_MODE_REG_SET == ModeRegSet::RShift)

#define MATH_MULTIPLIER                 set.math.multiplier

#define MATH_RANGE                      set.math.range

#define MEASURE(num)                    set.measures.measures[num]
#define MEASURE_MARKED                  set.measures.marked
#define MEASURE_IS_MARKED(meas)         ((meas) == MEASURE_MARKED)

#define MEASURES_CURS_ACTIVE            set.measures.curs_active
#define MEASURES_CURS_ACTIVE_IS_T       (MEASURES_CURS_ACTIVE == CursActive::T)

#define MEASURES_CURS_CNTRL_T           set.measures.cntrlT
#define MEASURES_CURS_CNTRL_U           set.measures.cntrlU

#define MEASURES_FIELD                  set.measures.field
#define MEASURES_FIELD_IS_HAND          (MEASURES_FIELD == MeasuresField::Hand)

#define MEASURES_NUMBER                 set.measures.number
#define MEASURES_NUMBER_IS_1            (MEASURES_NUMBER == MeasuresNumber::_1)
#define MEASURES_NUMBER_IS_1_5          (MEASURES_NUMBER == MeasuresNumber::_1_5)
#define MEASURES_NUMBER_IS_2_5          (MEASURES_NUMBER == MeasuresNumber::_2_5)
#define MEASURES_NUMBER_IS_3_5          (MEASURES_NUMBER == MeasuresNumber::_3_5)
#define MEASURES_NUMBER_IS_6_1          (MEASURES_NUMBER == MeasuresNumber::_6_1)
#define MEASURES_NUMBER_IS_6_2          (MEASURES_NUMBER == MeasuresNumber::_6_2)

#define MEASURES_POS_CUR_T(num)         set.measures.posT[num]
#define MEASURES_POS_CUR_T_0            MEASURES_POS_CUR_T(0)
#define MEASURES_POS_CUR_T_1            MEASURES_POS_CUR_T(1)

#define MEASURES_POS_CUR_U(num)         set.measures.posU[num]
#define MEASURES_POS_CUR_U_0            MEASURES_POS_CUR_U(0)
#define MEASURES_POS_CUR_U_1            MEASURES_POS_CUR_U(1)

#define MEASURES_SOURCE                 set.measures.source
#define MEASURES_SOURCE_IS_A            (MEASURES_SOURCE == Chan::A)
#define MEASURES_SOURCE_IS_B            (MEASURES_SOURCE == Chan::B)
#define MEASURES_SOURCE_IS_A_B          (MEASURES_SOURCE == Chan::A_B)

#define MENU_AUTO_HIDE                  set.display.menu_auto_hide

#define MENU_SHOWN                      set.menu.isShown
#define MENU_IS_SHOWN                   (MENU_SHOWN == 1)

#define MODE_ACCUM                      set.display.mode_acum
#define MODE_ACCUM_IS_RESET             (MODE_ACCUM == ModeAccumulation::Reset)
#define MODE_ACCUM_IS_NO_RESET          (MODE_ACCUM == ModeAccumulation::NoReset)

#define MODE_ALT_MARKERS                set.display.alt_markers
#define MODE_ALT_MARKERS_IS_HIDE        (MODE_ALT_MARKERS == AltMarkers::Hide)

#define MODE_AVERAGE                    set.display.mode_ave

#define MODE_BUTTON_MEMORY              set.memory.mode_button_memory
#define MODE_BUTTON_MEMORY_IS_SAVE      (MODE_BUTTON_MEMORY == ModeBtnMemory::Save)

#define MODE_COUPLE(ch)                 set.chan[ch].mode_couple
#define MODE_COUPLE_A                   MODE_COUPLE(0)
#define MODE_COUPLE_B                   MODE_COUPLE(1)

#define MODE_DRAW_MATH                  set.math.modeDraw
#define MODE_DRAW_MATH_IS_DISABLED      (MODE_DRAW_MATH == ModeDrawMath::Disable)
#define MODE_DRAW_MATH_IS_SEPARATE      (MODE_DRAW_MATH == ModeDrawMath::Separate)
#define MODE_DRAW_MATH_IS_TOGETHER      (MODE_DRAW_MATH == ModeDrawMath::Together)

#define MODE_DRAW_SIGNAL                set.display.mode_draw_signal
#define MODE_DRAW_SIGNAL_IS_LINES       (MODE_DRAW_SIGNAL == ModeDrawSignal::Lines)

#define MODE_SAVE_SIGNAL                set.memory.mode_save_signal
#define MODE_SAVE_SIGNAL_IS_BMP         (MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)

#define MODE_SHOW_INT_MEM               set.memory.mode_show_int_mem

#define MODE_PEAK_DET                   set.time.peak_det
#define PEAK_DET_IS_ALLOWED             (TBASE >= TBase::MIN_PEAK_DET)              // true, если разрешён режим пикового детектора
#define PEAK_DET_IS_ENABLED             (MODE_PEAK_DET != PeakDetMode::Disable)

#define RANGE(ch)                       set.chan[ch].range
#define RANGE_A                         RANGE(0)
#define RANGE_B                         RANGE(1)

#define RSHIFT(ch)                      set.chan[ch].rshift
#define RSHIFT_A                        RSHIFT(0)
#define RSHIFT_B                        RSHIFT(1)
#define RSHIFT_MATH                     set.math.rShift

#define SAMPLE_TYPE                     set.time.sample_type

#define SELF_RECORDER                   set.time.self_recorder

#define SHIFT_IN_MEMORY_IN_POINTS       set.display.shift_in_memory_in_points

#define SHOW_FULL_MEMORY_WINDOW         set.display.show_full_memory_window

#define SHOW_MEASURES                   set.measures.show

#define SHOW_STRING_NAVIGATION          set.display.show_string_navigation

#define SMOOTHING                       set.display.smoothing
#define SMOOTHING_IS_DISABLED           (SMOOTHING == Smoothing::Disable)

#define SOUND_IS_ENABLED                set.service.sound_enabled

#define START_MODE                      set.trig.start_mode
#define START_MODE_IS_AUTO              (START_MODE == StartMode::Auto)
#define START_MODE_IS_SINGLE            (START_MODE == StartMode::Single)

#define TBASE                           set.time.base
#define TBASE_IS_P2P                    (TBASE >= TBase::MIN_P2P)       // true, если установлена поточечная развёртка
#define TBASE_IS_RAND                   (TBASE <= TBase::MAX_RANDOMIZE) // true, если установлена рандомизаторная развёртка

#define TIME_MESSAGES                   set.display.time_messages

#define TIME_SHOW_LEVELS                set.display.time_show_levels

#define TPOS                            set.time.t_pos
#define TPOS_IS_LEFT                    (TPOS == TPos::Left)
#define TPOS_IS_CENTER                  (TPOS == TPos::Center)
#define TPOS_IS_RIGHT                   (TPOS == TPos::Right)

#define TRIG_INPUT                      set.trig.input
#define TRIG_INPUT_IS_AC                (TRIG_INPUT == TrigInput::AC)

#define TRIG_LEV(ch)                    set.trig.levelRel[ch]
#define TRIG_LEV_A                      TRIG_LEV(0)
#define TRIG_LEV_B                      TRIG_LEV(1)
#define TRIG_LEV_CURRENT                TRIG_LEV(TRIG_SOURCE)  // Уровень синхронизации по текущему источнику синхр.

#define TRIG_MODE_FIND                  set.trig.modeFind
#define TRIG_MODE_FIND_IS_HAND          (TRIG_MODE_FIND == TrigModeFind::Hand)

#define TRIG_MODE_LONG_PRESS            set.trig.modeLongPressTrig

#define TRIG_POLARITY                   set.trig.polarity
#define TRIG_POLARITY_IS_FRONT          (TRIG_POLARITY == TrigPolarity::Front)

#define TRIG_SOURCE                     set.trig.source
#define TRIG_SOURCE_IS_A                (TRIG_SOURCE == TrigSource::A)
#define TRIG_SOURCE_IS_B                (TRIG_SOURCE == TrigSource::B)
#define TRIG_SOURCE_IS_EXT              (TRIG_SOURCE == TrigSource::Ext)

#define TSHIFT                          set.time.shift

#define TYPE_GRID                       set.display.type_grid
#define TYPE_GRID_IS_1                  (TYPE_GRID == TypeGrid::_1)
#define TYPE_GRID_IS_2                  (TYPE_GRID == TypeGrid::_2)
#define TYPE_GRID_IS_3                  (TYPE_GRID == TypeGrid::_3)
