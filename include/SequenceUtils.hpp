#pragma once

#include "Sequence.hpp"

namespace SequenceUtils {

    template<class T>
    Pair<Sequence<T>*, Sequence<T>*> Unzip(Sequence<Pair<T, T>>* pairsSeq, Sequence<T>* sequence1, Sequence<T>* sequence2);

    template<class T>
    Sequence<T>* Range(T start, T end, T step, Sequence<T>* sequence);

}