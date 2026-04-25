#pragma once

#include "Exceptions.hpp"

template<class T>
class DynamicArray
{
    private:
        T* data;
        int size;

    public:
        DynamicArray(T* items, int count);
        DynamicArray(int size);
        DynamicArray(const DynamicArray<T>& dynamicArray);
        ~DynamicArray();

        DynamicArray<T>& operator=(const DynamicArray<T>& dynamicArray);

        T Get(int index);
        int GetSize();

        void Set(int index, T value);
        void Resize(int newSize);
};

#include "DynamicArray.tpp"
