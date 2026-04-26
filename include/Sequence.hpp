#pragma once

template<class T>
class Sequence
{
    public:
        virtual ~Sequence() = default;

        virtual T GetFirst() = 0;
        virtual T GetLast() = 0;
        virtual T Get(int index) = 0;
        virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;
        virtual int GetLength() = 0;

        virtual Sequence<T>* Append(T item) = 0;
        virtual Sequence<T>* Prepend(T item) = 0;
        virtual Sequence<T>* InsertAt(T item, int index) = 0;
        virtual Sequence<T>* Concat(Sequence<T>* list) = 0;

        virtual Sequence<T>* Map(T (*)(T)) = 0;
        virtual Sequence<T>* Where(bool (*)(T)) = 0;
        virtual T Reduce(T (*)(T, T)) = 0;
};