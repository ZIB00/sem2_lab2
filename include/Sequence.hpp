#pragma once

#include "ICollection.hpp"
#include "IEnumerable.hpp"
#include "Option.hpp"

template<class T>
class Sequence : public ICollection<T>, public IEnumerable<T>
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

        virtual Sequence<T>* Map(T (*)(T)) = 0;
        virtual Sequence<T>* Where(bool (*)(T)) = 0;
        virtual T Reduce(T (*)(T, T)) = 0;

        virtual Option<T> TryGetFirst(bool (*)(T)) = 0;
        virtual Option<T> TryGetLast(bool (*)(T)) = 0;

        virtual Sequence<T>* FlatMap(Sequence<T>* (*)(T)) = 0;

        virtual IEnumerator<T>* GetEnumerator() = 0;
};