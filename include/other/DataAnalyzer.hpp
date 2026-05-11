#pragma once

#include <cmath>
#include <stdexcept>
#include "Sequence.hpp"
#include "Interfaces.hpp"
#include "Option.hpp"

// Структура для возврата результата П-1
template <class T>
struct Stats {
    T min;
    T max;
    double avg;
};

template <class T>
class DataAnalyzer {
public:

    // =========================================================================
    // П-1. (min, max, avg) за 1 проход
    // =========================================================================
    static Stats<T> GetBasicStats(Sequence<T>* seq, const IGroup<T>& group) {
        if (seq == nullptr || seq->GetLength() == 0) {
            throw std::runtime_error("Sequence is empty");
        }

        T minVal = seq->Get(0);
        T maxVal = seq->Get(0);
        T sumVal = group.Zero();
        size_t count = seq->GetLength();

        // За один проход собираем всю информацию
        for (size_t i = 0; i < count; ++i) {
            T current = seq->Get(i);
            
            if (current < minVal) minVal = current;
            if (current > maxVal) maxVal = current;

            sumVal = group.Add(sumVal, current); // Складываем через IGroup
        }

        double avgVal = static_cast<double>(sumVal) / count;

        return Stats<T>{minVal, maxVal, avgVal};
    }

    // =========================================================================
    // П-2. Медиана
    // =========================================================================
    static double GetMedian(Sequence<T>* seq) {
        size_t length = seq->GetLength();
        if (seq == nullptr || length == 0) {
            throw std::runtime_error("Sequence is empty");
        }

        // Извлекаем элементы для сортировки
        T* tempArray = new T[length];
        for (size_t i = 0; i < length; ++i) {
            tempArray[i] = seq->Get(i);
        }

        // Простая сортировка вставками для надежности (или std::sort)
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

    // =========================================================================
    // П-3. Кол-во перестановок (количество инверсий для сортировки)
    // =========================================================================
    static size_t GetInversionsCount(Sequence<T>* seq) {
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

    // =========================================================================
    // П-4. Для каждого элемента - множество предшествующих и меньших
    // =========================================================================
    static Sequence<Sequence<T>*>* GetPrecedingSmaller(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq, Sequence<T>* emptyProto) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();

        for (size_t i = 0; i < length; ++i) {
            Sequence<T>* subSeq = emptyProto->Skip(emptyProto->GetLength());
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

    // =========================================================================
    // П-5. Список всех префиксов / постфиксов
    // =========================================================================
    static Sequence<Sequence<T>*>* GetPrefixesAndPostfixes(Sequence<T>* seq, Sequence<Sequence<T>*>* outSeq) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();

        // 1. Извлекаем все префиксы
        for (size_t i = 1; i <= length; ++i) {
            outSeq = outSeq->Append(seq->GetSubsequence(0, i - 1));
        }

        // 2. Извлекаем все постфиксы
        for (size_t i = 0; i < length; ++i) {
            outSeq = outSeq->Append(seq->GetSubsequence(i, length - 1));
        }

        return outSeq;
    }

    // =========================================================================
    // П-6. Скользящее среднее (a_{i-1} + a_i + a_{i+1}) / 3
    // =========================================================================
    static Sequence<double>* GetMovingAverage(Sequence<T>* seq, const IRing<T>& ring, Sequence<double>* outSeq) {
        if (seq == nullptr || outSeq == nullptr) return outSeq;
        size_t length = seq->GetLength();
        if (length < 3) return outSeq; // Недостаточно элементов

        for (size_t i = 1; i < length - 1; ++i) {
            T prev = seq->Get(i - 1);
            T curr = seq->Get(i);
            T next = seq->Get(i + 1);

            // Складываем элементы через интерфейс кольца/группы
            T sum = ring.Add(ring.Add(prev, curr), next);
            double movingAvg = static_cast<double>(sum) / 3.0;

            outSeq = outSeq->Append(movingAvg);
        }
        return outSeq;
    }

    // =========================================================================
    // П-7. Для каждого элемента: sqrt(sigma^2 - a_i^2), где sigma - СКО
    // =========================================================================
    static Sequence<double>* GetSigmaDifference(Sequence<T>* seq, const IRing<T>& ring, Sequence<double>* outSeq) {
        if (seq == nullptr || outSeq == nullptr || seq->GetLength() == 0) return outSeq;
        size_t length = seq->GetLength();

        // 1. Считаем среднее арифметическое
        T sum = ring.Zero();
        for (size_t i = 0; i < length; ++i) {
            sum = ring.Add(sum, seq->Get(i));
        }
        double mean = static_cast<double>(sum) / length;

        // 2. Считаем дисперсию (variance) и среднеквадратичное отклонение (sigma)
        double varianceSum = 0;
        for (size_t i = 0; i < length; ++i) {
            double diff = static_cast<double>(seq->Get(i)) - mean;
            varianceSum += diff * diff;
        }
        double sigmaSquared = varianceSum / length;

        // 3. Вычисляем sqrt(sigma^2 - a_i^2) для каждого элемента
        for (size_t i = 0; i < length; ++i) {
            double currentVal = static_cast<double>(seq->Get(i));
            double underRoot = sigmaSquared - (currentVal * currentVal);

            double result = (underRoot >= 0.0) ? std::sqrt(underRoot) : 0.0;
            outSeq = outSeq->Append(result);
        }

        return outSeq;
    }

    // П-8. Поэлементная сумма списка с его отражением (a_i + a_{n-1-i})
    static Sequence<T>* GetReflectedSum(Sequence<T>* seq, const IGroup<T>& group, Sequence<T>* outSeq) {
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
};