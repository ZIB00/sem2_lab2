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
        void InsertAtInternal(T item, int index);
        void ConcatInternal(Sequence<T>* list);

    protected:
        ListSequence();
        void SetItems(T* items, int count);
        void CopyItems(const ListSequence<T>& sequence);
        virtual ListSequence<T>* Instance() = 0;
        virtual ListSequence<T>* CreateEmpty() = 0;

    public:
        ~ListSequence() override;

        ListSequence<T>& operator=(const ListSequence<T>& sequence);

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

template<class T>
class MutableListSequence : public ListSequence<T>
{
    protected:
        ListSequence<T>* Instance() override;
        ListSequence<T>* CreateEmpty() override;

    public:
        MutableListSequence();
        MutableListSequence(T* items, int count);
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
        ImmutableListSequence(T* items, int count);
        ImmutableListSequence(const ImmutableListSequence<T>& sequence);

        ImmutableListSequence<T>& operator=(const ImmutableListSequence<T>& sequence);
};

#include "ListSequence.tpp"