#pragma once
#include "defines.h"
#include "Tester/Tester.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (Tester::*pFuncTester)();

class FSMC
{
public:
    FSMC() : mode(ModeNone), interchangeWithPanel(false), funcAfterInteractionWithPanel(0) {};

    void Init();

    void WriteToPanel(uint8 *data, int length);

    void WriteToFPGA8(uint8 *address, uint8 value);

    void WriteToFPGA16(uint8 *address, uint16 value);

    uint8 ReadFromFPGA(uint8 *address);
    /// Возвращает true, если идёт обмен с панелью
    bool InterchangeWithPanel();
    /// Установить функцию, которую нужно выполнить единоразово после завершения обмена с панелью
    void RunFunctionAfterInteractionWitchPanel(pFuncTester func);

private:

    enum ModeFSMC
    {
        ModeNone,
        ModePanelWrite,
        ModePanelRead,
        ModeFPGA
    };
   
    void ReadByte();

    void ConfigureForFPGA();
    /// Расставляет биты значенния data по ножкам D0...D7
    void SetOutData(uint8 data);
    /// Возвращает значение с ножек D0...D7
    uint8 GetOutData();
    /// Возвращает состояние PAN_0, PAN_1
    uint8 ReadPAN();

    ModeFSMC mode;

    bool interchangeWithPanel;
    /// Если не равно нулю, нужно выполнить эту функцию после завершения обмена с панелью
    pFuncTester funcAfterInteractionWithPanel;
};


extern FSMC fsmc;
