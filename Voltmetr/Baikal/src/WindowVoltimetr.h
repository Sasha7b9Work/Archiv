//  [3/1/2010 Sasha]
#pragma once
#pragma warning(disable:4127)
#include "ui_windowvoltimetr.h"
#pragma warning(default:4127)
#include "Display.h"
#include "Holder.h"
#include "Device.h"
#include "Timer.h"
#include "Configurator.h"
#include "SettingsDialog.h"

#define TIME_UPDATE 100

class WindowVoltimetr:public QMainWindow, public Ui::WindowVoltimetr {
    Q_OBJECT
public:
    WindowVoltimetr(QMainWindow *parent = 0);

private slots:
    void ChangeHolderU(int value);
    void ChangeHolderI(int value);
    void UpdatePorts();
    bool SetPort(int);
    void StartDevice();
    void StopDevice();
    void OnTimer();
    void SetExactly();
    void EnableDevice(bool);
    void EnableManualMode();
    void EnableAutoMode();
    void StartProgramDevice(bool);
    void PauseProgramDevice(bool);
    void close();
    void ShowDialogSettings();
    void SetColorDisplay();
    void SetColorSimbols();
    void ChangeDirPrograms();
    void ChangeDirDatas();
    void SaveProgram();
    void LoadProgram();
    void OnControlProgram(int, int, int, int);      // Слот для автоматического управления
	void DeleteRow();
	void InsertRow();

protected:
    void closeEvent(QCloseEvent *);

private:
    SettingsDialog settingsDialog;
    Display *displayUSet;
    Display *displayISet;
    Display *displayUMeas;
    Display *displayIMeas;
    Display *displays[4];
    Holder *holderU;
    Holder *holderI;
    Device device;
    QTimer timer;               // Основной таймер. Через промежутки времени, отсчитываемые им, будет производиться опрос прибора
    Configurator configurator;
    QFile *fileData;            // Указатель на файл с данными
    bool recordData;            // Установленное в true значение означает, что нужно происзводить запись данных в файл
	int currentRow;				// Здесь хранится текущая строка для автоматического режима

    void ConfigureWidgets();
    QString& VoltageToString(QString &voltageString);       // Дополняет строку напряжения символами "U = " и "V"
    QString& AmperageToString(QString &amperageString);     // Дополняет строку напряжения символами "I = " и "A"
    void LoadSettings();
    void SaveSettings();
    void ConnectingSlots();
    void CreateDataFile(bool create);           // Создать файл для записи данных, если create == true, и удалить, если create == false
	void CreateContextMenu();
    QString progrExt[2];                        // Расширение файла с программой для нулевой и первой модификации
};