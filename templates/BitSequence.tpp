#pragma once

#include "BitSequence.hpp"

#pragma region sequence main functions

template<class T>
void BitSequence<T>::SetBit(size_t index, bool value)
{
    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;

    unsigned char theByte = this->bytes->Get(byteIndex);

    if (value) {
        theByte |= (1 << bitOffset);
    } else {
        theByte &= ~(1 << bitOffset);
    }

    this->bytes->Set(byteIndex, theByte);
}

template<class T>
BitSequence<T>::BitSequence()
{
    this->bytes = new DynamicArray<unsigned char>(0);
    this->bitCount = 0;
}

template<class T>
BitSequence<T>::BitSequence(T* items, size_t count) : BitSequence()
{
    if (count < 0) throw InvalidArgument("Count cannot be negative");

    for (size_t i = 0; i < count; ++i) {
        this->Append(items[i]);
    }
}

template<class T>
BitSequence<T>::BitSequence(const BitSequence<T>& other)
{
    this->bytes = new DynamicArray<unsigned char>(*other.bytes);
    this->bitCount = other.bitCount;
}

template<class T>
BitSequence<T>::~BitSequence()
{
    delete this->bytes;
}

template<class T>
T BitSequence<T>::Get(size_t index)
{
    if (index < 0 || index >= this->bitCount) {
        throw OutOfRange("Index is out of range");
    }

    size_t byteIndex = index / 8;
    size_t bitOffset = index % 8;

    unsigned char theByte = this->bytes->Get(byteIndex);
    bool bitValue = (theByte >> bitOffset) & 1;

    return static_cast<T>(bitValue);
}

template<class T>
T BitSequence<T>::GetFirst()
{
    if (this->bitCount == 0) throw OutOfRange("Sequence is empty");

    return this->Get(0);
}

template<class T>
T BitSequence<T>::GetLast()
{
    if (this->bitCount == 0) throw OutOfRange("Sequence is empty");

    return this->Get(this->bitCount - 1);
}

template<class T>
size_t BitSequence<T>::GetLength()
{
    return this->bitCount;
}

template<class T>
Sequence<T>* BitSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex)
{
    if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= this->bitCount) {
        throw OutOfRange("Invalid sequence range");
    }

    BitSequence<T>* result = new BitSequence<T>();
    for (size_t i = startIndex; i <= endIndex; ++i) {
        result->Append(this->Get(i));
    }
    return result;
}

template<class T>
Sequence<T>* BitSequence<T>::Append(T item)
{
    if (this->bitCount % 8 == 0) {
        size_t currentBytes = this->bytes->GetSize();
        this->bytes->Resize(currentBytes + 1);
        this->bytes->Set(currentBytes, 0);
    }

    this->SetBit(this->bitCount, static_cast<bool>(item));
    this->bitCount++;

    return this;
}

template<class T>
Sequence<T>* BitSequence<T>::InsertAt(T item, size_t index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");
    if (index > this->bitCount) throw OutOfRange("Index is out of range");

    if (index == this->bitCount) {
        return this->Append(item);
    }

    this->Append(static_cast<T>(false)); 

    for (size_t i = this->bitCount - 1; i > index; i--) {
        bool previousBit = static_cast<bool>(this->Get(i - 1));
        this->SetBit(i, previousBit);
    }

    this->SetBit(index, static_cast<bool>(item));

    return this;
}

template<class T>
Sequence<T>* BitSequence<T>::Prepend(T item)
{
    return this->InsertAt(item, 0);
}

template<class T>
Sequence<T>* BitSequence<T>::Concat(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    for (size_t i = 0; i < list->GetLength(); ++i) {
        this->Append(list->Get(i));
    }

    return this;
}

#pragma endregion

#pragma region operators (=, [], const [], +, ==, !=)

template<class T>
BitSequence<T>& BitSequence<T>::operator=(const BitSequence<T>& other)
{
    if (this == &other) {
        return *this;
    }

    DynamicArray<unsigned char>* newBytes = new DynamicArray<unsigned char>(*other.bytes);
    delete this->bytes;
    this->bytes = newBytes;
    this->bitCount = other.bitCount;

    return *this;
}

template<class T>
T& BitSequence<T>::operator[](size_t index)
{
    if (index >= this->bitCount) throw OutOfRange("Index out of bounds");
    
    static T temp;
    temp = this->Get(index);
    return temp;
}

template<class T>
const T& BitSequence<T>::operator[](size_t index) const
{
    if (index >= this->bitCount) throw OutOfRange("Index out of bounds");

    static T temp;
    temp = this->Get(index);
    return temp;
}

