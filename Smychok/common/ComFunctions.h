#pragma once

bool ReadData(char *_nameFile, char **_buffer, int *_size);
bool SaveData(char *_nameFile, char *_buffer, int size);

// �������� � _out1 � _out2 ������������� ������, ������� � _buf1 � _buf2
int FindDifferentLines(const char *_buf1, const char *_buf2, char ** _out1, char ** _out2);

// ������� true, ���� �� ������ _sequence ��������� _word (������� ������ � ����� �� �����������)
bool VerifyOnWord(char *_sequence, const char *_word);

// �������� �� ������ _sequency ������������������ _word (��� �������� �������)
void ReplaceWord(char *_sequency, const char *_word);

char* GetWord(int _numWorld);           // �������� "�������� �������������" ����� � ���� "OOOOO" ... "IIIII"

// �������� � _buffer �� ������ _offset ������ _string (��� �������� �������)
void WriteString(char *_buffer, int _offset, char *_string);

// �������� ��� _oldString �� _newString (��� �������� �������)
void ReplaceAllStrings(char *_bufer, int _size, char *_oldString, char *_newString);