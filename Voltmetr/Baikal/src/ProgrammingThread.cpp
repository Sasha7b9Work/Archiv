//  [4/30/2010 Sasha]
#include "ProgrammingThread.h"
#include "GlobalVars.h"

extern std::vector<SetProg> settingsProg;

//  [4/30/2010 Sasha]
ProgrammingThread::ProgrammingThread(QObject *_parent) : QThread(_parent) {
	clockTimer = new QTimer;
}

//  [4/30/2010 Sasha]
ProgrammingThread::~ProgrammingThread() {
	delete clockTimer;
}

//  [4/30/2010 Sasha]
void ProgrammingThread::run() {
    exec();
}

//  [4/30/2010 Sasha]
void ProgrammingThread::StartProgram() {
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    if(!isRunning()) {
        start(LowPriority);
    }
    currentStep = 0;
    timer.Reset();
    clockTimer->start(10);
    emit ControlSignal((int)TCS_SetValues, settingsProg[0].voltage, settingsProg[0].amperage, 0);
}

//  [4/30/2010 Sasha]
void ProgrammingThread::PauseProgram(bool _pause) {
    if(_pause) {
        timer.Pause();
    }
    else {
        timer.Release();
    }
}

//  [4/30/2010 Sasha]
void ProgrammingThread::StopProgram() {
    clockTimer->stop();
}

//  [5/31/2010 Sasha]
Time ProgrammingThread::GetTime() {
	return timer.GetTime();
}

//  [4/30/2010 Sasha]
void ProgrammingThread::OnTimer() {
    // Если последний шаг программы отработан
	/*
    if((settingsProg.size() - 1) == (size_t)currentStep) {
		emit ControlSignal(TCS_Stop, 0, 0, 0);
        return;     // выходим без всяких действий
    }
	*/
	if(currentStep < ((int)settingsProg.size() - 1)) {
		Time time = timer.GetTime();
		int timeInSecs = (int)(time.ToMSec() / 1000L);
		if((timeInSecs >= settingsProg[currentStep + 1].time) && (settingsProg[currentStep + 1].IsValid())) {
			currentStep++;
			emit ControlSignal(TCS_SetValues, settingsProg[currentStep].voltage, settingsProg[currentStep].amperage, currentStep);
		}
	}
}
