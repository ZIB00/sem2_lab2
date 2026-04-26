#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"

constexpr int SEGMENT_SIZE = 10;

template<class T>
class SegmentedList : public Sequence<T>
{
    private:
        struct Node
        {
            DynamicArray<T>* elements;
            int count;
            Node* next;

            Node();
            ~Node();
        };

        Node* head;
        Node* tail;
        int length;

        void Clear();
        void ValidateNotEmpty();
        Node* FindNode(int index, int& localIndex);
        void Set(int index, T value);

    public:
        SegmentedList();
        SegmentedList(T* items, int count);
        SegmentedList(const SegmentedList<T>& list);
        ~SegmentedList() override;

        SegmentedList<T>& operator=(const SegmentedList<T>& list);

        T GetFirst() override;
        T GetLast() override;
        T Get(int index) override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) override;
        int GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, int index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;
};

#include "SegmentedList.tpp"
