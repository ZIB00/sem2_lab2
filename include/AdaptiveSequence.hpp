#pragma once

#include "ArraySequence.hpp"
#include "ListSequence.hpp"

template<class T>
class AdaptiveSequence : public Sequence<T>
{
    private:
        Sequence<T>* sequence;
        int getCount;
        int insertCount;

        void SwitchToArraySequence();
        void SwitchToListSequence();
        void UpdateStrategy();
        void CopyFrom(const AdaptiveSequence<T>& other);

    public:
        AdaptiveSequence();
        AdaptiveSequence(T* items, int count);
        AdaptiveSequence(const AdaptiveSequence<T>& other);
        ~AdaptiveSequence() override;

        AdaptiveSequence<T>& operator=(const AdaptiveSequence<T>& other);

        T GetFirst() override;
        T GetLast() override;
        T Get(int index) override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) override;
        int GetLength() override;
        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, int index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        Sequence<T>* Map(T (*)(T)) override;
        Sequence<T>* Where(bool (*)(T)) override;
        T Reduce(T (*)(T, T)) override;
};

#include "AdaptiveSequence.tpp"