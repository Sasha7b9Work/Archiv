#pragma once

bool ReadData(char *_nameFile, char **_buffer, int *_size);
bool SaveData(char *_nameFile, char *_buffer, int size);

// Записать в _out1 и _out2 различающиеся строки, начиная с _buf1 и _buf2
int FindDifferentLines(const char *_buf1, const char *_buf2, char ** _out1, char ** _out2);

// Вернуть true, если по адресу _sequence находится _word (нулевой символ в конце не учитывается)
bool VerifyOnWord(char *_sequence, const char *_word);

// Записать по адресу _sequency последовательность _word (без нулевого символа)
void ReplaceWord(char *_sequency, const char *_word);

char* GetWord(int _numWorld);           // Получить "двоичное представление" числа в виде "OOOOO" ... "IIIII"

// Записать в _buffer по адресу _offset строку _string (без нулевого символа)
void WriteString(char *_buffer, int _offset, char *_string);

// Заменить все _oldString на _newString (без нулевого символа)
void ReplaceAllStrings(char *_bufer, int _size, char *_oldString, char *_newString);