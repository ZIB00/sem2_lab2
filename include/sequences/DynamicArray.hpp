#pragma once

#include "Exceptions.hpp"

template<class T>
class DynamicArray
{
    private:
        T* data;
        size_t size;

    public:
        DynamicArray(T* items, size_t count);
        DynamicArray(size_t size);
        DynamicArray(const DynamicArray<T>& dynamicArray);
        DynamicArray(std::initializer_list<T> items);
        ~DynamicArray();

        DynamicArray<T>& operator=(const DynamicArray<T>& dynamicArray);
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        DynamicArray<T>* operator+(const DynamicArray<T>* other);
        bool operator==(const DynamicArray<T>* other);
        bool operator!=(const DynamicArray<T>* other);

        T Get(size_t index);
        size_t GetSize();

        void Set(size_t index, T value);
        void Resize(size_t newSize);
};

#include "DynamicArray.tpp"
