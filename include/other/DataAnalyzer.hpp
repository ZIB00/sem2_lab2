#pragma once

#include <cmath>
#include <stdexcept>
#include "Sequence.hpp"
#include "Implementation.hpp"
#include "Option.hpp"

namespace DataAnalyzer {
    template <class T>
    struct Stats {
        T min;
        T max;
        double avg;
    };
    
    template <class T>
    Stats<T> GetMinMaxAvg(Sequence<T>* seq, IGroup<T>& group);

    //Для DefaultRing
    template <class T>
    Stats<T> GetMinMaxAvg(Sequence<T>* seq);

    template <class T>
    double GetMedian(Sequence<T>* seq);

    template <class T>
    size_t GetInversionsCount(Sequence<T>* seq);

    template <class T>
    Sequence<Sequence<T>*>* GetPrecedingSmaller(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq, Sequence<T>* emptyProto);
    
    template <class T>
    Sequence<Sequence<T>*>* GetPrefixesAndPostfixes(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq);
    
    template <class T>
    Sequence<double>* GetMovingAverage(Sequence<T>* seq, IRing<T>& ring, Sequence<double>* outSeq);
    
    //Для DefaultRing
    template <class T>
    Sequence<double>* GetMovingAverage(Sequence<T>* seq, Sequence<double>* outSeq);
    
    template <class T>
    Sequence<double>* GetSigmaDifference(Sequence<T>* seq, IRing<T>& ring, Sequence<double>* outSeq);
    
    //Для DefaultRing
    template <class T>
    Sequence<double>* GetSigmaDifference(Sequence<T>* seq, Sequence<double>* outSeq);
    
    template <class T>
    Sequence<T>* GetReflectedSum(Sequence<T>* seq, IGroup<T>& group, Sequence<T>* outSeq);
    
    //Для DefaultRing
    template <class T>
    Sequence<T>* GetReflectedSum(Sequence<T>* seq, Sequence<T>* outSeq);
}

#include "DataAnalyzer.tpp"