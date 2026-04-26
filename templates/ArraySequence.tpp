#pragma once

#include "ArraySequence.hpp"

template<class T>
ArraySequence<T>::ArraySequence()
{
    this->items = new DynamicArray<T>(0);
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
void ArraySequence<T>::SetItems(T* items, int count)
{
    DynamicArray<T>* newItems = new DynamicArray<T>(items, count);
    delete this->items;
    this->items = newItems;
}

template<class T>
void ArraySequence<T>::CopyItems(const ArraySequence<T>& sequence)
{
    DynamicArray<T>* copiedItems = new DynamicArray<T>(*sequence.items);
    delete this->items;
    this->items = copiedItems;
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
void ArraySequence<T>::AppendInternal(T item)
{
    int length = this->GetLength();
    this->items->Resize(length + 1);
    this->items->Set(length, item);
}

template<class T>
void ArraySequence<T>::InsertAtInternal(T item, int index)
{
    this->ValidateInsertIndex(index);

    int length = this->GetLength();
    this->items->Resize(length + 1);

    for (int currentIndex = length; currentIndex > index; --currentIndex) {
        this->items->Set(currentIndex, this->items->Get(currentIndex - 1));
    }

    this->items->Set(index, item);
}

template<class T>
void ArraySequence<T>::ConcatInternal(Sequence<T>* list)
{
    if (list == nullptr) {
        throw InvalidArgument("Sequence cannot be null");
    }

    int listLength = list->GetLength();

    for (int index = 0; index < listLength; ++index) {
        this->AppendInternal(list->Get(index));
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

    ArraySequence<T>* result = this->CreateEmpty();

    try {
        for (int index = startIndex; index <= endIndex; ++index) {
            result->AppendInternal(this->items->Get(index));
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
Sequence<T>* ArraySequence<T>::Append(T item)
{
    ArraySequence<T>* result = this->Instance();
    result->AppendInternal(item);
    return result;
}

template<class T>
Sequence<T>* ArraySequence<T>::Prepend(T item)
{
    return this->InsertAt(item, 0);
}

template<class T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, int index)
{
    ArraySequence<T>* result = this->Instance();
    result->InsertAtInternal(item, index);
    return result;
}

template<class T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* list)
{
    ArraySequence<T>* result = this->Instance();
    result->ConcatInternal(list);
    return result;
}

template<class T>
MutableArraySequence<T>::MutableArraySequence() {}

template<class T>
MutableArraySequence<T>::MutableArraySequence(T* items, int count)
{
    this->SetItems(items, count);
}

template<class T>
MutableArraySequence<T>::MutableArraySequence(const MutableArraySequence<T>& sequence)
{
    this->CopyItems(sequence);
}

template<class T>
MutableArraySequence<T>& MutableArraySequence<T>::operator=(const MutableArraySequence<T>& sequence)
{
    this->ArraySequence<T>::operator=(sequence);
    return *this;
}

template<class T>
ArraySequence<T>* MutableArraySequence<T>::Instance()
{
    return this;
}

template<class T>
ArraySequence<T>* MutableArraySequence<T>::CreateEmpty()
{
    return new MutableArraySequence<T>();
}

template<class T>
ImmutableArraySequence<T>::ImmutableArraySequence() {}

template<class T>
ImmutableArraySequence<T>::ImmutableArraySequence(T* items, int count)
{
    this->SetItems(items, count);
}

template<class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& sequence)
{
    this->CopyItems(sequence);
}

template<class T>
ImmutableArraySequence<T>& ImmutableArraySequence<T>::operator=(const ImmutableArraySequence<T>& sequence)
{
    this->ArraySequence<T>::operator=(sequence);
    return *this;
}

template<class T>
ArraySequence<T>* ImmutableArraySequence<T>::Instance()
{
    return new ImmutableArraySequence<T>(*this);
}

template<class T>
ArraySequence<T>* ImmutableArraySequence<T>::CreateEmpty()
{
    return new ImmutableArraySequence<T>();
}
