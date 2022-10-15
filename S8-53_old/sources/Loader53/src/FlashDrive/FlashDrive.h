#pragma once
#include "defines.h"
#include "main.h"

class FDrive
{
public:
    static void Init();
    static bool Update();
    static bool FileExist(char *fileName);
    static int OpenFileForRead(char *fileName);
    /// ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    static int ReadFromFile(int numBytes, uint8 *buffer);
    static void CloseOpenedFile();
};
