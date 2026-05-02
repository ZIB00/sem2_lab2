#pragma once

template<class T>
class Sequence
{
    public:
        virtual ~Sequence() = default;

        virtual T GetFirst() = 0;
        virtual T GetLast() = 0;
        virtual T Get(size_t index) = 0;
        virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) = 0;
        virtual size_t GetLength() = 0;

        virtual Sequence<T>* Append(T item) = 0;
        virtual Sequence<T>* Prepend(T item) = 0;
        virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
        virtual Sequence<T>* Concat(Sequence<T>* list) = 0;

        template<class T2>
        virtual Sequence<T>* Map(T2 (*)(T)) = 0;

        virtual Sequence<T>* Where(bool (*)(T)) = 0;

        template<class T2>
        virtual T Reduce(T2 (*)(T2, T)) = 0;

        virtual Option<T> TryGetFirst(bool (*)(T)) = 0;
        virtual Option<T> TryGetLast(bool (*)(T)) = 0;

        virtual Sequence<T>* FlatMap(Sequence<T>* (*)(T)) = 0;
};