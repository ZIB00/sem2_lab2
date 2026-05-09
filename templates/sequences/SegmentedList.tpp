#pragma once

#include "SegmentedList.hpp"

#pragma region sequence main functions

template<class T>
SegmentedList<T>::Node::Node()
{
    this->elements = new DynamicArray<T>(SEGMENT_SIZE);
    this->count = 0;
    this->next = nullptr;
}

template<class T>
SegmentedList<T>::Node::~Node()
{
    delete this->elements;
}

template<class T>
SegmentedList<T>::SegmentedList()
{
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

template<class T>
SegmentedList<T>::SegmentedList(T* items, size_t count)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;

    if (count < 0) throw InvalidArgument("Count cannot be negative");
    if (count > 0 && items == nullptr) throw InvalidArgument("Items cannot be null when count is positive");

    try {
        for (size_t index = 0; index < count; ++index) {
            this->Append(items[index]);
        }
    }
    catch (...) {
        this->Clear();
        throw;
    }
}

template<class T>
SegmentedList<T>::SegmentedList(const SegmentedList<T>& list)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;

    try {
        Node* current = list.head;

        while (current != nullptr) {
            for (size_t index = 0; index < current->count; ++index) {
                this->Append(current->elements->Get(index));
            }

            current = current->next;
        }
    }
    catch (...) {
        this->Clear();
        throw;
    }
}

template<class T>
SegmentedList<T>::~SegmentedList()
{
    this->Clear();
}

template<class T>
void SegmentedList<T>::Clear()
{
    Node* current = this->head;

    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

template<class T>
void SegmentedList<T>::ValidateNotEmpty()
{
    if (this->length == 0) {
        throw OutOfRange("Sequence is empty");
    }
}

template<class T>
typename SegmentedList<T>::Node* SegmentedList<T>::FindNode(size_t index, size_t& localIndex)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");
    if (index >= this->length) throw OutOfRange("Index is out of range");

    Node* current = this->head;
    size_t remaining = index;

    while (current != nullptr) {
        if (remaining < current->count) {
            localIndex = remaining;
            return current;
        }

        remaining -= current->count;
        current = current->next;
    }

    throw OutOfRange("Index is out of range");
}

template<class T>
void SegmentedList<T>::Set(size_t index, T value)
{
    size_t localIndex = 0;
    Node* node = this->FindNode(index, localIndex);
    node->elements->Set(localIndex, value);
}

template<class T>
T SegmentedList<T>::GetFirst()
{
    this->ValidateNotEmpty();
    return this->head->elements->Get(0);
}

template<class T>
T SegmentedList<T>::GetLast()
{
    this->ValidateNotEmpty();
    return this->tail->elements->Get(this->tail->count - 1);
}

template<class T>
T SegmentedList<T>::Get(size_t index)
{
    size_t localIndex = 0;
    Node* node = this->FindNode(index, localIndex);
    return node->elements->Get(localIndex);
}

