#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"

template<class T>
class ArraySequence : public Sequence<T>
{
    private:
        DynamicArray<T>* items;

        void ValidateNotEmpty();
        void ValidateSubsequenceRange(int startIndex, int endIndex);
        void ValidateInsertIndex(int index);

    public:
        ArraySequence(T* items, int count);
        ArraySequence();
        ArraySequence(const ArraySequence<T>& sequence);
        ~ArraySequence() override;

        ArraySequence<T>& operator=(const ArraySequence<T>& sequence);

        T GetFirst() override;
        T GetLast() override;
        T Get(int index) override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) override;
        int GetLength() override;

        ArraySequence<T>* Append(T item) override;
        ArraySequence<T>* Prepend(T item) override;
        ArraySequence<T>* InsertAt(T item, int index) override;
        ArraySequence<T>* Concat(Sequence<T>* list) override;
};

#include "ArraySequence.tpp"
