#pragma once

#include "DynamicArray.hpp"

template<class T>
DynamicArray<T>::DynamicArray(T* items, size_t count)
{
    if (count < 0) throw InvalidArgument("Count cannot be negative");

    if (count == 0) {
        this->data = nullptr;
        this->size = 0;
        return;
    }

    if (items == nullptr) throw InvalidArgument("Items cannot be null when count is positive");

    this->data = new T[count];
    this->size = count;

    for (size_t i = 0; i < this->size; i++) {
        this->data[i] = items[i];
    }
}

template<class T>
DynamicArray<T>::DynamicArray(size_t size)
{
    if (size < 0) throw InvalidArgument("Size cannot be negative");

    if (size == 0) {
        this->data = nullptr;
        this->size = 0;
        return;
    }

    this->data = new T[size];
    this->size = size;
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray)
{
    this->size = dynamicArray.size;

    if (this->size == 0) {
        this->data = nullptr;
        return;
    }

    this->data = new T[this->size];

    for (size_t i = 0; i < this->size; i++) {
        this->data[i] = dynamicArray.data[i];
    }
}

template<class T>
DynamicArray<T>::~DynamicArray()
{
    delete[] this->data;
}

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& dynamicArray)
{
    if (this == &dynamicArray) {
        return *this;
    }

    T* newData = nullptr;

    if (dynamicArray.size > 0) {
        newData = new T[dynamicArray.size];

        for (size_t i = 0; i < dynamicArray.size; i++) {
            newData[i] = dynamicArray.data[i];
        }
    }

    delete[] this->data;

    this->data = newData;
    this->size = dynamicArray.size;

    return *this;
}

template<class T>
T& DynamicArray<T>::operator[](size_t index)
{
    if (index >= size) throw OutOfRange("Index out of bounds");
    return data[index];
}

template<class T>
const T& DynamicArray<T>::operator[](size_t index) const
{
    if (index >= size) throw OutOfRange("Index out of bounds");
    return data[index];
}

template<class T>
T DynamicArray<T>::Get(size_t index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");
    if (index >= this->size) throw OutOfRange("Index is out of range");

    return this->data[index];
}

template<class T>
size_t DynamicArray<T>::GetSize()
{
    return this->size;
}

template<class T>
void DynamicArray<T>::Set(size_t index, T value)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");
    if (index >= this->size) throw OutOfRange("Index is out of range");

    this->data[index] = value;
}

template<class T>
void DynamicArray<T>::Resize(size_t newSize)
{
    if (newSize < 0) throw InvalidArgument("Size cannot be negative");

    if (newSize == 0) {
        delete[] this->data;
        this->data = nullptr;
        this->size = 0;
        return;
    }

    T* newData = new T[newSize];

    size_t elementsToCopy = this->size < newSize ? this->size : newSize;

    for (size_t i = 0; i < elementsToCopy; i++) {
        newData[i] = this->data[i];
    }

    delete[] this->data;

    this->data = newData;
    this->size = newSize;
}
