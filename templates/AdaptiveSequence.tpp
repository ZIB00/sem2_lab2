#pragma once

#include "AdaptiveSequence.hpp"

#pragma region sequence main functions

template<class T>
AdaptiveSequence<T>::AdaptiveSequence()
{
    this->sequence = new MutableArraySequence<T>();
    this->getCount = 0;
    this->insertCount = 0;
}

template<class T>
AdaptiveSequence<T>::AdaptiveSequence(T* items, size_t count)
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
        for (size_t index = 0; index < other.sequence->GetLength(); ++index) {
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
        for (size_t index = 0; index < this->sequence->GetLength(); ++index) {
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
        for (size_t index = 0; index < this->sequence->GetLength(); ++index) {
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
T AdaptiveSequence<T>::Get(size_t index)
{
    this->getCount++;
    this->UpdateStrategy();
    return this->sequence->Get(index);
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex)
{
    if (startIndex < 0) throw InvalidArgument("Start index cannot be negative");
    if (endIndex < 0) throw InvalidArgument("End index cannot be negative");
    if (startIndex > endIndex) throw InvalidArgument("Start index cannot be greater than end index");
    if (endIndex >= this->GetLength()) throw OutOfRange("Index is out of range");

    AdaptiveSequence<T>* result = new AdaptiveSequence<T>();

    if (dynamic_cast<MutableListSequence<T>*>(this->sequence) != nullptr) {
        result->SwitchToListSequence();
    }

    try {
        for (size_t index = startIndex; index <= endIndex; ++index) {
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
size_t AdaptiveSequence<T>::GetLength()
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
Sequence<T>* AdaptiveSequence<T>::InsertAt(T item, size_t index)
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

#pragma endregion

#pragma region operators (=, [], const [], +, ==, !=)

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
T& AdaptiveSequence<T>::operator[](size_t index)
{
    return (*this->sequence)[index]; 
}

template<class T>
const T& AdaptiveSequence<T>::operator[](size_t index) const
{
    return (*this->sequence)[index]; 
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::operator+(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Cannot add null sequence");

    auto result = new AdaptiveSequence<T>();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            result->Append((*this)[i]);
        }
        for (size_t i = 0; i < other->GetLength(); ++i) {
            result->Append(other->Get(i));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
bool AdaptiveSequence<T>::operator==(Sequence<T>* other)
{
    if (this == other) return true;
    if (other == nullptr || this->GetLength() != other->GetLength()) return false;

    for (size_t i = 0; i < this->GetLength(); ++i) {
        if ((*this)[i] != other->Get(i)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool AdaptiveSequence<T>::operator!=(Sequence<T>* other)
{
    return !(*this == other);
}

#pragma endregion

#pragma region Mar/Where/Reduce

template<class T>
Sequence<T>* AdaptiveSequence<T>::Map(T (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    AdaptiveSequence<T>* result = new AdaptiveSequence<T>();

    if (dynamic_cast<MutableListSequence<T>*>(this->sequence) != nullptr) {
        result->SwitchToListSequence();
    }

    try {
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
            result->sequence->Append(Function(this->sequence->Get(index)));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::Where(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    AdaptiveSequence<T>* result = new AdaptiveSequence<T>();

    if (dynamic_cast<MutableListSequence<T>*>(this->sequence) != nullptr) {
        result->SwitchToListSequence();
    }

    try {
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
            T value = this->sequence->Get(index);

            if (Function(value)) {
                result->sequence->Append(value);
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
T AdaptiveSequence<T>::Reduce(T (*Function)(T, T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    if (this->GetLength() == 0) throw OutOfRange("Sequence is empty");

    T result = this->sequence->Get(0);
    size_t length = this->GetLength();

    for (size_t index = 1; index < length; ++index) {
        result = Function(result, this->sequence->Get(index));
    }

    return result;
}

#pragma endregion

#pragma region Option

template<class T>
Option<T> AdaptiveSequence<T>::GetFirst(bool (*Function)(T))
{
    this->UpdateStrategy();
    return this->sequence->GetFirst(Function);
}

template<class T>
Option<T> AdaptiveSequence<T>::GetLast(bool (*Function)(T))
{
    this->UpdateStrategy();
    return this->sequence->GetLast(Function);
}

#pragma endregion

#pragma region Zip/Skip/Split/Splice/FlatMap

template<class T>
Sequence<T>* AdaptiveSequence<T>::Skip(size_t count)
{
    if (count >= this->GetLength()) {
        return new AdaptiveSequence<T>();
    }
    return this->GetSubsequence(count, this->GetLength() - 1);
}

template<class T>
Sequence<Sequence<T>*>* AdaptiveSequence<T>::Split(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    auto result = new AdaptiveSequence<Sequence<T>*>();
    Sequence<T>* currentPart = new AdaptiveSequence<T>();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            T value = this->Get(i);
            if (Function(value)) {
                result->Append(currentPart);
                currentPart = new AdaptiveSequence<T>();
            } else {
                currentPart->Append(value);
            }
        }
        result->Append(currentPart);
    }
    catch (...) {
        delete result;
        delete currentPart;
        throw;
    }
    
    return result;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::Splice(size_t index, size_t count, Sequence<T>* insertSequence)
{
    if (index > this->GetLength()) throw OutOfRange("Index out of bounds");

    size_t realCount = (count <= this->GetLength() - index) ? count : this->GetLength() - index;

    auto result = new AdaptiveSequence<T>();

    try {
        for (size_t i = 0; i < index; ++i) {
            result->Append(this->Get(i));
        }

        if (insertSequence != nullptr) {
            for (size_t i = 0; i < insertSequence->GetLength(); ++i) {
                result->Append(insertSequence->Get(i));
            }
        }

        for (size_t i = index + realCount; i < this->GetLength(); ++i) {
            result->Append(this->Get(i));
        }
    } 
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<T>* AdaptiveSequence<T>::FlatMap(Sequence<T>* (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    auto result = new AdaptiveSequence<T>();

    try {
        size_t length = this->GetLength();
        for (size_t index = 0; index < length; ++index) {
            Sequence<T>* subSequence = Function(this->Get(index)); 
            
            for (size_t subIndex = 0; subIndex < subSequence->GetLength(); ++subIndex) {
                result->Append(subSequence->Get(subIndex));
            }
            
            delete subSequence;
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<Pair<T, T>>* AdaptiveSequence<T>::Zip(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    auto result = new AdaptiveSequence<Pair<T, T>>();
    
    size_t minLen = (this->GetLength() < other->GetLength()) ? this->GetLength() : other->GetLength();

    try {
        for (size_t i = 0; i < minLen; ++i) {
            result->Append(Pair<T, T>(this->Get(i), other->Get(i)));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

#pragma endregion

template<class T>
IEnumerator<T>* AdaptiveSequence<T>::GetEnumerator()
{
    return this->sequence->GetEnumerator();
}