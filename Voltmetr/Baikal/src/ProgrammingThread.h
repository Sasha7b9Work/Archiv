//  [4/29/2010 Sasha]
#pragma once
#include "defines.h"
#include "Timer.h"

// ���� ��������, ���������� �������� ���������
enum TypeControlSignal {
    TCS_SetValues,        // ���������� �������� �����
	TCS_Stop
};

class ProgrammingThread : public QThread {
    Q_OBJECT
public:
    ProgrammingThread(QObject *parent = 0);
    ~ProgrammingThread();

    void StartProgram();                    // ������ ���������
    void PauseProgram(bool pause);          // ���������� �� �����
    void StopProgram();                     // ��������� ���������
	Time GetTime();							// ���������� ������� ����� ������ ���������

protected:
    void run();

signals:
    // ������ ����������. �������� ������� ��������� 
    // numRow - ����� ������ ������������
    void ControlSignal(/* TypeControlSignal */ int type, int voltageRel, int amperageRel, int numRow);  

private slots:
    void OnTimer();

private:
    Timer timer;        // ������ ������� �������
    QTimer *clockTimer;  // ��� �������� ������
    int currentStep;    // ������� ��� ��������� (������ ������� settingsProg)
};