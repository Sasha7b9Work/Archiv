#include "Decoder.h"

/*
    ������ ���������
    0xff, 0xfe - ��������� �����
    0xxx - ��� �������
    ...
    ...  - ��������� �������
    ...
    0xfe, 0xff - �������� �����

    //////////////////////////////////////////////////////////////////////////
    0x00 - ������/�������� ������
    0xxx - id ������ (��. enum Button)
    0x01/0x10 - ������/��������

    //////////////////////////////////////////////////////////////////////////
    0x01 - ������� ���������� �����/������
    0xxx - id ���������� (��. enum Regulator)
    0x01/0x10 - �����/������
*/

/*
    ����������� ������ ����������� � ������. ���� ��������, ���������� ��������� �������, ������������ � ���������� ��������
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