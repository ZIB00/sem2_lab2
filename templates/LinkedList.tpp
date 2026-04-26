#pragma once

#include "LinkedList.hpp"

template<class T>
LinkedList<T>::Node::Node(T value, Node* next)
{
    this->value = value;
    this->next = next;
}

template<class T>
void LinkedList<T>::Clear()
{
    Node* current = this->head;

    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }

    this->head = nullptr;
    this->tail = nullptr;
}

template<class T>
LinkedList<T>::LinkedList(T* items, int count)
{
    if (count < 0) throw InvalidArgument("Count cannot be negative");
    if (count > 0 && items == nullptr) throw InvalidArgument("Items cannot be null when count is positive");

    this->head = nullptr;
    this->tail = nullptr;

    try {
        for (int i = 0; i < count; i++) {
            this->Append(items[i]);
        }
    }
    catch (...) {
        this->Clear();
        throw;
    }
}

template<class T>
LinkedList<T>::LinkedList()
{
    this->head = nullptr;
    this->tail = nullptr;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list)
{
    this->head = nullptr;
    this->tail = nullptr;

    Node* current = list.head;

    try {
        while (current != nullptr) {
            this->Append(current->value);
            current = current->next;
        }
    }
    catch (...) {
        this->Clear();
        throw;
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{
    this->Clear();
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list)
{
    if (this != &list) {
        LinkedList<T> copy(list);

        Node* tempHead = this->head;
        Node* tempTail = this->tail;

        this->head = copy.head;
        this->tail = copy.tail;

        copy.head = tempHead;
        copy.tail = tempTail;
    }

    return *this;
}

template<class T>
T LinkedList<T>::GetFirst()
{
    if (this->head == nullptr) throw OutOfRange("List is empty");

    return this->head->value;
}

template<class T>
T LinkedList<T>::GetLast()
{
    if (this->tail == nullptr) throw OutOfRange("List is empty");

    return this->tail->value;
}

template<class T>
T LinkedList<T>::Get(int index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");

    Node* current = this->head;
    int currentIndex = 0;

    while (current != nullptr) {
        if (currentIndex == index) {
            return current->value;
        }

        current = current->next;
        currentIndex++;
    }

    throw OutOfRange("Index is out of range");
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex)
{
    if (startIndex < 0) throw InvalidArgument("Start index cannot be negative");
    if (endIndex < 0) throw InvalidArgument("End index cannot be negative");
    if (startIndex > endIndex) throw InvalidArgument("Start index cannot be bigger than end index");

    LinkedList<T>* result = new LinkedList<T>();
    Node* current = this->head;
    int currentIndex = 0;

    try {
        while (current != nullptr && currentIndex <= endIndex) {
            if (currentIndex >= startIndex) {
                result->Append(current->value);
            }

            current = current->next;
            currentIndex++;
        }

        if (endIndex >= currentIndex) {
            throw OutOfRange("Index is out of range");
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}

template<class T>
int LinkedList<T>::GetLength()
{
    int length = 0;
    Node* current = this->head;

    while (current != nullptr) {
        length++;
        current = current->next;
    }

    return length;
}

template<class T>
void LinkedList<T>::Append(T item)
{
    Node* node = new Node(item);

    if (this->head == nullptr) {
        this->head = node;
        this->tail = node;
        return;
    }

    this->tail->next = node;
    this->tail = node;
}

template<class T>
void LinkedList<T>::Prepend(T item)
{
    Node* node = new Node(item, this->head);

    this->head = node;

    if (this->tail == nullptr) {
        this->tail = node;
    }
}

template<class T>
void LinkedList<T>::InsertAt(T item, int index)
{
    if (index < 0) throw InvalidArgument("Index cannot be negative");

    if (index == 0) {
        this->Prepend(item);
        return;
    }

    Node* previous = this->head;
    int previousIndex = 0;

    while (previous != nullptr && previousIndex < index - 1) {
        previous = previous->next;
        previousIndex++;
    }

    if (previous == nullptr) throw OutOfRange("Index is out of range");

    Node* node = new Node(item, previous->next);
    previous->next = node;

    if (node->next == nullptr) {
        this->tail = node;
    }
}

template<class T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* list)
{
    if (list == nullptr) throw InvalidArgument("List cannot be null");

    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* current = list->head;

    try {
        while (current != nullptr) {
            result->Append(current->value);
            current = current->next;
        }
    }
    catch (...) {
        delete result;
        throw;
    }

    return result;
}
