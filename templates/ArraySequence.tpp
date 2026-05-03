#pragma once

#include "ArraySequence.hpp"

#pragma region sequence main functions

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
void ArraySequence<T>::AppendInternal(T item)
{
    size_t length = this->GetLength();
    this->items->Resize(length + 1);
    this->items->Set(length, item);
}

template<class T>
void ArraySequence<T>::InsertAtInternal(T item, size_t index)
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
void ArraySequence<T>::ConcatInternal(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    size_t listLength = list->GetLength();

    for (size_t index = 0; index < listLength; ++index) {
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
size_t ArraySequence<T>::GetLength()
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
Sequence<T>* ArraySequence<T>::InsertAt(T item, size_t index)
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

#pragma endregion

#pragma region operators (=, [], const [], +, ==, !=)

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
T& ArraySequence<T>::operator[](size_t index)
{
    return (*this->items)[index]; 
}

template<class T>
const T& ArraySequence<T>::operator[](size_t index) const
{
    return (*this->items)[index]; 
}

template<class T>
Sequence<T>* ArraySequence<T>::operator+(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Cannot add null sequence");

    Sequence<T>* result = this->CreateEmpty();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            result = result->Append((*this)[i]);
        }
        for (size_t i = 0; i < other->GetLength(); ++i) {
            result = result->Append(other->Get(i));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
bool ArraySequence<T>::operator==(Sequence<T>* other)
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
bool ArraySequence<T>::operator!=(Sequence<T>* other)
{
    return !(*this == other);
}

#pragma endregion

#pragma region Mar/Where/Reduce

template<class T>
Sequence<T>* ArraySequence<T>::Map(T (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

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
Sequence<T>* ArraySequence<T>::Where(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    ArraySequence<T>* result = this->CreateEmpty();

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
T ArraySequence<T>::Reduce(T (*Function)(T, T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    this->ValidateNotEmpty();
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
Option<T> ArraySequence<T>::GetFirst(bool (*Function)(T))
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
Option<T> ArraySequence<T>::GetLast(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    size_t length = this->GetLength();
    // Идем с конца в начало
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
IEnumerator<T>* ArraySequence<T>::GetEnumerator()
{
    return new ArraySequenceEnumerator<T>(this);
}

#pragma region IEnumerator

template<class T>
ArraySequenceEnumerator<T>::ArraySequenceEnumerator(ArraySequence<T>* sequence)
{
    this->sequence = sequence;
    this->position = -1;
}

template<class T>
T ArraySequenceEnumerator<T>::GetCurrent()
{
    if (this->position < 0 || this->position >= sequence->GetLength()) throw OutOfRange("Enumerator is out of bounds");

    return sequence->Get(position);
}

template<class T>
bool ArraySequenceEnumerator<T>::MoveNext()
{
    if (position + 1 < sequence->GetLength()) {
        position++;
        return true;
    }
    return false;
}

template<class T>
void ArraySequenceEnumerator<T>::Reset()
{
    this->position = -1;
}

#pragma endregion

#pragma region Zip/Skip/Split/Splice/FlatMap

template<class T>
Sequence<T>* ArraySequence<T>::Skip(size_t count)
{
    if (count >= this->GetLength()) {
        return this->CreateEmpty();
    }
    return this->GetSubsequence(count, this->GetLength() - 1);
}

template<class T>
Sequence<Sequence<T>*>* ArraySequence<T>::Split(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    Sequence<Sequence<T>*>* result = nullptr;
    if (dynamic_cast<ImmutableArraySequence<T>*>(this) != nullptr) {
        result = new ImmutableArraySequence<Sequence<T>*>();
    } else {
        result = new MutableArraySequence<Sequence<T>*>();
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
Sequence<T>* ArraySequence<T>::Splice(size_t index, size_t count, Sequence<T>* insertSequence)
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
Sequence<T>* ArraySequence<T>::FlatMap(Sequence<T>* (*Function)(T))
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
Sequence<Pair<T, T>>* ArraySequence<T>::Zip(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    Sequence<Pair<T, T>>* result = nullptr;
    if (dynamic_cast<ImmutableArraySequence<T>*>(this) != nullptr) {
        result = new ImmutableArraySequence<Pair<T, T>>();
    } else {
        result = new MutableArraySequence<Pair<T, T>>();
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

#pragma endregion