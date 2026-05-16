#pragma once

#include "Sequence.hpp"
#include "IEnumerator.hpp"

namespace SequenceUtils {

    template<class T, class T2>
    Pair<Sequence<T>*, Sequence<T2>*> Unzip(Sequence<Pair<T, T2>>* pairsSeq, Sequence<T>* sequence1, Sequence<T2>* sequence2);

    template<class T>
    Sequence<T>* Range(T start, T end, T step, Sequence<T>* sequence);

    template<class T, class T2>
    class ZipEnumerator : public IEnumerator<Pair<T, T2>>
    {
    private:
        IEnumerator<T>* enumA;
        IEnumerator<T2>* enumB;
        Pair<T, T2> current;

    public:
        ZipEnumerator(IEnumerator<T>* a, IEnumerator<T2>* b);
        ~ZipEnumerator() override;

        Pair<T, T2> GetCurrent() override;

        bool MoveNext() override;

        void Reset() override;
    };

    template<class T, class T2>
    ZipEnumerator<T, T2>* Zip(Sequence<T>* a, Sequence<T2>* b);

    template<class T>
    class SplitEnumerator : public IEnumerator<Sequence<T>*>
    {
    private:
        IEnumerator<T>* source;
        std::function<bool(T)> predicate;
        Sequence<T>* prototype;
        Sequence<T>* current;
        bool done;

    public:
        SplitEnumerator(IEnumerator<T>* src, std::function<bool(T)> predicate, Sequence<T>* proto);
        
        ~SplitEnumerator() override;

        Sequence<T>* GetCurrent() override;
        bool MoveNext() override;
        void Reset() override;
    };

    template<class T>
    SplitEnumerator<T>* Split(Sequence<T>* seq, std::function<bool(T)> predicate, Sequence<T>* prototype);

    template<class T>
    Sequence<T>* FlatMap(Sequence<T>* seq, std::function<Sequence<T>*(T)> function);

    template<class T>
    Sequence<T>* Skip(Sequence<T>* seq, size_t count);

    template<class T>
    Sequence<T>* Splice(Sequence<T>* seq, size_t index, size_t count, Sequence<T>* insertSequence = nullptr);

    template<class T>
    Sequence<T>* Map(Sequence<T>* seq, std::function<T(T)> func);
    
    template<class T, class T2>
    Sequence<T2>* Map(Sequence<T>* seq, std::function<T2(T)> func, Sequence<T2>* proto);

    template<class T>
    Sequence<T>* Where(Sequence<T>* seq, std::function<bool(T)> predicate);

    template<class T, class T2>
    T2 Reduce(Sequence<T>* seq, std::function<T2(T2, T)> func);

    template<class T>
    Option<T> GetFirst(Sequence<T>* seq, std::function<bool(T)> func);

    template<class T>
    Option<T> GetLast(Sequence<T>* seq, std::function<bool(T)> func);
}

#include "SequenceUtils.tpp"
