#pragma once

#include "DynamicArray.hpp"
#include "Sequence.hpp"

template<class T>
class ArraySequence : public Sequence<T>
{
    private:
        DynamicArray<T>* items;

        void ValidateNotEmpty();
        void ValidateSubsequenceRange(size_t startIndex, size_t endIndex);
        void ValidateInsertIndex(size_t index);
        void AppendInternal(T item);
        void InsertAtInternal(T item, size_t index);
        void ConcatInternal(Sequence<T>* list);

    protected:
        ArraySequence();
        void SetItems(T* items, size_t count);
        void CopyItems(const ArraySequence<T>& sequence);
        virtual ArraySequence<T>* Instance() = 0;
        virtual ArraySequence<T>* CreateEmpty() = 0;

    public:
        ~ArraySequence() override;

        ArraySequence<T>& operator=(const ArraySequence<T>& sequence);

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;
        size_t GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        Sequence<T>* Map(T (*)(T)) override;
        Sequence<T>* Where(bool (*)(T)) override;
        T Reduce(T (*)(T, T)) override;

        Option<T> TryGetFirst(bool (*)(T)) override;
        Option<T> TryGetLast(bool (*)(T)) override;

        Sequence<T>* FlatMap(Sequence<T>* (*)(T)) override;

        IEnumerator<T>* GetEnumerator() override;
};

template<class T>
class ArraySequenceEnumerator : public IEnumerator<T>
{
private:
    ArraySequence<T>* sequence;
    int position;

public:
    ArraySequenceEnumerator(ArraySequence<T>* sequence);

    T GetCurrent() override;
    bool MoveNext() override;
    void Reset() override;
};

template<class T>
class MutableArraySequence : public ArraySequence<T>
{
    protected:
        ArraySequence<T>* Instance() override;
        ArraySequence<T>* CreateEmpty() override;

    public:
        MutableArraySequence();
        MutableArraySequence(T* items, size_t count);
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
        ImmutableArraySequence(T* items, size_t count);
        ImmutableArraySequence(const ImmutableArraySequence<T>& sequence);

        ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& sequence);
};

#include "ArraySequence.tpp"