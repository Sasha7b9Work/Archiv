//  [7/7/2009 Sasha] 
#pragma once
/*
Класс описывает круговой стек - при помещении очередного значения в него, если стек
заполнен, из него выталкивается элемент, помещавщийся самым первым, он сдвигается на
один элемент в начало и в конец заталкивается последний элемент
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

    // Сброс стека - ему присваивается размер size и всем элементам присваиваются нулевые значения
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
        // Если в стеке есть свободное место
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
            // Если стек выбран, возвращаем 0
            return 0;
        }
    };

    // Возвращает среднее значение элементов стека
    T GetAverage() {
        T retValue = SumArray<T>(stack, numElements);
        retValue = retValue / numElements;
        return retValue;
    };

    bool IsEmty() { return numElements == 0; };

private:
    int size;
    int numElements;    // Число элементов в стеке
    int position;       // Указывает всегда на место в стеке за последним помещённым в него элементом (при заполненном стеке position = size)
    T *stack;
};
