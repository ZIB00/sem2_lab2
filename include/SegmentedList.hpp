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
        T& operator[](size_t index) override;
        const T& operator[](size_t index) const override;
        Sequence<T>* operator+(Sequence<T>* other) override;
        bool operator==(Sequence<T>* other) override;
        bool operator!=(Sequence<T>* other) override;

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;
        size_t GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        Sequence<T>* Map(T (*Function)(T)) override;
        Sequence<T>* Where(bool (*Function)(T)) override;
        T Reduce(T (*Function)(T, T)) override;

        Option<T> GetFirst(bool (*Function)(T)) override;
        Option<T> GetLast(bool (*Function)(T)) override;

        IEnumerator<T>* GetEnumerator() override;

        Sequence<T>* FlatMap(Sequence<T>* (*Function)(T)) override;
        Sequence<T>* Skip(size_t count) override;
        Sequence<Sequence<T>*>* Split(bool (*Function)(T)) override;
        Sequence<T>* Splice(size_t index, size_t count, Sequence<T>* insertSequence = nullptr) override;
        Sequence<Pair<T, T>>* Zip(Sequence<T>* other) override;
};

#include "SegmentedList.tpp"