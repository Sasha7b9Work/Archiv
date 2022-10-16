#include "defines.h"
#include "WindowVoltimetr.h"

int main(int argc, char *argv[]) {
    LOG("Start");
    QApplication app(argc, argv);
    WindowVoltimetr *window = new WindowVoltimetr;
    window->show();
    window->move(100, 100);
    return app.exec();
}