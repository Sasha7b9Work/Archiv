#pragma once
#include "defines.h"
#include "main.h"

class FDrive
{
public:
    void Init();
    bool Update();
    bool FileExist(char *fileName);
    int OpenFileForRead(char *fileName);
    /// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    int ReadFromFile(int numBytes, uint8 *buffer);
    void CloseOpenedFile();
};

extern FDrive drive;
