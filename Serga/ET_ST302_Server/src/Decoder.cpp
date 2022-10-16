#include "Decoder.h"

/*
    ‘ормат сообщени€
    0xff, 0xfe - стартовые байты
    0xxx - код команды
    ...
    ...  - параметры команды
    ...
    0xfe, 0xff - конечные байты

    //////////////////////////////////////////////////////////////////////////
    0x00 - нажата/отпущена кнопка
    0xxx - id кнопки (см. enum Button)
    0x01/0x10 - нажата/отпущена

    //////////////////////////////////////////////////////////////////////////
    0x01 - поворот регул€тора влево/вправо
    0xxx - id регул€тора (см. enum Regulator)
    0x01/0x10 - влево/вправо
*/

/*
    ѕоступившие данные сохран€ютс€ в буфере. ≈сли возможно, выбираетс€ очередна€ команда, декодируетс€ и посылаетс€ адресату
*/

static const int SIZE_BUFFER = 1024;
static char buffer[SIZE_BUFFER];
static int pointer = 0;

void Decoder_WriteToBuffer(const char *_buffer, uint _length) {

}

void Decoder_ProcessingMessage() {

}

void Decoder_ProcessingData(const char *_buffer, uint _length) {
    Decoder_WriteToBuffer(_buffer, _length);
    Decoder_ProcessingMessage();
}