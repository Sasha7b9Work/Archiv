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

    // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и
    // откроет его
    bool OpenNewFileForWrite(const char* fullPathToFile);

    bool WriteToFile(uint8* data, int sizeData);

    // Записывает строку в файл, завершая её символами перевода каретки
    bool WriteStringToFile(pchar string);

    bool CloseFile();

    bool AppendStringToFile(const char* string);

    void ChangeState();

    bool IsConnected();

    // Сохранить текущий сигнал на флешку в виде, соотвествующем установленным настройкам
    void SaveCurrentSignal();

    String CreateFileName(pchar extension);

    extern bool needOpenFileMananger;    // Если 1, то нужно открыть файловый менеджер (сработало автоподключение)
};
