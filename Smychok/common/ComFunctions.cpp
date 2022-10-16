#include "ComFunctions.h"
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <iostream>

using namespace std;

bool ReadData(char *_nameFile, char **_buffer, int *_size)
{
    QFile file(_nameFile);
    if(!file.open(QIODevice::ReadOnly))
    {
        cout << "ERROR!!! Input file " << _nameFile << " not found..." << endl;
        return false;
    }

    QByteArray data = file.readAll();
    *_size = data.size();
    *_buffer = new char[data.size() + 1];
    memcpy(*_buffer, data.data(), *_size);

    file.close();

    return true;
}

bool SaveData(char *_nameFile, char *_buffer, int size)
{
    QFile file(_nameFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        cout << "ERROR!!! Output file " << _nameFile << " can not be create..." << endl;
        return false;
    }

    file.write(_buffer, size);

    file.close();

    return true;
}

int FindDifferentLines(const char *_buf1, const char *_buf2, char ** _out1, char ** _out2)
{
    const char *start1 = _buf1;
    const char *start2 = _buf2;

    int size = 0;

    while(*_buf1 != *_buf2)
    {
        _buf1++;
        _buf2++;
        size++;
    }

    *_out1 = new char[size + 1];
    *_out2 = new char[size + 1];
    memcpy(*_out1, start1, size);
    memcpy(*_out2, start2, size);
    (*_out1)[size] = '\0';
    (*_out2)[size] = '\0';

    return size;
}

bool VerifyOnWord(char *_sequence, const char *_word)
{
    int size = strlen(_word);

    for(int i = 0; i < size; i++)
    {
        if(_sequence[i] != _word[i])
            return false;
    }
    return true;
}

void ReplaceWord(char *_sequency, const char *_word)
{
    int size = strlen(_word);

    for(int i = 0; i < size; i++)
        _sequency[i] = _word[i];
}

char* GetWord(int _numWord)
{
    static char word[] = "00000";

    _numWord &= 0x1f;

    for(int i = 4; i >= 0; i--)
    {
        word[i] = (_numWord & (int)1) ? 'I' : 'O';
        _numWord >>= 1;
    }

    return word;
}

void WriteString(char *_buffer, int _offset, char *_string)
{
    ReplaceWord(_buffer + _offset, _string);
}

void ReplaceAllStrings(char *_bufer, int _size, char *_oldString, char *_newString)
{
    for(int i = 0; i < _size; i++)
        if(VerifyOnWord(_bufer + i, _oldString))
        {
            ReplaceWord(_bufer + i, _newString);
            i += sizeof(_newString);
        }
}