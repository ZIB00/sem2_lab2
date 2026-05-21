#include "MainWindow.hpp"

void MainWindow::onMinMaxAvg() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() == 0) {
                globalLog->append("Последовательность пуста.");
                return;
            }
            if (state.main) {
                auto statistics = DataAnalyzer::GetMinMaxAvg(state.main);
                globalLog->append("Анализ: Мин = " + QString::number(statistics.min) + 
                                    ", Макс = " + QString::number(statistics.max) + 
                                    ", Среднее = " + QString::number(statistics.avg));
            }
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onPermutations() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() == 0) {
                globalLog->append("Перестановки: последовательность пуста.");
                return;
            }
            size_t count = DataAnalyzer::GetInversionsCount(state.main);
            globalLog->append("Количество инверсий: " + QString::number(count));
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSmaller() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() == 0) {
                globalLog->append("Меньших: последовательность пуста.");
                return;
            }
            MutableArraySequence<Sequence<T>*> outContainer;
            MutableArraySequence<T> emptyProto;
            Sequence<Sequence<T>*>* result = DataAnalyzer::GetPrecedingSmaller(state.main, &outContainer, &emptyProto);
            
            for (size_t i = 0; i < result->GetLength(); i++) {
                QString line = "a[" + QString::number(i) + "]=" + QString::number(state.main->Get(i)) + " -> {";
                Sequence<T>* inner = result->Get(i);
                for (size_t j = 0; j < inner->GetLength(); j++) {
                    line += QString::number(inner->Get(j));
                    if (j + 1 < inner->GetLength()) line += ", ";
                }
                line += "}";
                globalLog->append(line);
            }
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onMovingAvg() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() < 3) {
                globalLog->append("Скользящее среднее: нужно минимум 3 элемента.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetMovingAverage(state.main, &outContainer);

            QString line = "Скользящее среднее: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) line += ", ";
            }
            globalLog->append(line);
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSigma() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() == 0) {
                globalLog->append("Sigma: последовательность пуста.");
                return;
            }
            MutableArraySequence<double> outContainer;
            Sequence<double>* result = DataAnalyzer::GetSigmaDifference(state.main, &outContainer);

            QString line = "Sigma-разность: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                line += QString::number(result->Get(i), 'f', 2);
                if (i + 1 < result->GetLength()) line += ", ";
            }
            globalLog->append(line);
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onReflected() {
    try {
        UniversalLambdaIntDouble([&](auto& state) {
            using T = typename std::decay_t<decltype(state)>::ValueType;
            if (!state.main || state.main->GetLength() == 0) {
                globalLog->append("Отражение: последовательность пуста.");
                return;
            }
            MutableArraySequence<T> outContainer;
            Sequence<T>* result = DataAnalyzer::GetReflectedSum(state.main, &outContainer);

            QString line = "Отражённая сумма: ";
            for (size_t i = 0; i < result->GetLength(); i++) {
                if (std::is_same_v<T, double>) line += QString::number(result->Get(i), 'f', 2);
                else line += QString::number(result->Get(i));
                if (i + 1 < result->GetLength()) line += ", ";
            }
            globalLog->append(line);
        });
    } CATCH_ALL_EXCEPTIONS
}