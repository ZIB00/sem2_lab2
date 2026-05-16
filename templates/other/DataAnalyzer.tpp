namespace DataAnalyzer {
    template<class T>
    Stats<T> GetMinMaxAvg(Sequence<T>* seq, IGroup<T>& group) {
        if (seq == nullptr || seq->GetLength() == 0) throw InvalidArgument("Sequence is empty");

        T minVal = seq->Get(0);
        T maxVal = seq->Get(0);
        T sumVal = group.Zero();
        size_t count = seq->GetLength();

        for (size_t i = 0; i < count; ++i) {
            T current = seq->Get(i);
            
            if (current < minVal) minVal = current;
            if (current > maxVal) maxVal = current;

            sumVal = group.Add(sumVal, current);
        }

        double avgVal = static_cast<double>(sumVal) / count;

        return Stats<T>{minVal, maxVal, avgVal};
    }

    template<class T>
    Stats<T> GetMinMaxAvg(Sequence<T>* seq) {
        DefaultRing<T> defaultGroup;
        return GetMinMaxAvg(seq, defaultGroup);
    }

    template<class T>
    double GetMedian(Sequence<T>* seq) {
        size_t length = seq->GetLength();
        if (seq == nullptr || length == 0) {
            throw InvalidArgument("Sequence is empty");
        }

        T* tempArray = new T[length];
        for (size_t i = 0; i < length; ++i) {
            tempArray[i] = seq->Get(i);
        }

        for (size_t i = 1; i < length; ++i) {
            T key = tempArray[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0 && tempArray[j] > key) {
                tempArray[j + 1] = tempArray[j];
                j--;
            }
            tempArray[j + 1] = key;
        }

        double median;
        if (length % 2 != 0) {
            median = static_cast<double>(tempArray[length / 2]);
        } else {
            median = static_cast<double>(tempArray[length / 2 - 1] + tempArray[length / 2]) / 2.0;
        }

        delete[] tempArray;
        return median;
    }

    template<class T>
    size_t GetInversionsCount(Sequence<T>* seq) {
        if (seq == nullptr) return 0;
        size_t inversions = 0;
        size_t length = seq->GetLength();

        for (size_t i = 0; i < length; ++i) {
            for (size_t j = i + 1; j < length; ++j) {
                if (seq->Get(i) > seq->Get(j)) {
                    inversions++;
                }
            }
        }
        return inversions;
    }

    template<class T>
    Sequence<Sequence<T>*>* GetPrecedingSmaller(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq, Sequence<T>* emptyProto) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();

        for (size_t i = 0; i < length; ++i) {
            Sequence<T>* subSeq = emptyProto->CreateEmpty();
            T current = seq->Get(i);

            for (size_t j = 0; j < i; ++j) {
                if (seq->Get(j) < current) {
                    subSeq = subSeq->Append(seq->Get(j));
                }
            }
            outSeq = outSeq->Append(subSeq);
        }
        return outSeq;
    }

    template<class T>
    Sequence<Sequence<T>*>* GetPrefixesAndPostfixes(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();

        for (size_t i = 1; i <= length; ++i) {
            outSeq = outSeq->Append(seq->GetSubsequence(0, i - 1));
        }

        for (size_t i = 0; i < length; ++i) {
            outSeq = outSeq->Append(seq->GetSubsequence(i, length - 1));
        }

        return outSeq;
    }

    template<class T>
    Sequence<double>* GetMovingAverage(Sequence<T>* seq, IRing<T>& ring, Sequence<double>* outSeq) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();
        if (length < 3) return outSeq;

        for (size_t i = 1; i < length - 1; ++i) {
            T prev = seq->Get(i - 1);
            T curr = seq->Get(i);
            T next = seq->Get(i + 1);

            T sum = ring.Add(ring.Add(prev, curr), next);
            double movingAvg = static_cast<double>(sum) / 3.0;

            outSeq = outSeq->Append(movingAvg);
        }
        return outSeq;
    }

    template<class T>
    Sequence<double>* GetMovingAverage(Sequence<T>* seq, Sequence<double>* outSeq) {
        DefaultRing<T> defaultRing;
        return GetMovingAverage(seq, defaultRing, outSeq);
    }

    template<class T>
    Sequence<double>* GetSigmaDifference(Sequence<T>* seq, IRing<T>& ring, Sequence<double>* outSeq) {
        if (seq == nullptr || outSeq == nullptr || seq->GetLength() == 0) return outSeq;
        size_t length = seq->GetLength();

        T sum = ring.Zero();
        for (size_t i = 0; i < length; ++i) {
            sum = ring.Add(sum, seq->Get(i));
        }
        double mean = static_cast<double>(sum) / length;

        double varianceSum = 0;
        for (size_t i = 0; i < length; ++i) {
            double diff = static_cast<double>(seq->Get(i)) - mean;
            varianceSum += diff * diff;
        }
        double sigmaSquared = varianceSum / length;

        for (size_t i = 0; i < length; ++i) {
            double currentVal = static_cast<double>(seq->Get(i));
            double underRoot = sigmaSquared - (ring.Multiply(currentVal, currentVal));

            double result = (underRoot >= 0.0) ? std::sqrt(underRoot) : 0.0;
            outSeq = outSeq->Append(result);
        }

        return outSeq;
    }

    template<class T>
    Sequence<double>* GetSigmaDifference(Sequence<T>* seq, Sequence<double>* outSeq) {
        DefaultRing<T> defaultRing;
        return GetSigmaDifference(seq, defaultRing, outSeq);
    }

    template<class T>
    Sequence<T>* GetReflectedSum(Sequence<T>* seq, IGroup<T>& group, Sequence<T>* outSeq) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();

        for (size_t i = 0; i < length; ++i) {
            T normalElem = seq->Get(i);
            T reflectedElem = seq->Get(length - 1 - i);

            T sum = group.Add(normalElem, reflectedElem);
            outSeq = outSeq->Append(sum);
        }

        return outSeq;
    }

    template<class T>
    Sequence<T>* GetReflectedSum(Sequence<T>* seq, Sequence<T>* outSeq) {
        DefaultRing<T> defaultRing;
        return GetReflectedSum(seq, defaultRing, outSeq);
    }
}