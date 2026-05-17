#include "MainWindow.hpp"

void MainWindow::onPrepend() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();

        UniversalLambda([&](auto& state) {
            if (!state.main || (!isMutable && !state.original)) return;
            typename std::decay_t<decltype(state)>::ValueType parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            auto* result = isMutable ? state.main->Prepend(parsedValue) : state.original->Prepend(parsedValue);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
        });
        
        globalLog->append("Выполнено добавление в начало.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onAppend() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();

        UniversalLambda([&](auto& state) {
            if (!state.main || (!isMutable && !state.original)) return;
            typename std::decay_t<decltype(state)>::ValueType parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            auto* result = isMutable ? state.main->Append(parsedValue) : state.original->Append(parsedValue);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
        });
        
        globalLog->append("Выполнено добавление в конец.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onInsert() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();
        int selectedRow = firstPanelList->currentRow();
        size_t targetIndex = (selectedRow < 0) ? 0 : static_cast<size_t>(selectedRow + 1);

        UniversalLambda([&](auto& state) {
            if (!state.main || (!isMutable && !state.original)) return;
            typename std::decay_t<decltype(state)>::ValueType parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            auto* result = isMutable ? state.main->InsertAt(parsedValue, targetIndex) : state.original->InsertAt(parsedValue, targetIndex);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
        });

        globalLog->append("Выполнена вставка после выбранного.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSubsequence() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QModelIndexList selectedIndexes = firstPanelList->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) { 
            globalLog->append("Ошибка: Выделите элементы в списке 'Основная'."); 
            return; 
        }

        size_t start = static_cast<size_t>(selectedIndexes.first().row());
        size_t end = start;
        for (const QModelIndex& index : selectedIndexes) {
            size_t currentRow = static_cast<size_t>(index.row());
            if (currentRow < start) start = currentRow;
            if (currentRow > end) end = currentRow;
        }

        UniversalLambda([&](auto& state) {
            if (!state.main || (!isMutable && !state.original)) return;
            auto* result = isMutable ? state.main->GetSubsequence(start, end) : state.original->GetSubsequence(start, end);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
        });

        globalLog->append(QString("Выделена подпоследовательность с индекса %1 по %2.").arg(start).arg(end));
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onConcat() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        UniversalLambda([&](auto& state) {
            if (!state.main || !state.additional || (!isMutable && !state.original)) { 
                globalLog->append("Concat: нужны Основная и Дополнительная."); 
                return; 
            }
            auto* result = isMutable ? state.main->Concat(state.additional) : state.original->Concat(state.additional);
            if (state.main != result && state.main != state.original) delete state.main;
            state.main = result;
        });
        globalLog->append("Последовательности объединены.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onFirst() {
    try {
        UniversalLambda([&](auto& state) {
            if (state.main) globalLog->append("Первый элемент: " + ElementToString(state.main->GetFirst()));
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onLast() {
    try {
        UniversalLambda([&](auto& state) {
            if (state.main) globalLog->append("Последний элемент: " + ElementToString(state.main->GetLast()));
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSelected() {
    int selectedRow = firstPanelList->currentRow();
    if (selectedRow < 0) { globalLog->append("Элемент не выбран."); return; }
    try {
        UniversalLambda([&](auto& state) {
            if (state.main) globalLog->append("Выбранный элемент: " + ElementToString(state.main->Get(selectedRow)));
        });
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitNot() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (!seqInt.main || (!isMutable && !seqInt.original)) return;
        
        BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? seqInt.main : seqInt.original);
        if (!baseSeq) { 
            globalLog->append("Ошибка: только для BitSequence."); 
            return; 
        }
        
        BitSequence<int>* result = baseSeq->NOT();
        if (seqInt.main != result && seqInt.main != seqInt.original) {
            delete seqInt.main;
        }
        seqInt.main = result;

        globalLog->append("Выполнена операция НЕ.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitAnd() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (!seqInt.main || !seqInt.additional || (!isMutable && !seqInt.original)) { 
            globalLog->append("Ошибка И: нужны Основная и Доп."); 
            return; 
        }
        
        BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? seqInt.main : seqInt.original);
        BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(seqInt.additional);
        
        if (!baseSeq || !addSeq) { 
            globalLog->append("Ошибка И: обе должны быть BitSequence."); 
            return; 
        }
        
        BitSequence<int>* result = baseSeq->AND(addSeq);
        if (seqInt.main != result && seqInt.main != seqInt.original) {
            delete seqInt.main;
        }
        seqInt.main = result;

        globalLog->append("Выполнена операция И.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitOr() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (!seqInt.main || !seqInt.additional || (!isMutable && !seqInt.original)) { 
            globalLog->append("Ошибка ИЛИ: нужны Основная и Доп."); 
            return; 
        }
        
        BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? seqInt.main : seqInt.original);
        BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(seqInt.additional);
        
        if (!baseSeq || !addSeq) { 
            globalLog->append("Ошибка ИЛИ: обе должны быть BitSequence."); 
            return; 
        }
        
        BitSequence<int>* result = baseSeq->OR(addSeq);
        if (seqInt.main != result && seqInt.main != seqInt.original) {
            delete seqInt.main;
        }
        seqInt.main = result;

        globalLog->append("Выполнена операция ИЛИ.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitXor() {
    try {
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (!seqInt.main || !seqInt.additional || (!isMutable && !seqInt.original)) { 
            globalLog->append("Ошибка XOR: нужны Основная и Доп."); 
            return; 
        }
        
        BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? seqInt.main : seqInt.original);
        BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(seqInt.additional);
        
        if (!baseSeq || !addSeq) { 
            globalLog->append("Ошибка XOR: обе должны быть BitSequence."); 
            return; 
        }
        
        BitSequence<int>* result = baseSeq->XOR(addSeq);
        if (seqInt.main != result && seqInt.main != seqInt.original) delete seqInt.main;
        seqInt.main = result;

        globalLog->append("Выполнена операция XOR.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}