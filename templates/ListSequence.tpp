#pragma once

#include "ListSequence.hpp"

#pragma region sequence main functions

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
void ListSequence<T>::SetItems(T* items, size_t count)
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
void ListSequence<T>::InsertAtInternal(T item, size_t index)
{
    this->items->InsertAt(item, index);
}

template<class T>
void ListSequence<T>::ConcatInternal(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    size_t length = list->GetLength();

    for (size_t index = 0; index < length; ++index) {
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
T ListSequence<T>::Get(size_t index)
{
    return this->items->Get(index);
}

template<class T>
Sequence<T>* ListSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex)
{
    LinkedList<T>* subList = this->items->GetSubList(startIndex, endIndex);
    ListSequence<T>* result = this->CreateEmpty();

    try {
        for (size_t index = 0; index < subList->GetLength(); ++index) {
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
size_t ListSequence<T>::GetLength()
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
Sequence<T>* ListSequence<T>::InsertAt(T item, size_t index)
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

#pragma endregion

#pragma region operators (=, [], const [], +, ==, !=)

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
T& ListSequence<T>::operator[](size_t index)
{
    return (*this->items)[index]; 
}

template<class T>
const T& ListSequence<T>::operator[](size_t index) const
{
    return (*this->items)[index]; 
}

template<class T>
Sequence<T>* ListSequence<T>::operator+(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Cannot add null sequence");

    Sequence<T>* result = this->CreateEmpty();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            result->AppendInternal((*this)[i]);
        }
        for (size_t i = 0; i < other->GetLength(); ++i) {
            result->AppendInternal(other->Get(i));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
bool ListSequence<T>::operator==(Sequence<T>* other)
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
bool ListSequence<T>::operator!=(Sequence<T>* other)
{
    return !(*this == other);
}

#pragma endregion

#pragma region Mar/Where/Reduce

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
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
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
        size_t length = this->GetLength();

        for (size_t index = 0; index < length; ++index) {
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
    size_t length = this->GetLength();

    for (size_t index = 1; index < length; ++index) {
        result = Function(result, this->items->Get(index));
    }

    return result;
}

#pragma endregion

#pragma region Option

template<class T>
Option<T> ListSequence<T>::GetFirst(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    size_t length = this->GetLength();
    for (size_t index = 0; index < length; ++index) {
        T value = this->items->Get(index);
        if (Function(value)) {
            return Option<T>(value);
        }
    }
    return Option<T>();
}

template<class T>
Option<T> ListSequence<T>::GetLast(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    size_t length = this->GetLength();
    for (size_t index = length; index > 0; --index) {
        T value = this->items->Get(index - 1);
        if (Function(value)) {
            return Option<T>(value);
        }
    }
    return Option<T>();
}

#pragma endregion

template<class T>
IEnumerator<T>* ListSequence<T>::GetEnumerator()
{
    return new ListSequenceEnumerator<T>(this);
}

#pragma region IEnumerator

template<class T>
ListSequenceEnumerator<T>::ListSequenceEnumerator(ListSequence<T>* sequence)
{
    this->sequence = sequence;
    this->position = -1;
}

template<class T>
T ListSequenceEnumerator<T>::GetCurrent()
{
    if (position < 0 || position >= sequence->GetLength()) throw OutOfRange("Enumerator is out of bounds");

    return sequence->Get(position);
}

template<class T>
bool ListSequenceEnumerator<T>::MoveNext()
{
    if (position + 1 < sequence->GetLength()) {
        position++;
        return true;
    }

    return false;
}

template<class T>
void ListSequenceEnumerator<T>::Reset()
{
    this->position = -1;
}

#pragma endregion

#pragma region Zip/Skip/Split/Splice/FlatMap

template<class T>
Sequence<T>* ListSequence<T>::Skip(size_t count)
{
    if (count >= this->GetLength()) {
        return this->CreateEmpty();
    }
    return this->GetSubsequence(count, this->GetLength() - 1);
}

template<class T>
Sequence<Sequence<T>*>* ListSequence<T>::Split(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    Sequence<Sequence<T>*>* result = nullptr;
    if (dynamic_cast<ImmutableListSequence<T>*>(this) != nullptr) {
        result = new ImmutableListSequence<Sequence<T>*>();
    } else {
        result = new MutableListSequence<Sequence<T>*>();
    }

    Sequence<T>* currentPart = this->CreateEmpty();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            T value = this->Get(i);
            if (Function(value)) {
                result->AppendInternal(currentPart);
                currentPart = this->CreateEmpty();
            } else {
                currentPart->AppendInternal(value);
            }
        }
        result->AppendInternal(currentPart);
    }
    catch (...) {
        delete result;
        delete currentPart;
        throw;
    }
    
    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::Splice(size_t index, size_t count, Sequence<T>* insertSequence)
{
    if (index > this->GetLength()) throw OutOfRange("Index out of bounds");

    size_t realCount = (count <= this->GetLength() - index) ? count : this->GetLength() - index;

    Sequence<T>* result = this->CreateEmpty();

    try {
        for (size_t i = 0; i < index; ++i) {
            result->AppendInternal(this->Get(i));
        }

        if (insertSequence != nullptr) {
            for (size_t i = 0; i < insertSequence->GetLength(); ++i) {
                result->AppendInternal(insertSequence->Get(i));
            }
        }

        for (size_t i = index + realCount; i < this->GetLength(); ++i) {
            result->AppendInternal(this->Get(i));
        }
    } 
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<T>* ListSequence<T>::FlatMap(Sequence<T>* (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    Sequence<T>* result = this->CreateEmpty();

    try {
        size_t length = this->GetLength();
        for (size_t index = 0; index < length; ++index) {
            Sequence<T>* subSequence = Function(this->Get(index)); 
            
            for (size_t subIndex = 0; subIndex < subSequence->GetLength(); ++subIndex) {
                result->AppendInternal(subSequence->Get(subIndex));
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
Sequence<Pair<T, T>>* ListSequence<T>::Zip(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    Sequence<Pair<T, T>>* result = nullptr;
    if (dynamic_cast<ImmutableListSequence<T>*>(this) != nullptr) {
        result = new ImmutableListSequence<Pair<T, T>>();
    } else {
        result = new MutableListSequence<Pair<T, T>>();
    }
    
    size_t minLen = (this->GetLength() < other->GetLength()) ? this->GetLength() : other->GetLength();

    try {
        for (size_t i = 0; i < minLen; ++i) {
            result->AppendInternal(Pair<T, T>(this->Get(i), other->Get(i)));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

#pragma endregion

#pragma region Mutable/Immutable

template<class T>
MutableListSequence<T>::MutableListSequence()
{
}

template<class T>
MutableListSequence<T>::MutableListSequence(T* items, size_t count)
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
ImmutableListSequence<T>::ImmutableListSequence(T* items, size_t count)
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

#pragma endregion