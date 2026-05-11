#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "Exceptions.hpp"
#include <algorithm>

template<class T>
class BitSequence : public Sequence<T>
{
    private:
        DynamicArray<unsigned char>* bytes;
        size_t bitCount;

        void SetBit(size_t index, bool value);

    public:
        BitSequence();
        BitSequence(T* items, size_t count);
        BitSequence(const BitSequence<T>& other);
        ~BitSequence() override;

        BitSequence<T>& operator=(const BitSequence<T>& other);
        Sequence<T>* operator+(Sequence<T>* other);
        bool operator==(Sequence<T>* other);
        bool operator!=(Sequence<T>* other);

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        void Set(size_t index, T value);
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
        Sequence<T>* Splice(size_t index, size_t count, Sequence<T>* insertSequence = nullptr) override;

        BitSequence<T>* NOT();
        BitSequence<T>* AND(const BitSequence<T>* other);
        BitSequence<T>* OR(const BitSequence<T>* other);
        BitSequence<T>* XOR(const BitSequence<T>* other);
};

#include "BitSequence.tpp"