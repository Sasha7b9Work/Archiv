#include <QtCore/QFile>
#include <iostream>
#include "../../common/ComFunctions.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        cout << "ERROR!!! Must be given name file and string to find" << endl;
        system("pause");
        return -1;
    }

    int numSearch = argc - 2;

    int size = 0;
    char *buffer = 0;

    if(!ReadData(argv[1], &buffer, &size))
    {
        cout << "ERROR!!! Can not read file" << argv[1] << endl;
        system("pause");
        return -1;
    }

    for(int i = 0; i < size; i++)
    {
        for(int count = 0; count < numSearch; count++)
        {
            char *word = argv[2 + count];
            if(VerifyOnWord(buffer + i, word))
                cout << (void*)i << " : " << word << endl;
        }
    }
}