template<class T>
Sequence<T>* SegmentedList<T>::GetSubsequence(size_t startIndex, size_t endIndex)
{
    if (startIndex < 0) throw InvalidArgument("Start index cannot be negative");
    if (endIndex < 0) throw InvalidArgument("End index cannot be negative");
    if (startIndex > endIndex) throw InvalidArgument("Start index cannot be greater than end index");
    if (endIndex >= this->length) throw OutOfRange("Index is out of range");

    SegmentedList<T>* result = new SegmentedList<T>();

    try {
        for (size_t index = startIndex; index <= endIndex; ++index) {
            result->Append(this->Get(index));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
size_t SegmentedList<T>::GetLength()
{
    return this->length;
}

template<class T>
Sequence<T>* SegmentedList<T>::Append(T item)
{
    if (this->tail == nullptr || this->tail->count == SEGMENT_SIZE) {
        Node* node = new Node();

        if (this->tail != nullptr) {
            this->tail->next = node;
        }

        this->tail = node;

        if (this->head == nullptr) {
            this->head = node;
        }
    }

    this->tail->elements->Set(this->tail->count, item);
    this->tail->count++;
    this->length++;
    return this;
}

template<class T>
Sequence<T>* SegmentedList<T>::Prepend(T item)
{
    if (this->head == nullptr || this->head->count == SEGMENT_SIZE) {
        Node* node = new Node();
        node->next = this->head;
        this->head = node;

        if (this->tail == nullptr) {
            this->tail = node;
        }
    }
    else {
        for (size_t index = this->head->count; index > 0; --index) {
            this->head->elements->Set(index, this->head->elements->Get(index - 1));
        }
    }

    this->head->elements->Set(0, item);
    this->head->count++;
    this->length++;
    return this;
}

template<class T>
Sequence<T>* SegmentedList<T>::InsertAt(T item, size_t index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");

    if (index > this->length) throw OutOfRange("Index is out of range");

    if (index == 0) {
        return this->Prepend(item);
    }

    if (index == this->length) {
        return this->Append(item);
    }

    size_t oldLength = this->length;
    this->Append(item);

    for (size_t currentIndex = oldLength; currentIndex > index; --currentIndex) {
        this->Set(currentIndex, this->Get(currentIndex - 1));
    }

    this->Set(index, item);
    return this;
}

template<class T>
Sequence<T>* SegmentedList<T>::Concat(Sequence<T>* list)
{
    if (list == nullptr) throw InvalidArgument("Sequence cannot be null");

    for (size_t index = 0; index < list->GetLength(); ++index) {
        this->Append(list->Get(index));
    }

    return this;
}

#pragma endregion

#pragma region operators (=, [], const [], +, ==, !=)

template<class T>
SegmentedList<T>& SegmentedList<T>::operator=(const SegmentedList<T>& list)
{
    if (this == &list) {
        return *this;
    }

    SegmentedList<T> copy(list);

    Node* oldHead = this->head;
    Node* oldTail = this->tail;
    size_t oldLength = this->length;

    this->head = copy.head;
    this->tail = copy.tail;
    this->length = copy.length;

    copy.head = oldHead;
    copy.tail = oldTail;
    copy.length = oldLength;

    return *this;
}

template<class T>
T& SegmentedList<T>::operator[](size_t index)
{
    size_t localIndex = 0;
    Node* node = this->FindNode(index, localIndex);
    return (*node->elements)[localIndex]; 
}

template<class T>
const T& SegmentedList<T>::operator[](size_t index) const
{
    size_t localIndex = 0;
    Node* node = const_cast<SegmentedList<T>*>(this)->FindNode(index, localIndex);
    return (*node->elements)[localIndex]; 
}

template<class T>
Sequence<T>* SegmentedList<T>::operator+(Sequence<T>* other)
{
    if (other == nullptr) throw InvalidArgument("Cannot add null sequence");

    auto result = new SegmentedList<T>();

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
bool SegmentedList<T>::operator==(Sequence<T>* other)
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
bool SegmentedList<T>::operator!=(Sequence<T>* other)
{
    return !(*this == other);
}

#pragma endregion

#pragma region Mar/Where/Reduce

template<class T>
Sequence<T>* SegmentedList<T>::Map(T (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    SegmentedList<T>* result = new SegmentedList<T>();

    try {
        for (size_t index = 0; index < this->length; ++index) {
            result->Append(Function(this->Get(index)));
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
Sequence<T>* SegmentedList<T>::Where(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    SegmentedList<T>* result = new SegmentedList<T>();

    try {
        for (size_t index = 0; index < this->length; ++index) {
            T value = this->Get(index);

            if (Function(value)) {
                result->Append(value);
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
T SegmentedList<T>::Reduce(T (*Function)(T, T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    this->ValidateNotEmpty();
    T result = this->Get(0);

    for (size_t index = 1; index < this->length; ++index) {
        result = Function(result, this->Get(index));
    }

    return result;
}

#pragma endregion

#pragma region Option

template<class T>
Option<T> SegmentedList<T>::GetFirst(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    for (size_t index = 0; index < this->length; ++index) {
        T value = this->Get(index);
        if (Function(value)) return Option<T>(value);
    }
    return Option<T>();
}

template<class T>
Option<T> SegmentedList<T>::GetLast(bool (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");
    
    for (size_t index = this->length; index > 0; --index) {
        T value = this->Get(index - 1);
        if (Function(value)) return Option<T>(value);
    }
    return Option<T>();
}

#pragma endregion

#pragma region Zip/Skip/Split/Splice/FlatMap

template<class T>
Sequence<T>* SegmentedList<T>::Skip(size_t count)
{
    if (count >= this->GetLength()) {
        return new SegmentedList<T>();
    }
    return this->GetSubsequence(count, this->GetLength() - 1);
}

template<class T>
Sequence<T>* SegmentedList<T>::Splice(size_t index, size_t count, Sequence<T>* insertSequence)
{
    if (index > this->GetLength()) throw OutOfRange("Index out of bounds");

    size_t realCount = (count <= this->GetLength() - index) ? count : this->GetLength() - index;

    auto result = new SegmentedList<T>();

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
Sequence<T>* SegmentedList<T>::FlatMap(Sequence<T>* (*Function)(T))
{
    if (Function == nullptr) throw InvalidArgument("Function cannot be null");

    auto result = new SegmentedList<T>();

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

#pragma endregion

template<class T>
IEnumerator<T>* SegmentedList<T>::GetEnumerator()
{
    throw std::logic_error("Enumerator for SegmentedList is not implemented yet");
}