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
        void AppendInternal(T item);
        void InsertAtInternal(T item, int index);
        void ConcatInternal(Sequence<T>* list);

    protected:
        ArraySequence();
        void SetItems(T* items, int count);
        void CopyItems(const ArraySequence<T>& sequence);
        virtual ArraySequence<T>* Instance() = 0;
        virtual ArraySequence<T>* CreateEmpty() = 0;

    public:
        ~ArraySequence() override;

        ArraySequence<T>& operator=(const ArraySequence<T>& sequence);

        T GetFirst() override;
        T GetLast() override;
        T Get(int index) override;
        Sequence<T>* GetSubsequence(int startIndex, int endIndex) override;
        int GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, int index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;
};

template<class T>
class MutableArraySequence : public ArraySequence<T>
{
    protected:
        ArraySequence<T>* Instance() override;
        ArraySequence<T>* CreateEmpty() override;

    public:
        MutableArraySequence();
        MutableArraySequence(T* items, int count);
        MutableArraySequence(const MutableArraySequence<T>& sequence);

        MutableArraySequence<T>& operator=(const MutableArraySequence<T>& sequence);
};

template<class T>
class ImmutableArraySequence : public ArraySequence<T>
{
    protected:
        ArraySequence<T>* Instance() override;
        ArraySequence<T>* CreateEmpty() override;

    public:
        ImmutableArraySequence();
        ImmutableArraySequence(T* items, int count);
        ImmutableArraySequence(const ImmutableArraySequence<T>& sequence);

        ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& sequence);
};

#include "ArraySequence.tpp"
