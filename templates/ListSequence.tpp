#pragma once

#include "ListSequence.hpp"

template<class T>
ListSequence<T>::ListSequence()
{
    this->items = new LinkedList<T>();
}

template<class T>
ListSequence<T>::ListSequence(T* items, int count)
{
    this->items = new LinkedList<T>(items, count);
}

template<class T>
ListSequence<T>::ListSequence(const ListSequence<T>& sequence)
{
    this->items = new LinkedList<T>(*sequence.items);
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

    LinkedList<T>* newItems = new LinkedList<T>(*sequence.items);
    delete this->items;
    this->items = newItems;
    return *this;
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
    ListSequence<T>* result = new ListSequence<T>();

    try {
        for (int index = 0; index < subList->GetLength(); ++index) {
            result->Append(subList->Get(index));
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
ListSequence<T>* ListSequence<T>::Append(T item)
{
    this->items->Append(item);
    return this;
}

template<class T>
ListSequence<T>* ListSequence<T>::Prepend(T item)
{
    this->items->Prepend(item);
    return this;
}

template<class T>
ListSequence<T>* ListSequence<T>::InsertAt(T item, int index)
{
    this->items->InsertAt(item, index);
    return this;
}

template<class T>
ListSequence<T>* ListSequence<T>::Concat(Sequence<T>* list)
{
    if (list == nullptr) {
        throw InvalidArgument("Sequence cannot be null");
    }

    int length = list->GetLength();

    for (int index = 0; index < length; ++index) {
        this->items->Append(list->Get(index));
    }

    return this;
}
