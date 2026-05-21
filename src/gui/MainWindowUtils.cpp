#include "MainWindow.hpp"

void MainWindow::onZip() {
    try {
        UniversalLambda([&](auto& state) {
            if (!state.main || !state.additional) { 
                globalLog->append("Zip: нужны Основная и Дополнительная."); 
                return; 
            }
            QString result = "Zip результат: ";
            size_t minLen = std::min(state.main->GetLength(), state.additional->GetLength());
            for (size_t i = 0; i < minLen; i++) {
                result += "{" + ElementToString(state.main->Get(i)) + ", " + ElementToString(state.additional->Get(i)) + "} ";
            }
            globalLog->append(result.trimmed());
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onUnzip() {
    try {
        if (!seqInt.main || seqInt.main->GetLength() == 0) {
            globalLog->append("Unzip: нужна непустая последовательность целых чисел.");
            return;
        }

        MutableArraySequence<Pair<int, int>>* pairsSeq = new MutableArraySequence<Pair<int, int>>();
        size_t pairCount = seqInt.main->GetLength() / 2;
        for (size_t i = 0; i < pairCount; i++) {
            Pair<int, int> pair;
            pair.first = seqInt.main->Get(i * 2);
            pair.second = seqInt.main->Get(i * 2 + 1);
            pairsSeq->Append(pair);
        }

        MutableArraySequence<int>* leftSeq = new MutableArraySequence<int>();
        MutableArraySequence<int>* rightSeq = new MutableArraySequence<int>();
        SequenceUtils::Unzip(pairsSeq, leftSeq, rightSeq);

        QString leftResult = "Sequence 1: ";
        QString rightResult = "Sequence 2: ";
        for (size_t i = 0; i < leftSeq->GetLength(); i++) leftResult += QString::number(leftSeq->Get(i)) + " ";
        for (size_t i = 0; i < rightSeq->GetLength(); i++) rightResult += QString::number(rightSeq->Get(i)) + " ";

        globalLog->append(leftResult.trimmed());
        globalLog->append(rightResult.trimmed());
        delete pairsSeq; delete leftSeq; delete rightSeq;
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onRange() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        QStringList params = rightLineEditUtilts->text().split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
        if (params.size() < 3) throw InvalidArgument("Ошибка Range: введите три числа через пробел."); 

        if (type == 0) {
            int startVal, endVal, stepVal;
            ParseStringToken(params.at(0), startVal); ParseStringToken(params.at(1), endVal); ParseStringToken(params.at(2), stepVal);
            if (stepVal == 0) throw InvalidArgument("Ошибка Range: шаг не может быть равен 0."); 

            auto* rangeSeq = new MutableArraySequence<int>();
            auto* result = SequenceUtils::Range(startVal, endVal, stepVal, rangeSeq);
            if (seqInt.main && seqInt.main != seqInt.original) delete seqInt.main;
            seqInt.main = result;
            globalLog->append("Range сгенерирован и помещен в Основную.");
        } else if (type == 1) {
            double startVal, endVal, stepVal;
            ParseStringToken(params.at(0), startVal); ParseStringToken(params.at(1), endVal); ParseStringToken(params.at(2), stepVal);
            if (stepVal == 0.0) throw InvalidArgument("Ошибка Range: шаг не может быть равен 0."); 

            auto* rangeSeq = new MutableArraySequence<double>();
            auto* result = SequenceUtils::Range(startVal, endVal, stepVal, rangeSeq);
            if (seqDouble.main && seqDouble.main != seqDouble.original) delete seqDouble.main;
            seqDouble.main = result;
            globalLog->append("Range сгенерирован и помещен в Основную.");
        } else {
            globalLog->append("Range: поддерживается только для числовых типов.");
        }
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSplit() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!seqInt.main || seqInt.main->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<int> proto;
            auto* enumerator = SequenceUtils::Split(seqInt.main, std::function<bool(int)>([](int x) { return x == 0; }), &proto);
            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<int>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) segmentText += ElementToString(segment->Get(i)) + " ";
                globalLog->append(segmentText.trimmed());
            }
            delete enumerator;
        } else if (type == 1) {
            if (!seqDouble.main || seqDouble.main->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<double> proto;
            auto* enumerator = SequenceUtils::Split(seqDouble.main, std::function<bool(double)>([](double x) { return x == 0.0; }), &proto);
            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<double>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) segmentText += ElementToString(segment->Get(i)) + " ";
                globalLog->append(segmentText.trimmed());
            }
            delete enumerator;
        } else {
            if (!seqChar.main || seqChar.main->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<char16_t> proto;
            auto* enumerator = SequenceUtils::Split(seqChar.main, std::function<bool(char16_t)>([](char16_t x) { return x == u'_'; }), &proto);
            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<char16_t>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) segmentText += ElementToString(segment->Get(i)) + " ";
                globalLog->append(segmentText.trimmed());
            }
            delete enumerator;
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onFlatMap() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!seqInt.main || (!isMutable && !seqInt.original)) return;
            auto* baseSeq = isMutable ? seqInt.main : seqInt.original;
            std::function<Sequence<int>*(int)> expandFn = [](int x) {
                auto* chunk = new MutableArraySequence<int>();
                chunk->Append(x); chunk->Append(x * 2); return chunk;
            };
            auto* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (seqInt.main != result && seqInt.main != seqInt.original) delete seqInt.main;
            seqInt.main = result;
        } else if (type == 1) {
            if (!seqDouble.main || (!isMutable && !seqDouble.original)) return;
            auto* baseSeq = isMutable ? seqDouble.main : seqDouble.original;
            std::function<Sequence<double>*(double)> expandFn = [](double x) {
                auto* chunk = new MutableArraySequence<double>();
                chunk->Append(x); chunk->Append(x * 2.0); return chunk;
            };
            auto* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (seqDouble.main != result && seqDouble.main != seqDouble.original) delete seqDouble.main;
            seqDouble.main = result;
        } else {
            if (!seqChar.main || (!isMutable && !seqChar.original)) return;
            auto* baseSeq = isMutable ? seqChar.main : seqChar.original;
            std::function<Sequence<char16_t>*(char16_t)> expandFn = [](char16_t x) {
                auto* chunk = new MutableArraySequence<char16_t>();
                chunk->Append(x); chunk->Append(static_cast<char16_t>(x + 1)); return chunk;
            };
            auto* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (seqChar.main != result && seqChar.main != seqChar.original) delete seqChar.main;
            seqChar.main = result;
        }
        globalLog->append("FlatMap выполнен.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSkip() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString countText = rightLineEditUtilts->text().trimmed();
        int countInt;
        ParseStringToken(countText, countInt);

        if (countInt < 0) throw InvalidArgument("Ошибка Skip: значение должно быть неотрицательным.");
        size_t count = static_cast<size_t>(countInt);

        UniversalLambda([&](auto& state) {
            if (state.main) {
                auto* baseSeq = isMutable ? state.main : state.original;
                auto* result = SequenceUtils::Skip(baseSeq, count);
                if (state.main != result && state.main != state.original) delete state.main;
                state.main = result;
            }
        });
        globalLog->append("Выполнена утилита Skip.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSplice() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString indexText = rightLineEditUtilts->text().trimmed();
        bool ok = false;
        size_t targetIndex = indexText.toUInt(&ok);
        if (!ok) { 
            globalLog->append("Splice: неверный индекс."); 
            return; 
        }

        UniversalLambda([&](auto& state) {
            if (!state.main || (!isMutable && !state.original)) return;
            auto* baseSeq = isMutable ? state.main : state.original;
            if (targetIndex >= baseSeq->GetLength()) { globalLog->append("Splice: индекс вне диапазона."); return; }
            auto* result = SequenceUtils::Splice(baseSeq, targetIndex, 1);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
            globalLog->append("Splice: удалён 1 элемент.");
        });
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onMap() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!seqInt.main) return;
            auto* baseSeq = isMutable ? seqInt.main : seqInt.original;
            auto* result = SequenceUtils::Map<int>(baseSeq, [](int e) { return e * 2; });
            if (seqInt.main != result && seqInt.main != seqInt.original) delete seqInt.main;
            seqInt.main = result;
        } else if (type == 1) {
            if (!seqDouble.main) return;
            auto* baseSeq = isMutable ? seqDouble.main : seqDouble.original;
            auto* result = SequenceUtils::Map<double>(baseSeq, [](double e) { return e * 2.0; });
            if (seqDouble.main != result && seqDouble.main != seqDouble.original) delete seqDouble.main;
            seqDouble.main = result;
        } else {
            if (!seqChar.main) return;
            auto* baseSeq = isMutable ? seqChar.main : seqChar.original;
            auto* result = SequenceUtils::Map<char16_t>(baseSeq, [](char16_t e) { return static_cast<char16_t>(e + 1); });
            if (seqChar.main != result && seqChar.main != seqChar.original) delete seqChar.main;
            seqChar.main = result;
        }
        globalLog->append("Выполнена утилита Map.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onWhere() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!seqInt.main) return;
            auto* baseSeq = isMutable ? seqInt.main : seqInt.original;
            auto* result = SequenceUtils::Where<int>(baseSeq, [](int e) { return e >= 0; });
            if (seqInt.main != result && seqInt.main != seqInt.original) delete seqInt.main;
            seqInt.main = result;
        } else if (type == 1) {
            if (!seqDouble.main) return;
            auto* baseSeq = isMutable ? seqDouble.main : seqDouble.original;
            auto* result = SequenceUtils::Where<double>(baseSeq, [](double e) { return e >= 0.0; });
            if (seqDouble.main != result && seqDouble.main != seqDouble.original) delete seqDouble.main;
            seqDouble.main = result;
        } else {
            if (!seqChar.main) return;
            auto* baseSeq = isMutable ? seqChar.main : seqChar.original;
            auto* result = SequenceUtils::Where<char16_t>(baseSeq, [](char16_t e) { return e != 0; });
            if (seqChar.main != result && seqChar.main != seqChar.original) delete seqChar.main;
            seqChar.main = result;
        }
        globalLog->append("Выполнена фильтрация Where.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onReduce() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (seqInt.main) {
                int total = SequenceUtils::Reduce<int, int>(seqInt.main, [](int acc, int e) { return acc + e; });
                globalLog->append("Результат Reduce (сумма): " + QString::number(total));
            }
        } else if (type == 1) {
            if (seqDouble.main) {
                double total = SequenceUtils::Reduce<double, double>(seqDouble.main, [](double acc, double e) { return acc + e; });
                globalLog->append("Результат Reduce (сумма): " + QString::number(total));
            }
        } else {
            globalLog->append("Reduce не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onGetFirstOpt() {
    try {
        QString thresholdText = rightLineEditUtilts->text().trimmed();
        UniversalLambda([&](auto& state) {
            if (!state.main) return;
            using T = typename std::decay_t<decltype(state)>::ValueType;
            T threshold; 
            ParseStringToken(thresholdText, threshold);
            Option<T> result = SequenceUtils::GetFirst<T>(state.main, [threshold](T x) { return x >= threshold; });
            if (result.HasValue()) globalLog->append("GetFirst >= " + ElementToString(threshold) + " -> " + ElementToString(result.Value()));
            else globalLog->append("GetFirst -> Option::None.");
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onGetLastOpt() {
    try {
        QString thresholdText = rightLineEditUtilts->text().trimmed();
        UniversalLambda([&](auto& state) {
            if (!state.main) return;
            using T = typename std::decay_t<decltype(state)>::ValueType;
            T threshold; ParseStringToken(thresholdText, threshold);
            Option<T> result = SequenceUtils::GetLast<T>(state.main, [threshold](T x) { return x <= threshold; });
            if (result.HasValue()) globalLog->append("GetLast <= " + ElementToString(threshold) + " -> " + ElementToString(result.Value()));
            else globalLog->append("GetLast -> Option::None.");
        });
    } CATCH_ALL_EXCEPTIONS
}