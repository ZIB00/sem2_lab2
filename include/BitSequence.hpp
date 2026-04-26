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
        int bitCount;

        void SetBit(int index, bool value);

    public:
        BitSequence();
        BitSequence(T* items, int count);
        BitSequence(const BitSequence<T>& other);
        ~BitSequence() override;

        BitSequence<T>& operator=(const BitSequence<T>& other);

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

        BitSequence<T>* NOT();
        BitSequence<T>* AND(const BitSequence<T>* other);
        BitSequence<T>* OR(const BitSequence<T>* other);
        BitSequence<T>* XOR(const BitSequence<T>* other);
};

#include "BitSequence.tpp"