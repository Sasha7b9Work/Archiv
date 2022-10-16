#include <QtCore/QFile>
#include <iostream>
#include "../../common/ComFunctions.h"



int main()
{
    char *buffer = 0;
    int size;

    if(!ReadData("rigol13_04.bin", &buffer, &size))
    {
        system("pause");
        return -1;
    }

    
    //WriteString(buffer, 0x0204cf, "MMMMM");
    WriteString(buffer, 0x02058f, "MMMMM");
	ReplaceAllStrings(buffer, size, "RIGOL", "MNIPI");
	WriteString(buffer, 0x390010, "06.2013       ");

    //WriteString(buffer, 0x390000, "C4-");
    //ReplaceAllStrings(buffer, size, "Rigol", "MNIPI");
    //WriteString(buffer, 0x1ff443, "1102");
    //ReplaceAllStrings(buffer, size, "1052", "1102");

    SaveData("rigol13_04_out.bin", buffer, size);
}

