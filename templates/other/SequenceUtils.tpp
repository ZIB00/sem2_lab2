#include "SequenceUtils.hpp"

namespace SequenceUtils {

    template<class T>
    Pair<Sequence<T>*, Sequence<T>*> Unzip(Sequence<Pair<T, T>>* pairsSeq, Sequence<T>* sequence1, Sequence<T>* sequence2)
    {
        try {
            IEnumerator<Pair<T, T>>* iterator = pairsSeq->GetEnumerator();
            while (iterator->MoveNext()) {
                Pair<T, T> currentPair = iterator->GetCurrent();
                sequence1 = sequence1->Append(currentPair.first);
                sequence2 = sequence2->Append(currentPair.second);
            }
            delete iterator;
        } catch (...) {
            throw;
        }
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

}