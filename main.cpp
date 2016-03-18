#include "mainwindow.h"
#include <QApplication>
#include "JarrSettings/settings.h"
#include <iostream>
#include <ui_mainwindow.h>
#include <windows.h>



int main(int argc, char *argv[])
{
    setlocale( LC_ALL, "" );

    if(argc)
    {
        for (int var = strlen(argv[0]);  var > 0; --var) {
            if(argv[0][var] == '\\')
            {
                argv[0][var] = 0;
                var = 0;
                break;
            }
        }
        std::cout << "execution path: " << argv[0] << std::endl;
    }

    QApplication a(argc, argv);
    MainWindow w;

    // fix problem when argv has special characters
    LPWSTR *argList;
    int n_argList = 0;
    argList = CommandLineToArgvW(GetCommandLineW(), &n_argList);

    if(argc >= 2)
    {
        w.customImg = QString::fromWCharArray(argList[1],wcslen(argList[1]));
        w.hasCustomImg = true;
    }

    w.execution_path = argv[0];
    w.initialLoad();
    w.show();

    return a.exec();
}
