//  [11/27/2009 Sasha]
#pragma warning(disable:4127)
#include <QApplication>
#pragma warning(default:4127)
#include "WindowRegistrator.h"
#include "../common/TablesOfConverting.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Device129Multi.lib")
#else
#pragma comment(lib, "../Release/Device129Multi.lib")
#endif

TablesOfConverting tables;

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    WindowRegistrator *window;
    window = new WindowRegistrator;
    window->show();
    window->move(50, 50);
    return app.exec();
}