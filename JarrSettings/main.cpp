#include <iostream>
#include "attribute.h"
#include "settings.h"


using namespace std;
using namespace jarr;

int main()
{
    cout << "Begining" << endl;
    Settings *myset = new Settings;

    myset->path = "C:/Users/Aljover/temp/test.txt";
    myset->load();
    myset->dump();

    myset->modify("var2","hola2");

    myset->path = "C:/Users/Aljover/temp/test.txt";
    //myset->save();


    Attribute& at = myset->retrive_att("var2");
    at.value = "hola mundo";

    myset->dump();

    cout << "Hello World!" << endl;

    cout << myset->retrive("var2") << endl;
    return 0;
}

