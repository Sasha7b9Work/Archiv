//  [8/28/2009 Sasha]
#pragma once
#pragma warning(disable:4127)
#include <QColor>
#pragma warning(default:4127)

namespace NS_Configurator {
	enum PARAMETER {
        P_SynhroChannel,
        P_Range0,
        P_Range1,
        P_Palm0,
        P_Palm1,
        P_PalmNum,
		P_RShift0,
        P_RShift1,
        P_ModeInput0,
        P_ModeInput1,
        P_POS0,
        P_NEG0,
        P_DIS0,
        P_POS1,
        P_NEG1,
        P_DIS1,
        P_TRIG_LEV0,
        P_TRIG_LEV1,
        P_START_MODE,
        P_Start,
        P_SynhroFront,
        P_FiltrSynhro,
        P_TBase,
        P_IndexCBFreq0,     // В вольтметре индексы комбобоксов диапазона частот
        P_IndexCBFreq1,     // Как, впрочем, и в спектроанализаторе
        P_TPos,
        P_TShift,
        P_CurrentCursors,
        P_DrawMode,
        P_CurrentTab,
        P_AdditionRShift0,      // Дополнительное смещение
        P_AdditionRShift1,
        P_PositionWindowX,      // Координаты окна на рабочем столе
        P_PositionWindowY,
        P_CBWindowAnalyzer,     // Индекс комбобокса с окна умножения на спектр
        P_CBKoeffRazv,          // Индекс комбобокса коэффициента развёртки для регистратора
        P_CBLengthRecord,       // Индекс комбобокса длины записи для регистратора
        P_NumberParameters
	};

    enum COLOR_PARAMETER
    {
        CP_Background,
        CP_Grid,
        CP_Signal0,
        CP_Signal1,
        CP_Signal2,
        CP_Curs0,
        CP_Curs1,
        CP_Curs2,
        CP_TPos,
        CP_TShift,
        CP_NumberColorParameters
    };
}

class Configurator {
public:
    Configurator(const char * const _nameFile);
    bool IsExist();             // Возвращает true, если существует файл конфигурации. Одновременно заполняет массив data из существующего файла
    // Считывает значение параметра из файла конфигурации.
    int LoadParameter(NS_Configurator::PARAMETER param);     // Считывает параметр. Если требуемый параметр имеет булевый тип, его нужно привести к bool

    bool CreateConfig();        // Вызывается перед сохранением настроек - создаёт конфигурационный файл
    // Записывает значение параметра в файл конфигурации. Если запись невозможна, возвращает false
    bool SaveParameter(NS_Configurator::PARAMETER param, int value);
    bool SaveParameter(NS_Configurator::PARAMETER param, bool value);
    bool SaveColor(NS_Configurator::COLOR_PARAMETER param, const QColor& color);
    QColor LoadColor(NS_Configurator::COLOR_PARAMETER);
    bool CloseConfig();         // Вызывается после сохранения настроек

private:
    int data[NS_Configurator::P_NumberParameters];
    QColor colors[NS_Configurator::CP_NumberColorParameters];
    const char * const nameFile;

    Configurator& operator=(Configurator&) {}
};