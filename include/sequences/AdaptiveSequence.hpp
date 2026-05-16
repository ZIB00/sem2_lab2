#pragma once

#include "ArraySequence.hpp"
#include "ListSequence.hpp"

template<class T>
class AdaptiveSequence : public Sequence<T>
{
    private:
        Sequence<T>* sequence;
        size_t getCount;
        size_t insertCount;

        void SwitchToArraySequence();
        void SwitchToListSequence();
        void UpdateStrategy();
        void CopyFrom(const AdaptiveSequence<T>& other);

    public:
        AdaptiveSequence();
        AdaptiveSequence(T* items, size_t count);
        AdaptiveSequence(const AdaptiveSequence<T>& other);
        AdaptiveSequence(std::initializer_list<T> items);
        ~AdaptiveSequence() override;

        AdaptiveSequence<T>& operator=(const AdaptiveSequence<T>& other);
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

        IEnumerator<T>* GetEnumerator() override;
};

#include "AdaptiveSequence.tpp"