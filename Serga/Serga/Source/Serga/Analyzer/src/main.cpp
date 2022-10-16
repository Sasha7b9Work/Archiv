//  [11/26/2009 Sasha]
#pragma warning(disable:4127)
#include <QApplication>
#pragma warning(default:4127)
#include "WindowAnalyzer.h"
#include "../common/TablesOfConverting.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Device129Multi.lib")
#else
#pragma comment(lib, "../Release/Device129Multi.lib")
#endif

TablesOfConverting tables;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WindowAnalyzer *window;

    window = new WindowAnalyzer;
    window->show();
    window->move(400, 0);
    return app.exec();
}