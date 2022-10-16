#include <QtCore/QFile>
#include <iostream>
#include <stdio.h>
#include "../../common/ComFunctions.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "ERROR!!! Must be given two files to compare" << endl;
        system("pause");
        return -1;
    }

    int size1 = 0;
    int size2 = 0;

    char *buffer1 = 0;
    char *buffer2 = 0;

    if(!ReadData(argv[1], &buffer1, &size1))
    {
        cout << "ERROR!!! Can not read file " << argv[1] << endl;
        system("pause");
        return -1;
    }

    if(!ReadData(argv[2], &buffer2, &size2))
    {
        cout << "ERROR!!! Can not read file " << argv[2] << endl;
        system("pause");
        return -1;
    }

    if(size1 != size2)
    {
        cout << "ERROR!!! Sizes do not match" << endl;
        system("pause");
        return -1;
    }

    char *str1 = 0;
    char *str2 = 0;

    for(int i = 0; i < size1; i++)
    {
        if(buffer1[i] != buffer2[i])
        {
            cout << (void*)i << " : ";
            i += FindDifferentLines(buffer1 + i, buffer2 + i, &str1, &str2);
            cout << str1 << " - " << str2 << endl;
        }
    }

 //   _getch();

    char buffer[100];

    cin >> buffer;
}