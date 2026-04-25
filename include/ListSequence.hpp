#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"

template<class T>
class ListSequence : public Sequence<T>
{
    private:
        LinkedList<T>* items;

    public:
        ListSequence();
        ListSequence(T* items, int count);
        ListSequence(const ListSequence<T>& sequence);
        ~ListSequence() override;

        ListSequence<T>& operator=(const ListSequence<T>& sequence);

        T GetFirst() override;
        T GetLast() override;
        T Get(int index) override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) override;
        int GetLength() override;

        ListSequence<T>* Append(T item) override;
        ListSequence<T>* Prepend(T item) override;
        ListSequence<T>* InsertAt(T item, int index) override;
        ListSequence<T>* Concat(Sequence<T>* list) override;
};

#include "ListSequence.tpp"
