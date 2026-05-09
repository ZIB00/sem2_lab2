#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"

template<class T>
class ListSequence : public Sequence<T>
{
    private:
        LinkedList<T>* items;

        void AppendInternal(T item);
        void PrependInternal(T item);
        void InsertAtInternal(T item, size_t index);
        void ConcatInternal(Sequence<T>* list);

    protected:
        ListSequence();
        void SetItems(T* items, size_t count);
        void CopyItems(const ListSequence<T>& sequence);
        virtual ListSequence<T>* Instance() = 0;
        virtual ListSequence<T>* CreateEmpty() = 0;

    public:
        ~ListSequence() override;

        ListSequence<T>& operator=(const ListSequence<T>& sequence);
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

        Sequence<T>* Map(T (*Function)(T)) override;
        Sequence<T>* Where(bool (*Function)(T)) override;
        T Reduce(T (*Function)(T, T)) override;

        Option<T> GetFirst(bool (*Function)(T)) override;
        Option<T> GetLast(bool (*Function)(T)) override;

        IEnumerator<T>* GetEnumerator() override;

        Sequence<T>* FlatMap(Sequence<T>* (*Function)(T));
        Sequence<T>* Skip(size_t count);
        Sequence<T>* Splice(size_t index, size_t count, Sequence<T>* insertSequence = nullptr);
};

template<class T>
class ListSequenceEnumerator : public IEnumerator<T>
{
    private:
        ListSequence<T>* sequence;
        int position;
    
    public:
        ListSequenceEnumerator(ListSequence<T>* sequence);

        T GetCurrent() override;
        bool MoveNext() override;
        void Reset() override;
};

template<class T>
class MutableListSequence : public ListSequence<T>
{
    protected:
        ListSequence<T>* Instance() override;
        ListSequence<T>* CreateEmpty() override;

    public:
        MutableListSequence();
        MutableListSequence(T* items, size_t count);
        MutableListSequence(const MutableListSequence<T>& sequence);

        MutableListSequence<T>& operator=(const MutableListSequence<T>& sequence);
};

template<class T>
class ImmutableListSequence : public ListSequence<T>
{
    protected:
        ListSequence<T>* Instance() override;
        ListSequence<T>* CreateEmpty() override;

    public:
        ImmutableListSequence();
        ImmutableListSequence(T* items, size_t count);
        ImmutableListSequence(const ImmutableListSequence<T>& sequence);

        ImmutableListSequence<T>& operator=(const ImmutableListSequence<T>& sequence);
};

#include "ListSequence.tpp"