#pragma once

#include "LinkedList.hpp"
#include "Sequence.hpp"

template<class T>
class ListSequence : public Sequence<T>
{
    private:
        LinkedList<T>* items;

        void Appendsize_ternal(T item);
        void Prependsize_ternal(T item);
        void InsertAtsize_ternal(T item, size_t index);
        void Concatsize_ternal(Sequence<T>* list);

    protected:
        ListSequence();
        void SetItems(T* items, size_t count);
        void CopyItems(const ListSequence<T>& sequence);
        virtual ListSequence<T>* Instance() = 0;
        virtual ListSequence<T>* CreateEmpty() = 0;

    public:
        ~ListSequence() override;

        ListSequence<T>& operator=(const ListSequence<T>& sequence);

        T GetFirst() override;
        T GetLast() override;
        T Get(size_t index) override;
        Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) override;
        size_t GetLength() override;

        Sequence<T>* Append(T item) override;
        Sequence<T>* Prepend(T item) override;
        Sequence<T>* InsertAt(T item, size_t index) override;
        Sequence<T>* Concat(Sequence<T>* list) override;

        template<class T2>
        Sequence<T>* Map(T2 (*)(T)) override;
        
        Sequence<T>* Where(bool (*)(T)) override;

        template<class T2>
        T Reduce(T2 (*)(T2, T)) override;
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