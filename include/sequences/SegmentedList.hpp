#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Sequence.hpp"

constexpr size_t SEGMENT_SIZE = 10;

template<class T>
class SegmentedList : public Sequence<T>
{
    private:
        LinkedList<DynamicArray<T>*>* segments;

        size_t length;

        void Clear();
        void ValidateNotEmpty();
        DynamicArray<T>* FindNode(size_t index, size_t& localIndex);
        void Set(size_t index, T value);

    public:
        SegmentedList();
        SegmentedList(T* items, size_t count);
        SegmentedList(const SegmentedList<T>& list);
        SegmentedList(std::initializer_list<T> items);
        ~SegmentedList() override;

        SegmentedList<T>& operator=(const SegmentedList<T>& list);
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        Sequence<T>* operator+(Sequence<T>* other);
        bool operator==(Sequence<T>* other);
        bool operator!=(Sequence<T>* other);

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;
        size_t GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        Sequence<T>* CreateEmpty() override;

        class Enumerator : public IEnumerator<T>
        {
            private:
                SegmentedList<T>* list;
                size_t segmentIndex;
                size_t localIndex;
                bool started;

            public:
                Enumerator(SegmentedList<T>* list);
                T GetCurrent() override;
                bool MoveNext() override;
                void Reset() override;
        };

        IEnumerator<T>* GetEnumerator() override;
};

#include "SegmentedList.tpp"