#pragma once

#define SAFE_DELETE(x)       if(x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x) if(x) { delete []x; x = NULL; }

#define INVALID_VALUE_DOUBLE 1e100
#define EMPTY_STRING "---.---"
// Здесь определено имя файла, в котором хранятся калибровочные коэффициенты для первого и второго каналов,
// а также аддр_мин и аддр_макс для рандомизатора
// Файл общий на все четыре прибора
// Запись в таком порядке:
// Коэфф. калибр. 0, коэфф. калибр. 1, аддр_мин, аддр_макс
#define CALIBRATE_NAME_FILE "calibrate.cfg"

// Время в микросекундах, требующееся для приёма одного байта от модуля
#define TIME_ONE_BYTE 100.0

// Уровень для отметки входа/выхода в/из процедуру/процедуры
#define LEVEL_INPUT_PROC 10
// Уровень для трассировки выполнения процедуры
#define LEVEL_TRACE_PROC 11