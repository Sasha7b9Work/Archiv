// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "common/Utils/Containers/String_.h"
#include <ff.h>


namespace FDrive
{
    struct StructForWrite
    {
        static const int SIZE_FLASH_TEMP_BUFFER = 512;
        uint8 tempBuffer[SIZE_FLASH_TEMP_BUFFER];
        int sizeData;
        FIL fileObj;
        char name[255];
    };

    struct StructForReadDir
    {
        char nameDir[_MAX_LFN + 1];
        char lfn[(_MAX_LFN + 1)];
        FILINFO fno;
        DIR dir;
    };

    struct State { enum E {
        Idle,
        Start,
        Running,
    }; };
}


namespace FDrive
{
    extern State::E state;

    void Init();

    void Update();

    void GetNumDirsAndFiles(const char* fullPath, int *numDirs, int *numFiles);

    String GetNameDir(const char* fuulPath, int numDir, StructForReadDir *sfrd);

    bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);

    void CloseCurrentDir(StructForReadDir *sfrd);

    bool GetNameFile(pchar fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);

    bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);

    // ������� ������� ���� ��� ������. ���� ����� ���� ��� ����������, ����� ���, ������� ����� ������� ����� �
    // ������� ���
    bool OpenNewFileForWrite(const char* fullPathToFile);

    bool WriteToFile(uint8* data, int sizeData);

    // ���������� ������ � ����, �������� � ��������� �������� �������
    bool WriteStringToFile(pchar string);

    bool CloseFile();

    bool AppendStringToFile(const char* string);

    void ChangeState();

    bool IsConnected();

    // ��������� ������� ������ �� ������ � ����, �������������� ������������� ����������
    void SaveCurrentSignal();

    String CreateFileName(pchar extension);

    extern bool needOpenFileMananger;    // ���� 1, �� ����� ������� �������� �������� (��������� ���������������)
};
