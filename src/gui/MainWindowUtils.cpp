#include "MainWindow.hpp"

void MainWindow::onZip() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || !intAdditionalSeq) { globalLog->append("Zip: нужны Основная и Дополнительная."); return; }
            QString result = "Zip результат: ";
            size_t minLen = std::min(intMainSeq->GetLength(), intAdditionalSeq->GetLength());
            for (size_t i = 0; i < minLen; i++) {
                result += "{" + QString::number(intMainSeq->Get(i)) + ", " + QString::number(intAdditionalSeq->Get(i)) + "} ";
            }
            globalLog->append(result.trimmed());
        } else if (type == 1) {
            if (!doubleMainSeq || !doubleAdditionalSeq) { globalLog->append("Zip: нужны Основная и Дополнительная."); return; }
            QString result = "Zip результат: ";
            size_t minLen = std::min(doubleMainSeq->GetLength(), doubleAdditionalSeq->GetLength());
            for (size_t i = 0; i < minLen; i++) {
                result += "{" + QString::number(doubleMainSeq->Get(i)) + ", " + QString::number(doubleAdditionalSeq->Get(i)) + "} ";
            }
            globalLog->append(result.trimmed());
        } else {
            if (!charMainSeq || !charAdditionalSeq) { globalLog->append("Zip: нужны Основная и Дополнительная."); return; }
            QString result = "Zip результат: ";
            size_t minLen = std::min(charMainSeq->GetLength(), charAdditionalSeq->GetLength());
            for (size_t i = 0; i < minLen; i++) {
                QString mainChar = QString(QChar(static_cast<ushort>(charMainSeq->Get(i))));
                QString addChar = QString(QChar(static_cast<ushort>(charAdditionalSeq->Get(i))));
                result += "{" + mainChar + ", " + addChar + "} ";
            }
            globalLog->append(result.trimmed());
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onUnzip() {
    try {
        if (!intMainSeq || intMainSeq->GetLength() == 0) {
            globalLog->append("Unzip: нужна непустая последовательность целых чисел.");
            return;
        }

        MutableArraySequence<Pair<int, int>>* pairsSeq = new MutableArraySequence<Pair<int, int>>();
        size_t pairCount = intMainSeq->GetLength() / 2;

        for (size_t i = 0; i < pairCount; i++) {
            Pair<int, int> pair;
            pair.first = intMainSeq->Get(i * 2);
            pair.second = intMainSeq->Get(i * 2 + 1);
            pairsSeq->Append(pair);
        }

        MutableArraySequence<int>* leftSeq = new MutableArraySequence<int>();
        MutableArraySequence<int>* rightSeq = new MutableArraySequence<int>();

        SequenceUtils::Unzip(pairsSeq, leftSeq, rightSeq);

        QString leftResult = "Unzip left: ";
        QString rightResult = "Unzip right: ";

        for (size_t i = 0; i < leftSeq->GetLength(); i++) { leftResult += QString::number(leftSeq->Get(i)) + " "; }
        for (size_t i = 0; i < rightSeq->GetLength(); i++) { rightResult += QString::number(rightSeq->Get(i)) + " "; }

        globalLog->append(leftResult.trimmed());
        globalLog->append(rightResult.trimmed());

        delete pairsSeq; delete leftSeq; delete rightSeq;
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onRange() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        QStringList params = rightLineEditUtilts->text().split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);

        if (params.size() < 3) { 
            throw InvalidArgument("Ошибка Range: введите три числа через пробел (начало конец шаг)."); 
        }

        if (type == 0) {
            int startVal, endVal, stepVal;
            ParseStringToken(params.at(0), startVal);
            ParseStringToken(params.at(1), endVal);
            ParseStringToken(params.at(2), stepVal);
            if (stepVal == 0) { throw InvalidArgument("Ошибка Range: шаг диапазона не может быть равен 0."); }

            MutableArraySequence<int>* rangeSeq = new MutableArraySequence<int>();
            Sequence<int>* result = SequenceUtils::Range(startVal, endVal, stepVal, rangeSeq);
            if (intMainSeq && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            double startVal, endVal, stepVal;
            ParseStringToken(params.at(0), startVal);
            ParseStringToken(params.at(1), endVal);
            ParseStringToken(params.at(2), stepVal);
            if (stepVal == 0.0) { throw InvalidArgument("Ошибка Range: шаг диапазона не может быть равен 0."); }

            MutableArraySequence<double>* rangeSeq = new MutableArraySequence<double>();
            Sequence<double>* result = SequenceUtils::Range(startVal, endVal, stepVal, rangeSeq);
            if (doubleMainSeq && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            globalLog->append("Range: поддерживается только для числовых типов."); 
            return;
        }

        globalLog->append("Range сгенерирован и помещен в Основную.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSplit() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<int> proto;
            auto* enumerator = SequenceUtils::Split(intMainSeq, std::function<bool(int)>([](int x) { return x == 0; }), &proto);

            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<int>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) { segmentText += QString::number(segment->Get(i)) + " "; }
                globalLog->append(segmentText.trimmed());
            }
            delete enumerator;
            if (segmentIndex == 0) { globalLog->append("Split: все элементы равны 0, нет сегментов."); }

        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<double> proto;
            auto* enumerator = SequenceUtils::Split(doubleMainSeq, std::function<bool(double)>([](double x) { return x == 0.0; }), &proto);

            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<double>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) { segmentText += QString::number(segment->Get(i)) + " "; }
                globalLog->append(segmentText.trimmed());
            }
            delete enumerator;

        } else {
            if (!charMainSeq || charMainSeq->GetLength() == 0) { globalLog->append("Split: последовательность пуста."); return; }
            MutableArraySequence<char16_t> proto;
            auto* enumerator = SequenceUtils::Split(charMainSeq, std::function<bool(char16_t)>([](char16_t x) { return x == u'_'; }), &proto);

            int segmentIndex = 0;
            while (enumerator->MoveNext()) {
                Sequence<char16_t>* segment = enumerator->GetCurrent();
                QString segmentText = "Split сегмент " + QString::number(segmentIndex++) + ": ";
                for (size_t i = 0; i < segment->GetLength(); i++) { segmentText += QChar(segment->Get(i)); }
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
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) { globalLog->append("FlatMap: ошибка данных."); return; }
            Sequence<int>* baseSeq = isMutable ? intMainSeq : intOriginalSeq;
            std::function<Sequence<int>*(int)> expandFn = [](int x) -> Sequence<int>* {
                MutableArraySequence<int>* chunk = new MutableArraySequence<int>();
                chunk->Append(x); chunk->Append(x * 2); return chunk;
            };
            Sequence<int>* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;

        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) { globalLog->append("FlatMap: ошибка данных."); return; }
            Sequence<double>* baseSeq = isMutable ? doubleMainSeq : doubleOriginalSeq;
            std::function<Sequence<double>*(double)> expandFn = [](double x) -> Sequence<double>* {
                MutableArraySequence<double>* chunk = new MutableArraySequence<double>();
                chunk->Append(x); chunk->Append(x * 2.0); return chunk;
            };
            Sequence<double>* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;

        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) { globalLog->append("FlatMap: ошибка данных."); return; }
            Sequence<char16_t>* baseSeq = isMutable ? charMainSeq : charOriginalSeq;
            std::function<Sequence<char16_t>*(char16_t)> expandFn = [](char16_t x) -> Sequence<char16_t>* {
                MutableArraySequence<char16_t>* chunk = new MutableArraySequence<char16_t>();
                chunk->Append(x); chunk->Append(static_cast<char16_t>(x + 1)); return chunk;
            };
            Sequence<char16_t>* result = SequenceUtils::FlatMap(baseSeq, expandFn);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("FlatMap: каждый элемент x -> [x, x*2]");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSkip() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString countText = rightLineEditUtilts->text().trimmed();
        
        int countInt;
        ParseStringToken(countText, countInt);
        if (countInt < 0) {
            throw InvalidArgument("Ошибка Skip: количество пропускаемых элементов не может быть отрицательным.");
        }
        size_t count = static_cast<size_t>(countInt);

        if (type == 0 && intMainSeq) {
            Sequence<int>* baseSeq = isMutable ? intMainSeq : intOriginalSeq;
            Sequence<int>* result = SequenceUtils::Skip(baseSeq, count);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1 && doubleMainSeq) {
            Sequence<double>* baseSeq = isMutable ? doubleMainSeq : doubleOriginalSeq;
            Sequence<double>* result = SequenceUtils::Skip(baseSeq, count);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else if (type == 2 && charMainSeq) {
            Sequence<char16_t>* baseSeq = isMutable ? charMainSeq : charOriginalSeq;
            Sequence<char16_t>* result = SequenceUtils::Skip(baseSeq, count);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена утилита Skip.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSplice() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString indexText = rightLineEditUtilts->text().trimmed();

        if (indexText.isEmpty()) { globalLog->append("Splice: введите индекс."); return; }

        bool ok = false;
        size_t targetIndex = indexText.toUInt(&ok);
        if (!ok) { globalLog->append("Splice: индекс должен быть неотрицательным целым числом."); return; }

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            Sequence<int>* baseSeq = isMutable ? intMainSeq : intOriginalSeq;
            if (targetIndex >= baseSeq->GetLength()) { globalLog->append("Splice: индекс вне диапазона."); return; }
            
            Sequence<int>* result = SequenceUtils::Splice(baseSeq, targetIndex, 1);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) return;
            Sequence<double>* baseSeq = isMutable ? doubleMainSeq : doubleOriginalSeq;
            if (targetIndex >= baseSeq->GetLength()) { globalLog->append("Splice: индекс вне диапазона."); return; }
            
            Sequence<double>* result = SequenceUtils::Splice(baseSeq, targetIndex, 1);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            Sequence<char16_t>* baseSeq = isMutable ? charMainSeq : charOriginalSeq;
            if (targetIndex >= baseSeq->GetLength()) { globalLog->append("Splice: индекс вне диапазона."); return; }
            
            Sequence<char16_t>* result = SequenceUtils::Splice(baseSeq, targetIndex, 1);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Splice: удалён 1 элемент с индекса " + QString::number(targetIndex));
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onMap() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0 && intMainSeq) {
            Sequence<int>* baseSeq = isMutable ? intMainSeq : intOriginalSeq;
            Sequence<int>* result = SequenceUtils::Map<int>(baseSeq, [](int element) { return element * 2; });
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1 && doubleMainSeq) {
            Sequence<double>* baseSeq = isMutable ? doubleMainSeq : doubleOriginalSeq;
            Sequence<double>* result = SequenceUtils::Map<double>(baseSeq, [](double element) { return element * 2.0; });
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else if (type == 2 && charMainSeq) {
            Sequence<char16_t>* baseSeq = isMutable ? charMainSeq : charOriginalSeq;
            Sequence<char16_t>* result = SequenceUtils::Map<char16_t>(baseSeq, [](char16_t element) { return static_cast<char16_t>(element + 1); });
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена утилита Map (умножение на 2 или сдвиг символа).");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onWhere() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0 && intMainSeq) {
            Sequence<int>* baseSeq = isMutable ? intMainSeq : intOriginalSeq;
            Sequence<int>* result = SequenceUtils::Where<int>(baseSeq, [](int element) { return element >= 0; });
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1 && doubleMainSeq) {
            Sequence<double>* baseSeq = isMutable ? doubleMainSeq : doubleOriginalSeq;
            Sequence<double>* result = SequenceUtils::Where<double>(baseSeq, [](double element) { return element >= 0.0; });
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else if (type == 2 && charMainSeq) {
            Sequence<char16_t>* baseSeq = isMutable ? charMainSeq : charOriginalSeq;
            Sequence<char16_t>* result = SequenceUtils::Where<char16_t>(baseSeq, [](char16_t element) { return element != 0; });
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена фильтрация Where (оставлены только неотрицательные элементы).");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onReduce() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0 && intMainSeq) {
            int total = SequenceUtils::Reduce<int, int>(intMainSeq, [](int accumulator, int element) { return accumulator + element; });
            globalLog->append("Результат Reduce (сумма): " + QString::number(total));
        } else if (type == 1 && doubleMainSeq) {
            double total = SequenceUtils::Reduce<double, double>(doubleMainSeq, [](double accumulator, double element) { return accumulator + element; });
            globalLog->append("Результат Reduce (сумма): " + QString::number(total));
        } else {
            globalLog->append("Reduce не поддерживается для выбранного типа.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onGetFirstOpt() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        QString thresholdText = rightLineEditUtilts->text().trimmed();

        if (type == 0 && intMainSeq) {
            int threshold;
            ParseStringToken(thresholdText, threshold);
            Option<int> result = SequenceUtils::GetFirst<int>(intMainSeq, [threshold](int x) { return x >= threshold; });
            if (result.HasValue()) { globalLog->append("GetFirst >= " + QString::number(threshold) + " -> " + QString::number(result.GetValue())); } 
            else { globalLog->append("GetFirst >= " + QString::number(threshold) + " -> Option::None (не найдено)."); }
        } else if (type == 1 && doubleMainSeq) {
            double threshold;
            ParseStringToken(thresholdText, threshold);
            Option<double> result = SequenceUtils::GetFirst<double>(doubleMainSeq, [threshold](double x) { return x >= threshold; });
            if (result.HasValue()) { globalLog->append("GetFirst >= " + QString::number(threshold) + " -> " + QString::number(result.GetValue())); } 
            else { globalLog->append("GetFirst >= " + QString::number(threshold) + " -> Option::None (не найдено)."); }
        } else if (type == 2 && charMainSeq) {
            char16_t threshold;
            ParseStringToken(thresholdText, threshold);
            Option<char16_t> result = SequenceUtils::GetFirst<char16_t>(charMainSeq, [threshold](char16_t x) { return x >= threshold; });
            if (result.HasValue()) { globalLog->append("GetFirst >= '" + QString(QChar(threshold)) + "' -> '" + QString(QChar(result.GetValue())) + "'"); } 
            else { globalLog->append("GetFirst -> Option::None (не найдено)."); }
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onGetLastOpt() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        QString thresholdText = rightLineEditUtilts->text().trimmed();

        if (type == 0 && intMainSeq) {
            int threshold;
            ParseStringToken(thresholdText, threshold);
            Option<int> result = SequenceUtils::GetLast<int>(intMainSeq, [threshold](int x) { return x <= threshold; });
            if (result.HasValue()) { globalLog->append("GetLast <= " + QString::number(threshold) + " -> " + QString::number(result.GetValue())); } 
            else { globalLog->append("GetLast <= " + QString::number(threshold) + " -> Option::None (не найдено)."); }
        } else if (type == 1 && doubleMainSeq) {
            double threshold;
            ParseStringToken(thresholdText, threshold);
            Option<double> result = SequenceUtils::GetLast<double>(doubleMainSeq, [threshold](double x) { return x <= threshold; });
            if (result.HasValue()) { globalLog->append("GetLast <= " + QString::number(threshold) + " -> " + QString::number(result.GetValue())); } 
            else { globalLog->append("GetLast <= " + QString::number(threshold) + " -> Option::None (не найдено)."); }
        } else if (type == 2 && charMainSeq) {
            char16_t threshold;
            ParseStringToken(thresholdText, threshold);
            Option<char16_t> result = SequenceUtils::GetLast<char16_t>(charMainSeq, [threshold](char16_t x) { return x <= threshold; });
            if (result.HasValue()) { globalLog->append("GetLast <= '" + QString(QChar(threshold)) + "' -> '" + QString(QChar(result.GetValue())) + "'"); } 
            else { globalLog->append("GetLast -> Option::None (не найдено)."); }
        }
    } CATCH_ALL_EXCEPTIONS
}