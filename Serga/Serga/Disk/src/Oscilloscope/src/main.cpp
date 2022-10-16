#pragma warning(disable:4127)
#include <QApplication>
#include <QTextCodec>
#pragma warning(default:4127)
#include "../common/TablesOfConverting.h"
#include "../Device129Multi/src/Device129Multi.h"

#include "WindowOsci.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Device129Multi.lib")
#else
#pragma comment(lib, "../Release/Device129Multi.lib")
#endif

TablesOfConverting tables;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec *rusCodec = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForTr(rusCodec);

    WindowOsci *window = new WindowOsci;

    window->show();
    int retValue = app.exec();

    return retValue;
}