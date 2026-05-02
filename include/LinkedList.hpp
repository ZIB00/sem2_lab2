#pragma once

#include "Exceptions.hpp"

template<class T>
class LinkedList
{
    private:
        struct Node
        {
            T value;
            Node* next;

            Node(T value, Node* next = nullptr);
        };

        Node* head;
        Node* tail;

        void Clear();

    public:
        LinkedList(T* items, size_t count);
        LinkedList();
        LinkedList(const LinkedList<T>& list);
        ~LinkedList();

        LinkedList<T>& operator=(const LinkedList<T>& list);

        T GetFirst();
        T GetLast();
        T Get(size_t index);
        LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex);
        size_t GetLength();

        void Append(T item);
        void Prepend(T item);
        void InsertAt(T item, size_t index);
        LinkedList<T>* Concat(const LinkedList<T>* list);
};

#include "LinkedList.tpp"
