#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"

constexpr size_t SEGMENT_SIZE = 10;

template<class T>
class SegmentedList : public Sequence<T>
{
    private:
        struct Node
        {
            DynamicArray<T>* elements;
            size_t count;
            Node* next;

            Node();
            ~Node();
        };

        Node* head;
        Node* tail;
        size_t length;

        void Clear();
        void ValidateNotEmpty();
        Node* FindNode(size_t index, size_t& localIndex);
        void Set(size_t index, T value);

    public:
        SegmentedList();
        SegmentedList(T* items, size_t count);
        SegmentedList(const SegmentedList<T>& list);
        ~SegmentedList() override;

        SegmentedList<T>& operator=(const SegmentedList<T>& list);

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;
        size_t GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        template<class T2>
        Sequence<T>* Map(T2 (*)(T)) override;
        
        Sequence<T>* Where(bool (*)(T)) override;

        template<class T2>
        T Reduce(T2 (*)(T2, T)) override;
};

#include "SegmentedList.tpp"