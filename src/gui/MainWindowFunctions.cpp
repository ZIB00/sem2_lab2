#include "MainWindow.hpp"

void MainWindow::onPrepend() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            int parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<int>* result = isMutable ? intMainSeq->Prepend(parsedValue) : intOriginalSeq->Prepend(parsedValue);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) return;
            double parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<double>* result = isMutable ? doubleMainSeq->Prepend(parsedValue) : doubleOriginalSeq->Prepend(parsedValue);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            char16_t parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<char16_t>* result = isMutable ? charMainSeq->Prepend(parsedValue) : charOriginalSeq->Prepend(parsedValue);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }
        
        globalLog->append("Выполнено добавление в начало.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onAppend() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            int parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<int>* result = isMutable ? intMainSeq->Append(parsedValue) : intOriginalSeq->Append(parsedValue);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) return;
            double parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<double>* result = isMutable ? doubleMainSeq->Append(parsedValue) : doubleOriginalSeq->Append(parsedValue);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            char16_t parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<char16_t>* result = isMutable ? charMainSeq->Append(parsedValue) : charOriginalSeq->Append(parsedValue);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }
        
        globalLog->append("Выполнено добавление в конец.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onInsert() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        QString valueText = rightLineEditFunctional->text().trimmed();
        int selectedRow = firstPanelList->currentRow();
        size_t targetIndex = (selectedRow < 0) ? 0 : static_cast<size_t>(selectedRow + 1);

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            int parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<int>* result = isMutable ? intMainSeq->InsertAt(parsedValue, targetIndex) : intOriginalSeq->InsertAt(parsedValue, targetIndex);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) return;
            double parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<double>* result = isMutable ? doubleMainSeq->InsertAt(parsedValue, targetIndex) : doubleOriginalSeq->InsertAt(parsedValue, targetIndex);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            char16_t parsedValue;
            ParseStringToken(valueText, parsedValue);
            
            Sequence<char16_t>* result = isMutable ? charMainSeq->InsertAt(parsedValue, targetIndex) : charOriginalSeq->InsertAt(parsedValue, targetIndex);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена вставка после выбранного.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSubsequence() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);
        
        QModelIndexList selectedIndexes = firstPanelList->selectionModel()->selectedIndexes();
        
        if (selectedIndexes.isEmpty()) { 
            globalLog->append("Ошибка: Выделите элементы в списке 'Основная' для создания подпоследовательности."); 
            return; 
        }

        size_t start = static_cast<size_t>(selectedIndexes.first().row());
        size_t end = start;

        for (const QModelIndex& index : selectedIndexes) {
            size_t currentRow = static_cast<size_t>(index.row());
            if (currentRow < start) start = currentRow;
            if (currentRow > end) end = currentRow;
        }

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            Sequence<int>* result = isMutable ? intMainSeq->GetSubsequence(start, end) : intOriginalSeq->GetSubsequence(start, end);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || (!isMutable && !doubleOriginalSeq)) return;
            Sequence<double>* result = isMutable ? doubleMainSeq->GetSubsequence(start, end) : doubleOriginalSeq->GetSubsequence(start, end);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            Sequence<char16_t>* result = isMutable ? charMainSeq->GetSubsequence(start, end) : charOriginalSeq->GetSubsequence(start, end);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append(QString("Выделена подпоследовательность с индекса %1 по %2.").arg(start).arg(end));
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onConcat() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!intMainSeq || !intAdditionalSeq || (!isMutable && !intOriginalSeq)) { globalLog->append("Concat: нужны Основная и Дополнительная."); return; }
            Sequence<int>* result = isMutable ? intMainSeq->Concat(intAdditionalSeq) : intOriginalSeq->Concat(intAdditionalSeq);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            if (!doubleMainSeq || !doubleAdditionalSeq || (!isMutable && !doubleOriginalSeq)) { globalLog->append("Concat: нужны Основная и Дополнительная."); return; }
            Sequence<double>* result = isMutable ? doubleMainSeq->Concat(doubleAdditionalSeq) : doubleOriginalSeq->Concat(doubleAdditionalSeq);
            if (doubleMainSeq != result && doubleMainSeq != doubleOriginalSeq) delete doubleMainSeq;
            doubleMainSeq = result;
        } else {
            if (!charMainSeq || !charAdditionalSeq || (!isMutable && !charOriginalSeq)) { globalLog->append("Concat: нужны Основная и Дополнительная."); return; }
            Sequence<char16_t>* result = isMutable ? charMainSeq->Concat(charAdditionalSeq) : charOriginalSeq->Concat(charAdditionalSeq);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Последовательности объединены.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onFirst() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        if (type == 0 && intMainSeq) { globalLog->append("Первый элемент: " + QString::number(intMainSeq->GetFirst())); } 
        else if (type == 1 && doubleMainSeq) { globalLog->append("Первый элемент: " + QString::number(doubleMainSeq->GetFirst())); } 
        else if (type == 2 && charMainSeq) { globalLog->append("Первый элемент: " + QString(QChar(static_cast<ushort>(charMainSeq->GetFirst())))); }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onLast() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        if (type == 0 && intMainSeq) { globalLog->append("Последний элемент: " + QString::number(intMainSeq->GetLast())); } 
        else if (type == 1 && doubleMainSeq) { globalLog->append("Последний элемент: " + QString::number(doubleMainSeq->GetLast())); } 
        else if (type == 2 && charMainSeq) { globalLog->append("Последний элемент: " + QString(QChar(static_cast<ushort>(charMainSeq->GetLast())))); }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onSelected() {
    int selectedRow = firstPanelList->currentRow();
    if (selectedRow < 0) { globalLog->append("Элемент не выбран."); return; }

    try {
        int type = sequenceTypeSelector->currentIndex();
        if (type == 0 && intMainSeq) { globalLog->append("Выбранный элемент: " + QString::number(intMainSeq->Get(selectedRow))); } 
        else if (type == 1 && doubleMainSeq) { globalLog->append("Выбранный элемент: " + QString::number(doubleMainSeq->Get(selectedRow))); } 
        else if (type == 2 && charMainSeq) { globalLog->append("Выбранный элемент: " + QString(QChar(static_cast<ushort>(charMainSeq->Get(selectedRow))))); }
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitNot() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!intMainSeq || (!isMutable && !intOriginalSeq)) return;
            BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? intMainSeq : intOriginalSeq);
            if (!baseSeq) { globalLog->append("Ошибка: только для BitSequence."); return; }
            BitSequence<int>* result = baseSeq->NOT();
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            globalLog->append("Ошибка: битовые операции недоступны для вещественных."); return;
        } else {
            if (!charMainSeq || (!isMutable && !charOriginalSeq)) return;
            BitSequence<char16_t>* baseSeq = dynamic_cast<BitSequence<char16_t>*>(isMutable ? charMainSeq : charOriginalSeq);
            if (!baseSeq) { globalLog->append("Ошибка: только для BitSequence."); return; }
            BitSequence<char16_t>* result = baseSeq->NOT();
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена операция НЕ.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitAnd() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!intMainSeq || !intAdditionalSeq || (!isMutable && !intOriginalSeq)) { globalLog->append("Ошибка И: нужны Основная и Доп."); return; }
            BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? intMainSeq : intOriginalSeq);
            BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(intAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка И: обе должны быть BitSequence."); return; }
            BitSequence<int>* result = baseSeq->AND(addSeq);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            globalLog->append("Ошибка И: недоступно для вещественных."); return;
        } else {
            if (!charMainSeq || !charAdditionalSeq || (!isMutable && !charOriginalSeq)) { globalLog->append("Ошибка И: нужны Основная и Доп."); return; }
            BitSequence<char16_t>* baseSeq = dynamic_cast<BitSequence<char16_t>*>(isMutable ? charMainSeq : charOriginalSeq);
            BitSequence<char16_t>* addSeq = dynamic_cast<BitSequence<char16_t>*>(charAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка И: обе должны быть BitSequence."); return; }
            BitSequence<char16_t>* result = baseSeq->AND(addSeq);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена операция И.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitOr() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!intMainSeq || !intAdditionalSeq || (!isMutable && !intOriginalSeq)) { globalLog->append("Ошибка ИЛИ: нужны Основная и Доп."); return; }
            BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? intMainSeq : intOriginalSeq);
            BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(intAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка ИЛИ: обе должны быть BitSequence."); return; }
            BitSequence<int>* result = baseSeq->OR(addSeq);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            globalLog->append("Ошибка ИЛИ: недоступно для вещественных."); return;
        } else {
            if (!charMainSeq || !charAdditionalSeq || (!isMutable && !charOriginalSeq)) { globalLog->append("Ошибка ИЛИ: нужны Основная и Доп."); return; }
            BitSequence<char16_t>* baseSeq = dynamic_cast<BitSequence<char16_t>*>(isMutable ? charMainSeq : charOriginalSeq);
            BitSequence<char16_t>* addSeq = dynamic_cast<BitSequence<char16_t>*>(charAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка ИЛИ: обе должны быть BitSequence."); return; }
            BitSequence<char16_t>* result = baseSeq->OR(addSeq);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена операция ИЛИ.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onBitXor() {
    try {
        int type = sequenceTypeSelector->currentIndex();
        bool isMutable = (mutableOptionSelector->currentIndex() == 0);

        if (type == 0) {
            if (!intMainSeq || !intAdditionalSeq || (!isMutable && !intOriginalSeq)) { globalLog->append("Ошибка XOR: нужны Основная и Доп."); return; }
            BitSequence<int>* baseSeq = dynamic_cast<BitSequence<int>*>(isMutable ? intMainSeq : intOriginalSeq);
            BitSequence<int>* addSeq = dynamic_cast<BitSequence<int>*>(intAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка XOR: обе должны быть BitSequence."); return; }
            BitSequence<int>* result = baseSeq->XOR(addSeq);
            if (intMainSeq != result && intMainSeq != intOriginalSeq) delete intMainSeq;
            intMainSeq = result;
        } else if (type == 1) {
            globalLog->append("Ошибка XOR: недоступно для вещественных."); return;
        } else {
            if (!charMainSeq || !charAdditionalSeq || (!isMutable && !charOriginalSeq)) { globalLog->append("Ошибка XOR: нужны Основная и Доп."); return; }
            BitSequence<char16_t>* baseSeq = dynamic_cast<BitSequence<char16_t>*>(isMutable ? charMainSeq : charOriginalSeq);
            BitSequence<char16_t>* addSeq = dynamic_cast<BitSequence<char16_t>*>(charAdditionalSeq);
            if (!baseSeq || !addSeq) { globalLog->append("Ошибка XOR: обе должны быть BitSequence."); return; }
            BitSequence<char16_t>* result = baseSeq->XOR(addSeq);
            if (charMainSeq != result && charMainSeq != charOriginalSeq) delete charMainSeq;
            charMainSeq = result;
        }

        globalLog->append("Выполнена операция XOR.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}