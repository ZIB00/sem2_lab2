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
void ArraySequence<T>::SetItems(T* items, size_t count)
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
    if (this->GetLength() == 0) throw OutOfRange("Sequence is empty");
}

template<class T>
void ArraySequence<T>::ValidateSubsequenceRange(size_t startIndex, size_t endIndex)
{
    if (startIndex < 0) throw InvalidArgument("Start index cannot be negative");
    if (endIndex < 0) throw InvalidArgument("End index cannot be negative");
    if (startIndex > endIndex) throw InvalidArgument("Start index cannot be greater than end index");
    if (endIndex >= this->GetLength()) throw OutOfRange("Index is out of range");
}

template<class T>
void ArraySequence<T>::ValidateInsertIndex(size_t index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");
    if (index > this->GetLength()) throw OutOfRange("Index is out of range");
}

template<class T>
void ArraySequence<T>::Appendsize_ternal(T item)
{
    size_t length = this->GetLength();
    this->items->Resize(length + 1);
    this->items->Set(length, item);
}

template<class T>
void ArraySequence<T>::InsertAtsize_ternal(T item, size_t index)
{
    this->ValidateInsertIndex(index);

    size_t length = this->GetLength();
    this->items->Resize(length + 1);

    for (size_t currentIndex = length; currentIndex > index; --currentIndex) {
        this->items->Set(currentIndex, this->items->Get(currentIndex - 1));
    }

    this->items->Set(index, item);
}

template<class T>
void ArraySequence<T>::Concatsize_ternal(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    size_t listLength = list->GetLength();

    for (size_t index = 0; index < listLength; ++index) {
        this->Appendsize_ternal(list->Get(index));
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
T ArraySequence<T>::Get(size_t index)
{
    return this->items->Get(index);
}

template<class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex)
{
    this->ValidateSubsequenceRange(startIndex, endIndex);

    ArraySequence<T>* result = this->CreateEmpty();

    try {
        for (size_t index = startIndex; index <= endIndex; ++index) {
            result->Appendsize_ternal(this->items->Get(index));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
size_t ArraySequence<T>::GetLength()
{
    return this->items->GetSize();
}

template<class T>
Sequence<T>* ArraySequence<T>::Append(T item)
{
    ArraySequence<T>* result = this->Instance();
    result->Appendsize_ternal(item);
    return result;
}

template<class T>
Sequence<T>* ArraySequence<T>::Prepend(T item)
{
    return this->InsertAt(item, 0);
}

template<class T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, size_t index)
{
    ArraySequence<T>* result = this->Instance();
    result->InsertAtsize_ternal(item, index);
    return result;
}

template<class T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* list)
{
    ArraySequence<T>* result = this->Instance();
    result->Concatsize_ternal(list);
    return result;
}

template<class T>
template<class T2>
Sequence<T>* ArraySequence<T>::Map(T2 (*function)(T))
{
    if (function == nullptr) throw InvalidArgument("Function function cannot be null");

    Sequence<T>* result = nullptr;

    if (dynamic_cast<ImmutableArraySequence<T>*>(this) != nullptr) {
        result = new ImmutableArraySequence<T>();
    }
    else {
        result = new MutableArraySequence<T>();
    }

    try {
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
            Sequence<T>* updated = result->Append(function(this->items->Get(index)));
            if (updated != result) {
                delete result;
                result = updated;
            }
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<T>* ArraySequence<T>::Where(bool (*predicate)(T))
{
    if (predicate == nullptr) throw InvalidArgument("Predicate function cannot be null");

    ArraySequence<T>* result = this->CreateEmpty();

    try {
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
            T value = this->items->Get(index);

            if (predicate(value)) {
                result->Appendsize_ternal(value);
            }
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
template<class T2>
T ArraySequence<T>::Reduce(T2 (*function)(T2, T))
{
    if (function == nullptr) throw InvalidArgument("Function function cannot be null");

    this->ValidateNotEmpty();
    T result = this->items->Get(0);
    size_t length = this->GetLength();

    for (size_t index = 1; index < length; ++index) {
        result = function(result, this->items->Get(index));
    }

    return result;
}

template<class T>
MutableArraySequence<T>::MutableArraySequence() {}

template<class T>
MutableArraySequence<T>::MutableArraySequence(T* items, size_t count)
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
ImmutableArraySequence<T>::ImmutableArraySequence(T* items, size_t count)
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
