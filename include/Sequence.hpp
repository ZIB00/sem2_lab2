#pragma once

#include "ICollection.hpp"
#include "IEnumerable.hpp"
#include "Option.hpp"

template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;

    Pair() : first(T1()), second(T2()) {} 
    
    Pair(T1 f, T2 s) : first(f), second(s) {} 
};

template<class T>
class Sequence : public ICollection<T>, public IEnumerable<T>
{
    public:
        virtual ~Sequence() = default;
        
        virtual T& operator[](size_t index) = 0;
        virtual const T& operator[](size_t index) const = 0;
        virtual Sequence<T>* operator+(Sequence<T>* other) = 0;
        virtual bool operator==(Sequence<T>* other) = 0;
        virtual bool operator!=(Sequence<T>* other) = 0;

        virtual T GetFirst() = 0;
        virtual T GetLast() = 0;
        virtual T Get(size_t index) = 0;
        virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) = 0;
        virtual size_t GetLength() = 0;

        virtual Sequence<T>* Append(T item) = 0;
        virtual Sequence<T>* Prepend(T item) = 0;
        virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
        virtual Sequence<T>* Concat(Sequence<T>* list) = 0;

        //M-2.
        virtual Sequence<T>* Map(T (*Function)(T)) = 0;
        virtual Sequence<T>* Where(bool (*)(T)) = 0;
        virtual T Reduce(T (*Function)(T, T)) = 0;

        virtual Option<T> GetFirst(bool (*Function)(T)) = 0;
        virtual Option<T> GetLast(bool (*Function)(T)) = 0;
        
        virtual IEnumerator<T>* GetEnumerator() = 0;

        //M2.1
        virtual Sequence<T>* FlatMap(Sequence<T>* (*Function)(T)) = 0;
        virtual Sequence<T>* Skip(size_t count) = 0;
        virtual Sequence<Sequence<T>*>* Split(bool (*Function)(T)) = 0;
        virtual Sequence<T>* Splice(size_t index, size_t count, Sequence<T>* insertSequence = nullptr) = 0;
        virtual Sequence<Pair<T, T>>* Zip(Sequence<T>* other) = 0;
};