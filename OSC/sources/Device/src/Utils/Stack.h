#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Stack
{
public:
    Stack(uint size);
    ~Stack();
    void Push(T elem);
    T Pop();
    uint Size() const;
    bool Empty() const;
    /// ���������� ���������� ����� �� ��� �����
    int NumFirstZeros() const;

private:
    T *buffer;
    uint size;
    uint numElements;
};
