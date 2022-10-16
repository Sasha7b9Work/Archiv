//  [7/7/2009 Sasha] 
#pragma once
/*
����� ��������� �������� ���� - ��� ��������� ���������� �������� � ����, ���� ����
��������, �� ���� ������������� �������, ������������ ����� ������, �� ���������� ��
���� ������� � ������ � � ����� ������������� ��������� �������
*/

#include "defines.h"
#include "GlobalFunctions.h"

using namespace NS_GlobalFunctions;

template <class T> 
class CircularStack {
public:
    CircularStack() :
        stack(NULL),
        size(0),
        numElements(0)
        {
        Reset(0);
    };

    CircularStack(const int size) {
        stack = NULL;
        Reset(size);
    };

    ~CircularStack() {
        SAFE_DELETE_ARRAY(stack);
    };

    // ����� ����� - ��� ������������� ������ size � ���� ��������� ������������� ������� ��������
    void Reset(const int size) {
        SAFE_DELETE_ARRAY(stack);
        try {
            stack = new T[size];
        } catch (...) {
            stack = NULL;
            //LOG("Error in operator new");
        }
        this->size = size;
        position = 0;
        numElements = 0;
    };

    void Push(const T value) {
        // ���� � ����� ���� ��������� �����
        if(position == size) {
            position = 0;
        }
        stack[position] = value;
        position++;
        numElements++;
        if(numElements > size) {
            numElements = size;
        }
   
    };

    T Pop() {
        if(position > 0) {
            return stack[--position];
            numElements--;
        }
        else {
            // ���� ���� ������, ���������� 0
            return 0;
        }
    };

    // ���������� ������� �������� ��������� �����
    T GetAverage() {
        T retValue = SumArray<T>(stack, numElements);
        retValue = retValue / numElements;
        return retValue;
    };

    bool IsEmty() { return numElements == 0; };

private:
    int size;
    int numElements;    // ����� ��������� � �����
    int position;       // ��������� ������ �� ����� � ����� �� ��������� ���������� � ���� ��������� (��� ����������� ����� position = size)
    T *stack;
};