template<class T>
Sequence<T>* BitSequence<T>::operator+(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Cannot add null sequence");

    auto result = new BitSequence<T>();

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
bool BitSequence<T>::operator==(Sequence<T>* other)
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
bool BitSequence<T>::operator!=(Sequence<T>* other)
{
    return !(*this == other);
}

#pragma endregion

#pragma region Mar/Where/Reduce

template<class T>
Sequence<T>* BitSequence<T>::Map(T (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    BitSequence<T>* result = new BitSequence<T>();
    for (size_t i = 0; i < this->bitCount; ++i) {
        result->Append(Function(this->Get(i)));
    }

    return result;
}

template<class T>
Sequence<T>* BitSequence<T>::Where(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    BitSequence<T>* result = new BitSequence<T>();
    for (size_t i = 0; i < this->bitCount; ++i) {
        T value = this->Get(i);
        if (Function(value)) {
            result->Append(value);
        }
    }
    
    return result;
}

template<class T>
T BitSequence<T>::Reduce(T (*Function)(T, T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    if (this->bitCount == 0) throw OutOfRange("Sequence is empty");

    T result = this->Get(0);
    for (size_t i = 1; i < this->bitCount; ++i) {
        result = Function(result, this->Get(i));
    }

    return result;
}

#pragma endregion

#pragma region Option

template<class T>
Option<T> BitSequence<T>::GetFirst(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    for (size_t index = 0; index < this->GetLength(); ++index) {
        T value = this->Get(index);
        if (Function(value)) return Option<T>(value);
    }
    return Option<T>();
}

template<class T>
Option<T> BitSequence<T>::GetLast(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    for (size_t index = this->GetLength(); index > 0; --index) {
        T value = this->Get(index - 1);
        if (Function(value)) return Option<T>(value);
    }
    return Option<T>();
}

#pragma endregion

#pragma region Zip/Skip/Split/Splice/FlatMap

template<class T>
Sequence<T>* BitSequence<T>::Skip(size_t count)
{
    if (count >= this->GetLength()) {
        return new BitSequence<T>();
    }
    return this->GetSubsequence(count, this->GetLength() - 1);
}

template<class T>
Sequence<Sequence<T>*>* BitSequence<T>::Split(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    auto result = new BitSequence<Sequence<T>*>();
    Sequence<T>* currentPart = new BitSequence<T>();

    try {
        for (size_t i = 0; i < this->GetLength(); ++i) {
            T value = this->Get(i);
            if (Function(value)) {
                result->Append(currentPart);
                currentPart = new BitSequence<T>();
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
Sequence<T>* BitSequence<T>::Splice(size_t index, size_t count, Sequence<T>* insertSequence)
{
    if (index > this->GetLength()) throw OutOfRange("Index out of bounds");

    size_t realCount = (count <= this->GetLength() - index) ? count : this->GetLength() - index;

    auto result = new BitSequence<T>();

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
Sequence<T>* BitSequence<T>::FlatMap(Sequence<T>* (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    auto result = new BitSequence<T>();

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
Sequence<Pair<T, T>>* BitSequence<T>::Zip(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    auto result = new BitSequence<Pair<T, T>>();
    
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
IEnumerator<T>* BitSequence<T>::GetEnumerator()
{
    throw std::logic_error("Enumerator for BitSequence is not implemented yet");
}

template<class T>
BitSequence<T>* BitSequence<T>::NOT()
{
    BitSequence<T>* result = new BitSequence<T>();
    size_t byteSize = this->bytes->GetSize();
    
    result->bytes->Resize(byteSize);
    result->bitCount = this->bitCount;

    for (size_t i = 0; i < byteSize; ++i) {
        result->bytes->Set(i, ~this->bytes->Get(i));
    }

    return result;
}

template<class T>
BitSequence<T>* BitSequence<T>::AND(const BitSequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    BitSequence<T>* result = new BitSequence<T>();
    size_t maxBytes = std::max(this->bytes->GetSize(), other->bytes->GetSize());
    
    result->bytes->Resize(maxBytes);
    result->bitCount = std::max(this->bitCount, other->bitCount);

    for (size_t i = 0; i < maxBytes; ++i) {
        unsigned char byteA = 0;
        if (i < this->bytes->GetSize()) {
            byteA = this->bytes->Get(i);
        }

        unsigned char byteB = 0;
        if (i < other->bytes->GetSize()) {
            byteB = other->bytes->Get(i);
        }

        result->bytes->Set(i, byteA & byteB);
    }

    return result;
}

template<class T>
BitSequence<T>* BitSequence<T>::OR(const BitSequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    BitSequence<T>* result = new BitSequence<T>();
    size_t maxBytes = std::max(this->bytes->GetSize(), other->bytes->GetSize());
    
    result->bytes->Resize(maxBytes);
    result->bitCount = std::max(this->bitCount, other->bitCount);

    for (size_t i = 0; i < maxBytes; ++i) {
        unsigned char byteA = 0;
        if (i < this->bytes->GetSize()) {
            byteA = this->bytes->Get(i);
        }

        unsigned char byteB = 0;
        if (i < other->bytes->GetSize()) {
            byteB = other->bytes->Get(i);
        }

        result->bytes->Set(i, byteA | byteB);
    }

    return result;
}

template<class T>
BitSequence<T>* BitSequence<T>::XOR(const BitSequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Other sequence cannot be null");

    BitSequence<T>* result = new BitSequence<T>();
    size_t maxBytes = std::max(this->bytes->GetSize(), other->bytes->GetSize());
    
    result->bytes->Resize(maxBytes);
    result->bitCount = std::max(this->bitCount, other->bitCount);

    for (size_t i = 0; i < maxBytes; ++i) {
        unsigned char byteA = 0;
        if (i < this->bytes->GetSize()) {
            byteA = this->bytes->Get(i);
        }

        unsigned char byteB = 0;
        if (i < other->bytes->GetSize()) {
            byteB = other->bytes->Get(i);
        }

        result->bytes->Set(i, byteA ^ byteB);
    }

    return result;
}