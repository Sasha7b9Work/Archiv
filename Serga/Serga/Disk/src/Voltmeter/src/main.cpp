//  [11/25/2009 Sasha]
#pragma warning(disable:4127)
#include <QApplication>
#pragma warning(default:4127)
#include "../common/defines.h"
#include "Voltmeter_Window.h"
#include "../../common/Timer.h"
#include <Windows.h>
#include "../common/TablesOfConverting.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Device129Multi.lib")
#else
#pragma comment(lib, "../Release/Device129Multi.lib")
#endif

TablesOfConverting tables;

int main(int argc, char *argv[]) {
    /*
    BOOL rezult = SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    Timer timer;
    timer.Reset();
    long long currentTime = 0;
    long long prevTime = timer.GetTime().ToUSec();
    long long deltaGreat10000 = 0;
    long long deltaGreat1000 = 0;
    long long deltaGreat100 = 0;
    long long deltaGreat10 = 0;
    long long deltaLess10 = 0;
    int counter = 10000000;
    for(int i = 0; i < counter; i++) {
        currentTime = timer.GetTime().ToUSec();
        long long delta = currentTime - prevTime;
        if(delta < 10) {
            deltaLess10++;
        }
        else if(delta < 100) {
            deltaGreat10++;
        }
        else if(delta < 1000) {
            deltaGreat100++;
        }
        else if(delta < 10000) {
            deltaGreat1000++;
        }
        else {
            deltaGreat10000++;
        }
        prevTime = timer.GetTime().ToUSec();
    }
    double percent10000 = deltaGreat10000 / (double)counter;
    double percent1000 = deltaGreat1000 / (double)counter;
    double percent100 = deltaGreat100 / (double)counter;
    double percent10 = deltaGreat10 / (double)counter;
    double percentLess10 = deltaLess10 / (double)counter;
    return 0;
    */
    //SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    QApplication app(argc, argv);
    Voltmeter_Window *window = new Voltmeter_Window;
    window->show();
    int retValue = app.exec();
    return retValue;
}