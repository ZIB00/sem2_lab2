#pragma once

#include "Sequence.hpp"
#include "IEnumerator.hpp"

namespace SequenceUtils {

    template<class T>
    Pair<Sequence<T>*, Sequence<T>*> Unzip(Sequence<Pair<T, T>>* pairsSeq, Sequence<T>* sequence1, Sequence<T>* sequence2);

    template<class T>
    Sequence<T>* Range(T start, T end, T step, Sequence<T>* sequence);

    // =========================================================================
    // ZipEnumerator — ленивый итератор по двум последовательностям.
    // Останавливается когда заканчивается более короткая (как Python zip).
    // =========================================================================
    template<class T>
    class ZipEnumerator : public IEnumerator<Pair<T, T>>
    {
    private:
        IEnumerator<T>* enumA;
        IEnumerator<T>* enumB;
        Pair<T, T> current;

    public:
        ZipEnumerator(IEnumerator<T>* a, IEnumerator<T>* b)
            : enumA(a), enumB(b) {}

        ~ZipEnumerator() override
        {
            delete enumA;
            delete enumB;
        }

        Pair<T, T> GetCurrent() override { return current; }

        bool MoveNext() override
        {
            if (!enumA->MoveNext() || !enumB->MoveNext()) return false;
            current = Pair<T, T>(enumA->GetCurrent(), enumB->GetCurrent());
            return true;
        }

        void Reset() override
        {
            enumA->Reset();
            enumB->Reset();
        }
    };

    // Возвращает связанный итератор пар — никакой промежуточной коллекции
    template<class T>
    ZipEnumerator<T>* Zip(Sequence<T>* a, Sequence<T>* b);

    // =========================================================================
    // SplitEnumerator — ленивый итератор по частям.
    // Каждый MoveNext() собирает следующую часть до следующего разделителя.
    // prototype — пустая последовательность нужного типа (владение у вызывающего)
    // =========================================================================
    template<class T>
    class SplitEnumerator : public IEnumerator<Sequence<T>*>
    {
    private:
        IEnumerator<T>* source;
        bool (*predicate)(T);
        Sequence<T>* prototype;
        Sequence<T>* current;
        bool done;

    public:
        SplitEnumerator(IEnumerator<T>* src, bool (*pred)(T), Sequence<T>* proto)
            : source(src), predicate(pred), prototype(proto), current(nullptr), done(false) {}

        ~SplitEnumerator() override
        {
            delete source;
        }

        Sequence<T>* GetCurrent() override { return current; }

        bool MoveNext() override
        {
            if (done) return false;

            // Пустая часть того же типа через Skip всей длины прототипа
            Sequence<T>* part = prototype->Skip(prototype->GetLength());

            bool hitSeparator = false;
            while (source->MoveNext()) {
                T val = source->GetCurrent();
                if (predicate(val)) {
                    hitSeparator = true;
                    break;
                }
                part = part->Append(val);
            }

            current = part;
            if (!hitSeparator) done = true;

            return true;
        }

        void Reset() override
        {
            source->Reset();
            current = nullptr;
            done = false;
        }
    };

    // Возвращает связанный итератор частей — никакой промежуточной коллекции
    template<class T>
    SplitEnumerator<T>* Split(Sequence<T>* seq, bool (*predicate)(T), Sequence<T>* prototype);

}

#include "SequenceUtils.tpp"
