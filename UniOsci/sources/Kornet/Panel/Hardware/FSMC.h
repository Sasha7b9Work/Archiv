#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FSMC
{
public:
    FSMC() : inReadState(false) { };

    void Init();

    uint8 ReadByte();
    /// �������� ����� �� ����. ���� ��������� ����� ����, ��������� ������ �� ���������� ��-�� ����, ��� ��� ������� ����
    void WriteBuffer(uint8 *data = 0, int length = 0);

private:

    /// ���� ��������� � ����� ������ ������
    void ConfigToRead();
    /// ���� ��������� � ����� �������� ������
    void ConfigToWrite();
    /// ���������� true, ���� ������ RD FPGA ��������� � ������� ���������
    bool inReadState;
};


extern FSMC fsmc;
