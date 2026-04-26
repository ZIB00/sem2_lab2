#pragma once

#include "AdaptiveSequence.hpp"

template<class T>
AdaptiveSequence<T>::AdaptiveSequence()
{
    this->sequence = new MutableArraySequence<T>();
    this->getCount = 0;
    this->insertCount = 0;
}

template<class T>
AdaptiveSequence<T>::AdaptiveSequence(T* items, int count)
{
    this->sequence = new MutableArraySequence<T>(items, count);
    this->getCount = 0;
    this->insertCount = 0;
}

template<class T>
AdaptiveSequence<T>::AdaptiveSequence(const AdaptiveSequence<T>& other)
{
    this->sequence = nullptr;
    this->CopyFrom(other);
}

template<class T>
AdaptiveSequence<T>::~AdaptiveSequence()
{
    delete this->sequence;
}

template<class T>
AdaptiveSequence<T>& AdaptiveSequence<T>::operator=(const AdaptiveSequence<T>& other)
{
    if (this == &other) {
        return *this;
    }

    this->CopyFrom(other);
    return *this;
}

template<class T>
void AdaptiveSequence<T>::CopyFrom(const AdaptiveSequence<T>& other)
{
    Sequence<T>* copiedSequence = nullptr;

    if (dynamic_cast<MutableListSequence<T>*>(other.sequence) != nullptr) {
        copiedSequence = new MutableListSequence<T>();
    }
    else {
        copiedSequence = new MutableArraySequence<T>();
    }

    try {
        for (int index = 0; index < other.sequence->GetLength(); ++index) {
            copiedSequence->Append(other.sequence->Get(index));
        }
    }
    catch (...) {
        delete copiedSequence;
        throw;
    }

    delete this->sequence;
    this->sequence = copiedSequence;
    this->getCount = other.getCount;
    this->insertCount = other.insertCount;
}

template<class T>
void AdaptiveSequence<T>::SwitchToArraySequence()
{
    if (dynamic_cast<MutableArraySequence<T>*>(this->sequence) != nullptr) {
        return;
    }

    Sequence<T>* newSequence = new MutableArraySequence<T>();

    try {
        for (int index = 0; index < this->sequence->GetLength(); ++index) {
            newSequence->Append(this->sequence->Get(index));
        }
    }
    catch (...) {
        delete newSequence;
        throw;
    }

    delete this->sequence;
    this->sequence = newSequence;
}

template<class T>
void AdaptiveSequence<T>::SwitchToListSequence()
{
    if (dynamic_cast<MutableListSequence<T>*>(this->sequence) != nullptr) {
        return;
    }

    Sequence<T>* newSequence = new MutableListSequence<T>();

    try {
        for (int index = 0; index < this->sequence->GetLength(); ++index) {
            newSequence->Append(this->sequence->Get(index));
        }
    }
    catch (...) {
        delete newSequence;
        throw;
    }

    delete this->sequence;
    this->sequence = newSequence;
}

template<class T>
void AdaptiveSequence<T>::UpdateStrategy()
{
    if (this->insertCount > this->getCount + 5) {
        this->SwitchToListSequence();
    }
    else if (this->getCount > this->insertCount + 5) {
        this->SwitchToArraySequence();
    }
}

template<class T>
T AdaptiveSequence<T>::GetFirst()
{
    this->getCount++;
    this->UpdateStrategy();
    return this->sequence->GetFirst();
}

template<class T>
T AdaptiveSequence<T>::GetLast()
{
    this->getCount++;
    this->UpdateStrategy();
    return this->sequence->GetLast();
}

template<class T>
T AdaptiveSequence<T>::Get(int index)
{
    this->getCount++;
    this->UpdateStrategy();
    return this->sequence->Get(index);
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::GetSubsequence(int startIndex, int endIndex)
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

    AdaptiveSequence<T>* result = new AdaptiveSequence<T>();

    if (dynamic_cast<MutableListSequence<T>*>(this->sequence) != nullptr) {
        result->SwitchToListSequence();
    }

    try {
        for (int index = startIndex; index <= endIndex; ++index) {
            result->sequence->Append(this->sequence->Get(index));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
int AdaptiveSequence<T>::GetLength()
{
    return this->sequence->GetLength();
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::Append(T item)
{
    this->insertCount++;
    this->UpdateStrategy();
    this->sequence->Append(item);
    return this;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::Prepend(T item)
{
    this->insertCount++;
    this->UpdateStrategy();
    this->sequence->Prepend(item);
    return this;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::InsertAt(T item, int index)
{
    this->insertCount++;
    this->UpdateStrategy();
    this->sequence->InsertAt(item, index);
    return this;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::Concat(Sequence<T>* list)
{
    this->insertCount += list == nullptr ? 0 : list->GetLength();
    this->UpdateStrategy();
    this->sequence->Concat(list);
    return this;
}
