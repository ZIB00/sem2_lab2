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
        void SetItems(std::initializer_list<T> items);
        void CopyItems(const ArraySequence<T>& sequence);
        virtual ArraySequence<T>* Instance() = 0;
        virtual ArraySequence<T>* CreateEmptyMut() = 0;

    public:
        ~ArraySequence() override;

        ArraySequence<T>& operator=(const ArraySequence<T>& sequence);
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
        ArraySequence<T>* CreateEmptyMut() override;

    public:
        MutableArraySequence();
        MutableArraySequence(T* items, size_t count);
        MutableArraySequence(const MutableArraySequence<T>& sequence);
        MutableArraySequence(std::initializer_list<T> items);

        MutableArraySequence<T>& operator=(const MutableArraySequence<T>& sequence);
};

template<class T>
class ImmutableArraySequence : public ArraySequence<T>
{
    protected:
        ArraySequence<T>* Instance() override;
        ArraySequence<T>* CreateEmptyMut() override;

    public:
        ImmutableArraySequence();
        ImmutableArraySequence(T* items, size_t count);
        ImmutableArraySequence(const ImmutableArraySequence<T>& sequence);
        ImmutableArraySequence(std::initializer_list<T> items);

        ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& sequence);
};

#include "ArraySequence.tpp"