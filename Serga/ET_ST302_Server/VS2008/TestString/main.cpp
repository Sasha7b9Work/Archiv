#include "../../src/String.h"
#include <iostream>

using namespace std;

int main() {
    
    //String string1("string1");

    /*
    String *string1 = new String("string1");

    cout << string1->c_str() << endl;

    *string1 += " string2";
    cout << string1->c_str() << endl;

    cout << (*string1 += " string3").c_str() << endl;

    String string4(10000);
    
    cout << string4.c_str() << endl;

    *string1 += string4;

    cout << string1->c_str() << endl;

    delete string1;
    */

    /*
    char buf[] = "test";

    cout << buf << " " << strlen(buf) << endl;
    */

    for(int i = 0; i < 20; i++) {
        String string(i);
        cout << string.c_str() << endl;
    }

    return 0;
}