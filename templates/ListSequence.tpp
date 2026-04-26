#pragma once

#include "ListSequence.hpp"

template<class T>
ListSequence<T>::ListSequence()
{
    this->items = new LinkedList<T>();
}

template<class T>
ListSequence<T>::~ListSequence()
{
    delete this->items;
}

template<class T>
ListSequence<T>& ListSequence<T>::operator=(const ListSequence<T>& sequence)
{
    if (this == &sequence) {
        return *this;
    }

    LinkedList<T>* copiedItems = new LinkedList<T>(*sequence.items);
    delete this->items;
    this->items = copiedItems;
    return *this;
}

template<class T>
void ListSequence<T>::SetItems(T* items, int count)
{
    LinkedList<T>* newItems = new LinkedList<T>(items, count);
    delete this->items;
    this->items = newItems;
}

template<class T>
void ListSequence<T>::CopyItems(const ListSequence<T>& sequence)
{
    LinkedList<T>* copiedItems = new LinkedList<T>(*sequence.items);
    delete this->items;
    this->items = copiedItems;
}

template<class T>
void ListSequence<T>::AppendInternal(T item)
{
    this->items->Append(item);
}

template<class T>
void ListSequence<T>::PrependInternal(T item)
{
    this->items->Prepend(item);
}

template<class T>
void ListSequence<T>::InsertAtInternal(T item, int index)
{
    this->items->InsertAt(item, index);
}

template<class T>
void ListSequence<T>::ConcatInternal(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    int length = list->GetLength();

    for (int index = 0; index < length; ++index) {
        this->AppendInternal(list->Get(index));
    }
}

template<class T>
T ListSequence<T>::GetFirst()
{
    return this->items->GetFirst();
}

template<class T>
T ListSequence<T>::GetLast()
{
    return this->items->GetLast();
}

template<class T>
T ListSequence<T>::Get(int index)
{
    return this->items->Get(index);
}

template<class T>
Sequence<T>* ListSequence<T>::GetSubsequence(int startIndex, int endIndex)
{
    LinkedList<T>* subList = this->items->GetSubList(startIndex, endIndex);
    ListSequence<T>* result = this->CreateEmpty();

    try {
        for (int index = 0; index < subList->GetLength(); ++index) {
            result->AppendInternal(subList->Get(index));
        }

        delete subList;
        return result;
    }
    catch (...) {
        delete result;
        delete subList;
        throw;
    }
}

template<class T>
int ListSequence<T>::GetLength()
{
    return this->items->GetLength();
}

template<class T>
Sequence<T>* ListSequence<T>::Append(T item)
{
    ListSequence<T>* result = this->Instance();
    result->AppendInternal(item);
    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::Prepend(T item)
{
    ListSequence<T>* result = this->Instance();
    result->PrependInternal(item);
    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::InsertAt(T item, int index)
{
    ListSequence<T>* result = this->Instance();
    result->InsertAtInternal(item, index);
    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* list)
{
    ListSequence<T>* result = this->Instance();
    result->ConcatInternal(list);
    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::Map(T (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    Sequence<T>* result = nullptr;

    if (dynamic_cast<ImmutableListSequence<T>*>(this) != nullptr) {
        result = new ImmutableListSequence<T>();
    }
    else {
        result = new MutableListSequence<T>();
    }

    try {
        int length = this->GetLength();

        for (int index = 0; index < length; ++index) {
            Sequence<T>* updated = result->Append(Function(this->items->Get(index)));
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
Sequence<T>* ListSequence<T>::Where(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    ListSequence<T>* result = this->CreateEmpty();

    try {
        int length = this->GetLength();

        for (int index = 0; index < length; ++index) {
            T value = this->items->Get(index);

            if (Function(value)) {
                result->AppendInternal(value);
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
T ListSequence<T>::Reduce(T (*Function)(T, T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    if (this->GetLength() == 0) throw OutOfRange("Sequence is empty");

    T result = this->items->Get(0);
    int length = this->GetLength();

    for (int index = 1; index < length; ++index) {
        result = Function(result, this->items->Get(index));
    }

    return result;
}

template<class T>
MutableListSequence<T>::MutableListSequence()
{
}

template<class T>
MutableListSequence<T>::MutableListSequence(T* items, int count)
{
    this->SetItems(items, count);
}

template<class T>
MutableListSequence<T>::MutableListSequence(const MutableListSequence<T>& sequence)
{
    this->CopyItems(sequence);
}

template<class T>
MutableListSequence<T>& MutableListSequence<T>::operator=(const MutableListSequence<T>& sequence)
{
    this->ListSequence<T>::operator=(sequence);
    return *this;
}

template<class T>
ListSequence<T>* MutableListSequence<T>::Instance()
{
    return this;
}

template<class T>
ListSequence<T>* MutableListSequence<T>::CreateEmpty()
{
    return new MutableListSequence<T>();
}

template<class T>
ImmutableListSequence<T>::ImmutableListSequence()
{
}

template<class T>
ImmutableListSequence<T>::ImmutableListSequence(T* items, int count)
{
    this->SetItems(items, count);
}

template<class T>
ImmutableListSequence<T>::ImmutableListSequence(const ImmutableListSequence<T>& sequence)
{
    this->CopyItems(sequence);
}

template<class T>
ImmutableListSequence<T>& ImmutableListSequence<T>::operator=(const ImmutableListSequence<T>& sequence)
{
    this->ListSequence<T>::operator=(sequence);
    return *this;
}

template<class T>
ListSequence<T>* ImmutableListSequence<T>::Instance()
{
    return new ImmutableListSequence<T>(*this);
}

template<class T>
ListSequence<T>* ImmutableListSequence<T>::CreateEmpty()
{
    return new ImmutableListSequence<T>();
}
