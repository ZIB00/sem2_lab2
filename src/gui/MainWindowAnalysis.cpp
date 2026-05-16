#include "MainWindow.hpp"

void MainWindow::onStats() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0 && intMainSeq) {
            DataAnalyzer::Stats<int> statistics = DataAnalyzer::GetMinMaxAvg(intMainSeq);
            globalLog->append("Анализ: Мин = " + QString::number(statistics.min) + ", Макс = " + QString::number(statistics.max) + ", Среднее = " + QString::number(statistics.avg));
        } else if (type == 1 && doubleMainSeq) {
            DataAnalyzer::Stats<double> statistics = DataAnalyzer::GetMinMaxAvg(doubleMainSeq);
            globalLog->append("Анализ: Мин = " + QString::number(statistics.min) + ", Макс = " + QString::number(statistics.max) + ", Среднее = " + QString::number(statistics.avg));
        } else {
            globalLog->append("Анализ недоступен для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onPermutations() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() == 0) {
                globalLog->append("Перестановки: последовательность пуста.");
                return;
            }
            size_t count = DataAnalyzer::GetInversionsCount(intMainSeq);
            globalLog->append("Количество инверсий (int): " + QString::number(count));
        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() == 0) {
                globalLog->append("Перестановки: последовательность пуста.");
                return;
            }
            size_t count = DataAnalyzer::GetInversionsCount(doubleMainSeq);
            globalLog->append("Количество инверсий (double): " + QString::number(count));
        } else {
            globalLog->append("Перестановки: не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSmaller() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() == 0) {
                globalLog->append("Меньших: последовательность пуста.");
                return;
            }

            MutableArraySequence<Sequence<int>*> outContainer;
            MutableArraySequence<int> emptyProto;

            Sequence<Sequence<int>*>* result = DataAnalyzer::GetPrecedingSmaller(intMainSeq, &outContainer, &emptyProto);

            for (size_t i = 0; i < result->GetLength(); i++) {
                QString line = "a[" + QString::number(i) + "]=" + QString::number(intMainSeq->Get(i)) + " -> {";
                Sequence<int>* inner = result->Get(i);
                for (size_t j = 0; j < inner->GetLength(); j++) {
                    line += QString::number(inner->Get(j));
                    if (j + 1 < inner->GetLength()) { line += ", "; }
                }
                line += "}";
                globalLog->append(line);
            }
        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() == 0) {
                globalLog->append("Меньших: последовательность пуста.");
                return;
            }

            MutableArraySequence<Sequence<double>*> outContainer;
            MutableArraySequence<double> emptyProto;

            Sequence<Sequence<double>*>* result = DataAnalyzer::GetPrecedingSmaller(doubleMainSeq, &outContainer, &emptyProto);

            for (size_t i = 0; i < result->GetLength(); i++) {
                QString line = "a[" + QString::number(i) + "]=" + QString::number(doubleMainSeq->Get(i)) + " -> {";
                Sequence<double>* inner = result->Get(i);
                for (size_t j = 0; j < inner->GetLength(); j++) {
                    line += QString::number(inner->Get(j));
                    if (j + 1 < inner->GetLength()) { line += ", "; }
                }
                line += "}";
                globalLog->append(line);
            }
        } else {
            globalLog->append("Меньших: не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onMovingAvg() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() < 3) {
                globalLog->append("Скользящее среднее: нужно минимум 3 элемента.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetMovingAverage(intMainSeq, &outContainer);

            QString line = "Скользящее ср. (окно 3): ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() < 3) {
                globalLog->append("Скользящее среднее: нужно минимум 3 элемента.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetMovingAverage(doubleMainSeq, &outContainer);

            QString line = "Скользящее ср. (окно 3): ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else {
            globalLog->append("Скользящее среднее: не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSigma() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() == 0) {
                globalLog->append("Sigma: последовательность пуста.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetSigmaDifference(intMainSeq, &outContainer);

            QString line = "Sigma-разность: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() == 0) {
                globalLog->append("Sigma: последовательность пуста.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetSigmaDifference(doubleMainSeq, &outContainer);

            QString line = "Sigma-разность: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else {
            globalLog->append("Sigma: не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onReflected() {
    try {
        int type = sequenceTypeSelector->currentIndex();

        if (type == 0) {
            if (!intMainSeq || intMainSeq->GetLength() == 0) {
                globalLog->append("Отражение: последовательность пуста.");
                return;
            }
            MutableArraySequence<int> outContainer;
            Sequence<int>* result = DataAnalyzer::GetReflectedSum(intMainSeq, &outContainer);

            QString line = "Отражённая сумма: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i));
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else if (type == 1) {
            if (!doubleMainSeq || doubleMainSeq->GetLength() == 0) {
                globalLog->append("Отражение: последовательность пуста.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetReflectedSum(doubleMainSeq, &outContainer);

            QString line = "Отражённая сумма: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) { line += ", "; }
            }
            globalLog->append(line);
        } else {
            globalLog->append("Отражение: не поддерживается для символов.");
        }
    } CATCH_ALL_EXCEPTIONS
}