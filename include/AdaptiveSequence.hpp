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
        ~AdaptiveSequence() override;

        AdaptiveSequence<T>& operator=(const AdaptiveSequence<T>& other);
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

#include "AdaptiveSequence.tpp"