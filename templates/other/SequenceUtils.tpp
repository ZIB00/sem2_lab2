#include "SequenceUtils.hpp"

namespace SequenceUtils {

    template<class T>
    Pair<Sequence<T>*, Sequence<T>*> Unzip(Sequence<Pair<T, T>>* pairsSeq, Sequence<T>* sequence1, Sequence<T>* sequence2)
    {
        if (pairsSeq == nullptr || sequence1 == nullptr || sequence2 == nullptr)
            throw InvalidArgument("Arguments cannot be null");

        IEnumerator<Pair<T, T>>* iterator = pairsSeq->GetEnumerator();
        try {
            while (iterator->MoveNext()) {
                Pair<T, T> current = iterator->GetCurrent();
                sequence1 = sequence1->Append(current.first);
                sequence2 = sequence2->Append(current.second);
            }
        } catch (...) {
            delete iterator;
            throw;
        }
        delete iterator;
        return Pair<Sequence<T>*, Sequence<T>*>(sequence1, sequence2);
    }

    template<class T>
    Sequence<T>* Range(T start, T end, T step, Sequence<T>* sequence)
    {
        if (sequence == nullptr) throw InvalidArgument("Output sequence cannot be null");
        if (step == 0) throw InvalidArgument("Step cannot be zero");

        if (step > 0) {
            for (T i = start; i <= end; i += step) sequence = sequence->Append(i);
        } else {
            for (T i = start; i >= end; i += step) sequence = sequence->Append(i);
        }

        return sequence;
    }

    template<class T>
    ZipEnumerator<T>* Zip(Sequence<T>* a, Sequence<T>* b)
    {
        if (a == nullptr || b == nullptr)
            throw InvalidArgument("Sequences cannot be null");

        return new ZipEnumerator<T>(a->GetEnumerator(), b->GetEnumerator());
    }

    template<class T>
    SplitEnumerator<T>* Split(Sequence<T>* seq, bool (*predicate)(T), Sequence<T>* prototype)
    {
        if (seq == nullptr || predicate == nullptr || prototype == nullptr)
            throw InvalidArgument("Arguments cannot be null");

        return new SplitEnumerator<T>(seq->GetEnumerator(), predicate, prototype);
    }

}