#pragma once

#include "ArraySequence.hpp"

template<class T>
ArraySequence<T>::ArraySequence()
{
    this->items = new DynamicArray<T>(0);
}

template<class T>
ArraySequence<T>::ArraySequence(T* items, int count)
{
    this->items = new DynamicArray<T>(items, count);
}

template<class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& sequence)
{
    this->items = new DynamicArray<T>(*sequence.items);
}

template<class T>
ArraySequence<T>::~ArraySequence()
{
    delete this->items;
}

template<class T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence<T>& sequence)
{
    if (this == &sequence) {
        return *this;
    }

    DynamicArray<T>* copiedItems = new DynamicArray<T>(*sequence.items);
    delete this->items;
    this->items = copiedItems;

    return *this;
}

template<class T>
void ArraySequence<T>::ValidateNotEmpty()
{
    if (this->GetLength() == 0) {
        throw OutOfRange("Sequence is empty");
    }
}

template<class T>
void ArraySequence<T>::ValidateSubsequenceRange(int startIndex, int endIndex)
{
    if (startIndex < 0) {
        throw InvalidArgument("Start index cannot be negative");
    }

    if (endIndex < 0) {
        throw InvalidArgument("End index cannot be negative");
    }

    if (startIndex > endIndex) {
        throw InvalidArgument("Start index cannot be greater than end index");
    }

    if (endIndex >= this->GetLength()) {
        throw OutOfRange("Index is out of range");
    }
}

template<class T>
void ArraySequence<T>::ValidateInsertIndex(int index)
{
    if (index < 0) {
        throw InvalidArgument("Index cannot be negative");
    }

    if (index > this->GetLength()) {
        throw OutOfRange("Index is out of range");
    }
}

template<class T>
T ArraySequence<T>::GetFirst()
{
    this->ValidateNotEmpty();
    return this->items->Get(0);
}

template<class T>
T ArraySequence<T>::GetLast()
{
    this->ValidateNotEmpty();
    return this->items->Get(this->GetLength() - 1);
}

template<class T>
T ArraySequence<T>::Get(int index)
{
    return this->items->Get(index);
}

template<class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(int startIndex, int endIndex)
{
    this->ValidateSubsequenceRange(startIndex, endIndex);

    ArraySequence<T>* result = new ArraySequence<T>();

    try {
        for (int index = startIndex; index <= endIndex; ++index) {
            result->Append(this->items->Get(index));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
int ArraySequence<T>::GetLength()
{
    return this->items->GetSize();
}

template<class T>
ArraySequence<T>* ArraySequence<T>::Append(T item)
{
    int length = this->GetLength();
    this->items->Resize(length + 1);
    this->items->Set(length, item);

    return this;
}

template<class T>
ArraySequence<T>* ArraySequence<T>::Prepend(T item)
{
    return this->InsertAt(item, 0);
}

template<class T>
ArraySequence<T>* ArraySequence<T>::InsertAt(T item, int index)
{
    this->ValidateInsertIndex(index);

    int length = this->GetLength();
    this->items->Resize(length + 1);

    for (int currentIndex = length; currentIndex > index; --currentIndex) {
        this->items->Set(currentIndex, this->items->Get(currentIndex - 1));
    }

    this->items->Set(index, item);
    return this;
}

template<class T>
ArraySequence<T>* ArraySequence<T>::Concat(Sequence<T>* list)
{
    if (list == nullptr) {
        throw InvalidArgument("Sequence cannot be null");
    }

    int listLength = list->GetLength();

    for (int index = 0; index < listLength; ++index) {
        this->Append(list->Get(index));
    }

    return this;
}
