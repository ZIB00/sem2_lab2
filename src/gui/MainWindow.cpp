#include "MainWindow.hpp"
#include <QApplication>
#include <QScrollArea>
#include <climits>
#include <cmath>
#include <type_traits>

namespace {
    bool intSplitIsZero(int x) { return x == 0; }
    bool dblSplitIsZero(double x) { return x == 0.0; }
    bool charSplitIsSpace(char16_t x){ return x == u' '; }

    char16_t charFromQChar(QChar ch)
    {
        return static_cast<char16_t>(ch.unicode());
    }

    QString charToText(char16_t value)
    {
        return QString(QChar(static_cast<ushort>(value)));
    }

    QString charToDisplay(char16_t value)
    {
        return value == u' ' ? "[пробел]" : charToText(value);
    }

    template<typename T>
    QString valueToString(const T& value)
    {
        return QString::number(value);
    }

    QString valueToString(char16_t value)
    {
        return charToDisplay(value);
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Последовательности");
    setMinimumSize(1200, 650);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->addWidget(createPanel());
    mainSplitter->addWidget(displayPanel());
    mainSplitter->addWidget(operationsPanel());
    mainSplitter->setSizes({220, 450, 330});

    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(140);
    logOutput->setPlaceholderText("Лог операций...");

    mainLayout->addWidget(mainSplitter);
    mainLayout->addWidget(logOutput);
}

MainWindow::~MainWindow()
{
    delete currentSeqInt;
    delete currentSeqDouble;
    delete currentSeqChar;
    delete originalSeqInt;
    delete originalSeqDouble;
    delete originalSeqChar;
}

QGroupBox* MainWindow::createPanel()
{
    QGroupBox* group = new QGroupBox("Создать последовательность");
    QVBoxLayout* layout = new QVBoxLayout(group);

    instructionButton = new QPushButton("? Справка");
    layout->addWidget(instructionButton);
    connect(instructionButton, &QPushButton::clicked, this, &MainWindow::onShowInstructions);

    layout->addWidget(new QLabel("Тип данных:"));
    dataTypeCombo = new QComboBox();
    dataTypeCombo->addItems({"int", "double", "char16_t"});
    layout->addWidget(dataTypeCombo);

    layout->addWidget(new QLabel("Структура:"));
    seqTypeCombo = new QComboBox();
    seqTypeCombo->addItems({"ArraySequence", "ListSequence",
                            "AdaptiveSequence", "SegmentedList", "BitSequence"});
    layout->addWidget(seqTypeCombo);
    connect(seqTypeCombo, &QComboBox::currentTextChanged, this, &MainWindow::onSeqTypeChanged);

    layout->addWidget(new QLabel("Вариант:"));
    mutabilityCombo = new QComboBox();
    mutabilityCombo->addItems({"Изменяемая", "Неизменяемая"});
    layout->addWidget(mutabilityCombo);

    layout->addWidget(new QLabel("Элементы (через пробел):"));
    elementsInput = new QLineEdit();
    elementsInput->setPlaceholderText("1 2 3 / абвг деё жз");
    layout->addWidget(elementsInput);

    createButton = new QPushButton("Создать");
    layout->addWidget(createButton);
    layout->addStretch();

    connect(createButton, &QPushButton::clicked, this, &MainWindow::onCreateSequence);
    return group;
}

QGroupBox* MainWindow::displayPanel()
{
    QGroupBox* group = new QGroupBox("Последовательность");
    QVBoxLayout* layout = new QVBoxLayout(group);

    seqInfoLabel = new QLabel("Тип: —   Длина: 0");
    layout->addWidget(seqInfoLabel);
    seqDisplay = new QListWidget();
    layout->addWidget(seqDisplay);

    origGroup = new QGroupBox("Оригинал (до операции)");
    QVBoxLayout* origLayout = new QVBoxLayout(origGroup);
    origInfoLabel = new QLabel("Длина: 0");
    origDisplay = new QListWidget();
    origLayout->addWidget(origInfoLabel);
    origLayout->addWidget(origDisplay);
    origGroup->setVisible(false);
    layout->addWidget(origGroup);

    return group;
}

// Правая панель (вкладки)  
QTabWidget* MainWindow::operationsPanel()
{
    QTabWidget* tabs = new QTabWidget();

    // Оборачиваем вкладки в QScrollArea чтобы не обрезалось
    auto wrap = [](QWidget* w) {
        QScrollArea* sa = new QScrollArea();
        sa->setWidget(w);
        sa->setWidgetResizable(true);
        return sa;
    };

    tabs->addTab(wrap(buildMethodsTab()), "Методы");
    tabs->addTab(wrap(buildFunctionsTab()), "Функции");
    tabs->addTab(wrap(buildUtilsTab()), "Утилиты");
    tabs->addTab(wrap(buildAnalysisTab()), "Анализ");
    return tabs;
}

//Методы
QWidget* MainWindow::buildMethodsTab()
{
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    auto addGroup = [&](const QString& title, std::function<void(QHBoxLayout*)> fill) {
        QGroupBox* g = new QGroupBox(title);
        QHBoxLayout* h = new QHBoxLayout(g);
        fill(h);
        layout->addWidget(g);
    };

    addGroup("Получить по индексу", [&](QHBoxLayout* h) {
        indexInput = new QLineEdit(); indexInput->setPlaceholderText("индекс");
        QPushButton* btn = new QPushButton("Получить");
        h->addWidget(indexInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onGet);
    });
    addGroup("Добавить в конец", [&](QHBoxLayout* h) {
        appendInput = new QLineEdit(); appendInput->setPlaceholderText("значение");
        QPushButton* btn = new QPushButton("Добавить");
        h->addWidget(appendInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onAppend);
    });
    addGroup("Добавить в начало", [&](QHBoxLayout* h) {
        prependInput = new QLineEdit(); prependInput->setPlaceholderText("значение");
        QPushButton* btn = new QPushButton("Добавить");
        h->addWidget(prependInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onPrepend);
    });
    addGroup("Вставить по индексу", [&](QHBoxLayout* h) {
        insertValueInput = new QLineEdit(); insertValueInput->setPlaceholderText("значение");
        insertIndexInput = new QLineEdit(); insertIndexInput->setPlaceholderText("индекс");
        QPushButton* btn = new QPushButton("Вставить");
        h->addWidget(insertValueInput); h->addWidget(insertIndexInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onInsertAt);
    });
    addGroup("Подпоследовательность [начало, конец]", [&](QHBoxLayout* h) {
        subStartInput = new QLineEdit(); subStartInput->setPlaceholderText("начало");
        subEndInput = new QLineEdit(); subEndInput->setPlaceholderText("конец");
        QPushButton* btn = new QPushButton("Получить");
        h->addWidget(subStartInput); h->addWidget(subEndInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onGetSubsequence);
    });
    addGroup("Первый / Последний", [&](QHBoxLayout* h) {
        QPushButton* bf = new QPushButton("Первый");
        QPushButton* bl = new QPushButton("Последний");
        h->addWidget(bf); h->addWidget(bl);
        connect(bf, &QPushButton::clicked, this, &MainWindow::onGetFirst);
        connect(bl, &QPushButton::clicked, this, &MainWindow::onGetLast);
    });

    layout->addStretch();
    return w;
}

//Функции
QWidget* MainWindow::buildFunctionsTab()
{
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    // Map
    {
        QGroupBox* g = new QGroupBox("Map — применить функцию к каждому элементу");
        QVBoxLayout* v = new QVBoxLayout(g);
        mapFuncCombo = new QComboBox();
        mapFuncCombo->addItems({
            "x * 2          (для символов: следующий символ)",
            "x * x          (для символов: предыдущий символ)",
            "x + 1         (для символов: заглавная/строчная)",
            "|x|             (для символов: строчная)",
            "-x              (для символов: заглавные)"
        });
        QPushButton* btn = new QPushButton("Map");
        v->addWidget(mapFuncCombo);
        v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onMap);
        layout->addWidget(g);
    }
    // Where
    {
        QGroupBox* g = new QGroupBox("Where — оставить элементы по условию");
        QVBoxLayout* v = new QVBoxLayout(g);
        whereFuncCombo = new QComboBox();
        whereFuncCombo->addItems({
            "x > 0             (для символов: только буквы)",
            "x % 2 == 0     (для символов: только цифры)",
            "x < 0             (для символов: только строчные)",
            "x != 0            (для символов: только заглавные)"
        });
        QPushButton* btn = new QPushButton("Where");
        v->addWidget(whereFuncCombo);
        v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onWhere);
        layout->addWidget(g);
    }
    // Reduce
    {
        QGroupBox* g = new QGroupBox("Reduce — свернуть в одно значение");
        QVBoxLayout* v = new QVBoxLayout(g);
        reduceFuncCombo = new QComboBox();
        reduceFuncCombo->addItems({
            "a + b  (сумма)          (для символов: первый элемент)",
            "a * b  (произведение)   (для символов: первый элемент)",
            "макс(a,b)               (для символов: первый элемент)",
            "мин(a,b)                (для символов: первый элемент)"
        });
        QPushButton* btn = new QPushButton("Reduce");
        v->addWidget(reduceFuncCombo);
        v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onReduce);
        layout->addWidget(g);
    }
    // FlatMap
    {
        QGroupBox* g = new QGroupBox("FlatMap — развернуть в новую последовательность");
        QVBoxLayout* v = new QVBoxLayout(g);
        flatMapFuncCombo = new QComboBox();
        flatMapFuncCombo->addItems({
            "x → [x, x²]   (для символов: [x, x])",
            "x → [x, -x]   (для символов: [x, заглавная(x)])",
            "x → [x]       (то же самое)))"
        });
        QPushButton* btn = new QPushButton("FlatMap");
        v->addWidget(flatMapFuncCombo);
        v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onFlatMap);
        layout->addWidget(g);
    }

    {
        QGroupBox* g = new QGroupBox("Skip — пропустить N элементов");
        QHBoxLayout* h = new QHBoxLayout(g);
        skipInput = new QLineEdit(); skipInput->setPlaceholderText("N");
        QPushButton* btn = new QPushButton("Skip");
        h->addWidget(skipInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onSkip);
        layout->addWidget(g);
    }
    {
        QGroupBox* g = new QGroupBox("Splice — удалить N элементов, начиная с позиции i");
        QHBoxLayout* h = new QHBoxLayout(g);
        spliceIndexInput = new QLineEdit(); spliceIndexInput->setPlaceholderText("i");
        spliceCountInput = new QLineEdit(); spliceCountInput->setPlaceholderText("N");
        QPushButton* btn = new QPushButton("Splice");
        h->addWidget(spliceIndexInput); h->addWidget(spliceCountInput); h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onSplice);
        layout->addWidget(g);
    }

    layout->addStretch();
    return w;
}

//Утилиты
QWidget* MainWindow::buildUtilsTab()
{
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    {
        QGroupBox* g = new QGroupBox("Zip — объединить с другой последовательностью");
        QVBoxLayout* v = new QVBoxLayout(g);
        v->addWidget(new QLabel("Вторая последовательность (через пробел):"));
        zipInput = new QLineEdit(); zipInput->setPlaceholderText("4 5 6");
        QPushButton* btn = new QPushButton("Zip");
        v->addWidget(zipInput); v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onZip);
        layout->addWidget(g);
    }
    {
        QGroupBox* g = new QGroupBox("Unzip — разбить последовательность пар на две");
        QVBoxLayout* v = new QVBoxLayout(g);
        v->addWidget(new QLabel("Введите пары через пробел, значения пары через запятую:"));
        v->addWidget(new QLabel("Форматы:  1,2 3,4 5,6   или   (1,2) (3,4)   или   1,2; 3,4"));
        unzipLabel = new QLineEdit(); unzipLabel->setPlaceholderText("1,2 3,4 5,6");
        QPushButton* btn = new QPushButton("Unzip");
        v->addWidget(unzipLabel); v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onUnzip);
        layout->addWidget(g);
    }
    {
        QGroupBox* g = new QGroupBox("Split — разбить по разделителю (0 / пробел)");
        QPushButton* btn = new QPushButton("Split");
        QVBoxLayout* v = new QVBoxLayout(g);
        v->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onSplit);
        layout->addWidget(g);
    }
    {
        QGroupBox* g = new QGroupBox("Range — заполнить диапазоном [начало, конец] и шагом");
        QHBoxLayout* h = new QHBoxLayout(g);
        rangeStartInput = new QLineEdit(); rangeStartInput->setPlaceholderText("начало");
        rangeEndInput = new QLineEdit(); rangeEndInput->setPlaceholderText("конец");
        rangeStepInput  = new QLineEdit(); rangeStepInput->setPlaceholderText("шаг");
        QPushButton* btn = new QPushButton("Range");
        h->addWidget(rangeStartInput); h->addWidget(rangeEndInput);
        h->addWidget(rangeStepInput);  h->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onRange);
        layout->addWidget(g);
    }

    layout->addStretch();
    return w;
}

//Анализ"
QWidget* MainWindow::buildAnalysisTab()
{
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    auto makeBtn = [&](const QString& name, auto slot) {
        QPushButton* btn = new QPushButton(name);
        connect(btn, &QPushButton::clicked, this, slot);
        layout->addWidget(btn);
    };

    makeBtn("Min / Max / Avg", &MainWindow::onBasicStats);
    makeBtn("Медиана", &MainWindow::onMedian);
    makeBtn("Кол-во инверсий", &MainWindow::onInversions);
    makeBtn("Предшествующие меньшие элементы", &MainWindow::onPrecedingSmaller);
    makeBtn("Все префиксы и постфиксы", &MainWindow::onPrefixesPostfixes);
    makeBtn("Скользящее среднее (a[i-1]+a[i]+a[i+1])/3", &MainWindow::onMovingAverage);
    makeBtn("√(σ² − aᵢ²)", &MainWindow::onSigmaDiff);
    makeBtn("aᵢ + a[n-1-i] (отражение)", &MainWindow::onReflectedSum);

    layout->addStretch();
    return w;
}

//Вспомогательные методы

void MainWindow::log(const QString& msg)
{
    logOutput->append(msg);
}

void MainWindow::logError(const QString& msg)
{
    logOutput->append("<span style='color:reduce'>✗ " + msg + "</span>");
}

void MainWindow::saveOriginal()
{
    if (!isImmutable()) return;
    delete originalSeqInt;    
    originalSeqInt = nullptr;
    delete originalSeqDouble; 
    originalSeqDouble = nullptr;
    delete originalSeqChar;   
    originalSeqChar = nullptr;

    if (currentSeqInt && currentSeqInt->GetLength() > 0) {
        originalSeqInt = currentSeqInt->GetSubsequence(0, currentSeqInt->GetLength() - 1);
    }

    if (currentSeqDouble && currentSeqDouble->GetLength() > 0) {
        originalSeqDouble = currentSeqDouble->GetSubsequence(0, currentSeqDouble->GetLength() - 1);
    }

    if (currentSeqChar && currentSeqChar->GetLength() > 0) {
        originalSeqChar = currentSeqChar->GetSubsequence(0, currentSeqChar->GetLength() - 1);
    }
}

void MainWindow::clearOriginal()
{
    delete originalSeqInt;
    originalSeqInt = nullptr;
    delete originalSeqDouble;
    originalSeqDouble = nullptr;
    delete originalSeqChar;
    originalSeqChar = nullptr;
    origGroup->setVisible(false);
}

void MainWindow::refreshDisplay()
{
    seqDisplay->clear();
    origDisplay->clear();

    bool isImmut = isImmutable();
    origGroup->setVisible(isImmut);

    auto fillList = [](QListWidget* list, QLabel* lbl, auto* seq, const QString& typeName) {
        if (!seq) { 
            lbl->setText("Тип: —   Длина: 0"); 
            return; 
        }

        size_t len = seq->GetLength();

        lbl->setText(QString("Тип: %1   Длина: %2").arg(typeName).arg(len));
        
        for (size_t i = 0; i < len; ++i) {
            list->addItem(QString("[%1] = %2").arg(i).arg(valueToString(seq->Get(i))));
        }
    };

    QString typeName = seqTypeCombo->currentText();
    switch (currentType) {
        case DataType::Int:
            fillList(seqDisplay, seqInfoLabel, currentSeqInt, typeName);
            if (isImmut) fillList(origDisplay, origInfoLabel, originalSeqInt, typeName + " (оригинал)");
            break;
        case DataType::Double:
            fillList(seqDisplay, seqInfoLabel, currentSeqDouble, typeName);
            if (isImmut) fillList(origDisplay, origInfoLabel, originalSeqDouble, typeName + " (оригинал)");
            break;
        case DataType::Char:
            fillList(seqDisplay, seqInfoLabel, currentSeqChar, typeName);
            if (isImmut) fillList(origDisplay, origInfoLabel, originalSeqChar, typeName + " (оригинал)");
            break;
    }
}

bool MainWindow::parseIntVal(const QString& text, int& out)
{
    bool ok;
    QString t = text.trimmed();
    out = t.toInt(&ok);
    if (!ok) {
        logError("Некорректный ввод");
        return false;
    }
    return true;
}

bool MainWindow::parseDoubleVal(const QString& text, double& out)
{
    bool ok;
    QString t = text.trimmed();
    out = t.toDouble(&ok);
    if (!ok) {
        logError("Ожидается вещественное число, получено: \"" + t + "\"");
        return false;
    }
    if (std::isinf(out)) {
        logError(QString("Число \"%1\" слишком велико для double").arg(t));
        return false;
    }
    return true;
}

bool MainWindow::parseIndex(const QString& text, size_t& out)
{
    bool ok;
    int v = text.trimmed().toInt(&ok);
    if (!ok || v < 0) {
        logError("Некорректный ввод");
        return false;
    }
    out = static_cast<size_t>(v);
    return true;
}

bool MainWindow::parseIntList(const QString& text, QList<int>& out)
{
    out.clear();
    QString t = text.trimmed();
    if (t.isEmpty()) { 
        logError("Поле ввода пустое"); 
        return false; 
    }

    for (const QString& s : t.split(' ', Qt::SkipEmptyParts)) {
        bool ok;
        int v = s.toInt(&ok);
        if (!ok) { logError("Некорректный ввод"); return false; }
        out.append(v);
    }
    return true;
}

bool MainWindow::parseDoubleList(const QString& text, QList<double>& out)
{
    out.clear();
    QString t = text.trimmed();
    if (t.isEmpty()) { logError("Поле ввода пустое"); return false; }
    for (const QString& s : t.split(' ', Qt::SkipEmptyParts)) {
        bool ok;
        double v = s.toDouble(&ok);
        if (!ok) { logError("\"" + s + "\" — не вещественное число"); return false; }
        out.append(v);
    }
    return true;
}

bool MainWindow::parseCharList(const QString& text, QList<char16_t>& out)
{
    out.clear();
    if (text.isEmpty()) { logError("Поле ввода пустое"); return false; }
    for (QChar ch : text)
        out.append(charFromQChar(ch));
    return true;
}

//Слоты

void MainWindow::onShowInstructions()
{
    InstructionDialog dlg(this);
    dlg.exec();
}

void MainWindow::onSeqTypeChanged()
{
    if (hasNoMutability()) {
        mutabilityCombo->setCurrentIndex(0);
        mutabilityCombo->setEnabled(false);
    } else {
        mutabilityCombo->setEnabled(true);
    }
}

void MainWindow::onCreateSequence()
{
    int dtIdx = dataTypeCombo->currentIndex();
    currentType = (dtIdx == 0) ? DataType::Int : (dtIdx == 1) ? DataType::Double : DataType::Char;

    QString seqType = seqTypeCombo->currentText();
    bool mutable_ = (mutabilityCombo->currentIndex() == 0);

    if (!mutable_ && hasNoMutability()) {
        logError(seqType + " поддерживает только Mutable");
        return;
    }

    clearOriginal();

    delete currentSeqInt;
    currentSeqInt = nullptr;
    delete currentSeqDouble;
    currentSeqDouble = nullptr;
    delete currentSeqChar;
    currentSeqChar = nullptr;

    try {
        if (currentType == DataType::Int) {
            QList<int> elems;
            if (!parseIntList(elementsInput->text(), elems)) return;
            int* arr = new int[elems.size()];
            for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];
            size_t cnt = static_cast<size_t>(elems.size());

            if (seqType == "ArraySequence") {
                if (mutable_) {
                    currentSeqInt = (Sequence<int>*)new MutableArraySequence<int>(arr, cnt);
                } else {
                    currentSeqInt = (Sequence<int>*)new ImmutableArraySequence<int>(arr, cnt);
                }
            } else if (seqType == "ListSequence") {
                if (mutable_) {
                    currentSeqInt = (Sequence<int>*)new MutableListSequence<int>(arr, cnt);
                } else {
                    currentSeqInt = (Sequence<int>*)new ImmutableListSequence<int>(arr, cnt);
                }
            } else if (seqType == "AdaptiveSequence") {
                currentSeqInt = new AdaptiveSequence<int>(arr, cnt);
            } else if (seqType == "SegmentedList") {
                currentSeqInt = new SegmentedList<int>(arr, cnt);
            } else if (seqType == "BitSequence") {
                currentSeqInt = new BitSequence<int>(arr, cnt);
            }
            delete[] arr;

        } else if (currentType == DataType::Double) {
            QList<double> elems;
            if (!parseDoubleList(elementsInput->text(), elems)) return;
            double* arr = new double[elems.size()];
            for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];
            size_t cnt = static_cast<size_t>(elems.size());

            if (seqType == "ArraySequence") {
                if (mutable_) {
                    currentSeqDouble = (Sequence<double>*)new MutableArraySequence<double>(arr, cnt);
                } else {
                    currentSeqDouble = (Sequence<double>*)new ImmutableArraySequence<double>(arr, cnt);
                }
            } else if (seqType == "ListSequence") {
                if (mutable_) {
                    currentSeqDouble = (Sequence<double>*)new MutableListSequence<double>(arr, cnt);
                } else {
                    currentSeqDouble = (Sequence<double>*)new ImmutableListSequence<double>(arr, cnt);
                }
            } else if (seqType == "AdaptiveSequence") {
                currentSeqDouble = new AdaptiveSequence<double>(arr, cnt);
            } else if (seqType == "SegmentedList") {
                currentSeqDouble = new SegmentedList<double>(arr, cnt);
            } else if (seqType == "BitSequence") {
                logError("BitSequence поддерживает только int");
                delete[] arr; return;
            }
            delete[] arr;

        } else {
            QList<char16_t> elems;
            if (!parseCharList(elementsInput->text(), elems)) return;
            char16_t* arr = new char16_t[elems.size()];
            for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];
            size_t cnt = static_cast<size_t>(elems.size());

            if (seqType == "ArraySequence") {
                if (mutable_) {
                    currentSeqChar = (Sequence<char16_t>*)new MutableArraySequence<char16_t>(arr, cnt);
                } else {
                    currentSeqChar = (Sequence<char16_t>*)new ImmutableArraySequence<char16_t>(arr, cnt);
                }
            } else if (seqType == "ListSequence") {
                if (mutable_) {
                    currentSeqChar = (Sequence<char16_t>*)new MutableListSequence<char16_t>(arr, cnt);
                } else {
                    currentSeqChar = (Sequence<char16_t>*)new ImmutableListSequence<char16_t>(arr, cnt);
                }
            } else if (seqType == "AdaptiveSequence") {
                currentSeqChar = new AdaptiveSequence<char16_t>(arr, cnt);
            } else if (seqType == "SegmentedList") {
                currentSeqChar = new SegmentedList<char16_t>(arr, cnt);
            } else if (seqType == "BitSequence") {
                logError("BitSequence поддерживает только int");
                delete[] arr; return;
            }
            delete[] arr;
        }

        log("✓ Создана: " + seqType + " <" + dataTypeCombo->currentText() + "> "
            + (mutable_ ? "Mutable" : "Immutable"));
        refreshDisplay();
    } catch (const std::exception& e) {
        logError(QString(e.what()));
    }
}
 
#define DISPATCH_READ(intBlock, dblBlock, charBlock) \
    switch (currentType) { \
        case DataType::Int: \
            if (!currentSeqInt) { \
                logError("Нет последовательности"); \
                return; \
            } \
            intBlock; \
            break; \
        case DataType::Double:\
            if (!currentSeqDouble) { \
                logError("Нет последовательности"); \
                return; \
            } dblBlock; \
            break; \
        case DataType::Char: \
            if (!currentSeqChar) { \
                logError("Нет последовательности"); \
                return; \
            } \
            charBlock;  \
            break; \
    }

#define DISPATCH_WRITE(intBlock, dblBlock, charBlock) \
    saveOriginal(); \
    DISPATCH_READ(intBlock, dblBlock, charBlock) \
    refreshDisplay();


void MainWindow::onGet()
{
    size_t index;
    if (!parseIndex(indexInput->text(), index)) return;
    try {
        DISPATCH_READ( \
            log(QString("Get(%1) = %2").arg(index).arg(currentSeqInt->Get(index))), \
            log(QString("Get(%1) = %2").arg(index).arg(currentSeqDouble->Get(index))), \
            log(QString("Get(%1) = %2").arg(index).arg(valueToString(currentSeqChar->Get(index)))) \
        );
    } catch (const std::exception& e) { 
        logError(e.what()); 
    }
}

void MainWindow::onAppend()
{
    try {
        switch (currentType) {
            case DataType::Int: {
                if (!currentSeqInt) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                int v; if (!parseIntVal(appendInput->text(), v)) return;

                saveOriginal(); 
                currentSeqInt = currentSeqInt->Append(v);

                log("Append(" + QString::number(v) + ")"); 
                break;
            }
            case DataType::Double: {
                if (!currentSeqDouble) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                double v; 
                if (!parseDoubleVal(appendInput->text(), v)) 
                return;

                saveOriginal(); 
                currentSeqDouble = currentSeqDouble->Append(v);

                log("Append(" + QString::number(v) + ")"); 
                break;
            }
            case DataType::Char: {
                if (!currentSeqChar) { 
                    logError("Нет последовательности"); 
                    return; 
                }
                QList<char16_t> elems; 
                if (!parseCharList(appendInput->text(), elems)) return;

                saveOriginal();

                for (char16_t c : elems) currentSeqChar = currentSeqChar->Append(c);

                log(QString("Append(%1 символ)").arg(elems.size())); 
                break;
            }
        }
        refreshDisplay();
    } catch (const std::exception& e) { 
        logError(e.what()); 
    }
}

void MainWindow::onPrepend()
{
    try {
        switch (currentType) {
            case DataType::Int: {
                if (!currentSeqInt) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                int v; 
                if (!parseIntVal(prependInput->text(), v)) return;

                saveOriginal(); 
                currentSeqInt = currentSeqInt->Prepend(v);

                log("Prepend(" + QString::number(v) + ")"); break;
            }
            case DataType::Double: {
                if (!currentSeqDouble) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                double v; 
                if (!parseDoubleVal(prependInput->text(), v)) return;

                saveOriginal(); 
                currentSeqDouble = currentSeqDouble->Prepend(v);

                log("Prepend(" + QString::number(v) + ")"); break;
            }
            case DataType::Char: {
                if (!currentSeqChar) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                QList<char16_t> elems; 
                if (!parseCharList(prependInput->text(), elems)) return;

                saveOriginal();
                for (int i = elems.size() - 1; i >= 0; --i) {
                    currentSeqChar = currentSeqChar->Prepend(elems[i]);
                }
                log(QString("Prepend(%1 char16_t)").arg(elems.size())); 
                break;
            }
        }
        refreshDisplay();
    } catch (const std::exception& e) { 
        logError(e.what()); 
    }
}

void MainWindow::onInsertAt()
{
    size_t index; if (!parseIndex(insertIndexInput->text(), index)) return;
    try {
        switch (currentType) {
            case DataType::Int: {
                if (!currentSeqInt) { 
                    logError("Нет последовательности"); 
                    return; 
                }
                int v; 
                if (!parseIntVal(insertValueInput->text(), v)) return;

                saveOriginal(); 
                currentSeqInt = currentSeqInt->InsertAt(v, index);

                log(QString("Вставка (%1, номер=%2)").arg(v).arg(index+1)); 
                break;
            }
            case DataType::Double: {
                if (!currentSeqDouble) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                double v; 
                if (!parseDoubleVal(insertValueInput->text(), v)) return;

                saveOriginal(); 
                currentSeqDouble = currentSeqDouble->InsertAt(v, index);

                log(QString("Вставка (%1, номер=%2)").arg(v).arg(index+1)); 
                break;
            }
            case DataType::Char: {
                if (!currentSeqChar) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                QList<char16_t> elems; 
                if (!parseCharList(insertValueInput->text(), elems)) return;

                saveOriginal();
                for (int i = 0; i < elems.size(); ++i) {
                    currentSeqChar = currentSeqChar->InsertAt(elems[i], index + i);
                }

                log(QString("Вставка(%1 символ, номер=%2)").arg(elems.size()).arg(index+1)); 
                break;
            }
        }
        refreshDisplay();
    } catch (const std::exception& e) { 
        logError(e.what()); 
    }
}

void MainWindow::onGetSubsequence()
{
    size_t s, e;
    if (!parseIndex(subStartInput->text(), s)) return;
    if (!parseIndex(subEndInput->text(), e)) return;

    try {
        auto printSeq = [&](auto* seq) {
            if (!seq) { logError("Нет последовательности"); return; }
            auto* sub = seq->GetSubsequence(s, e);
            QString r = QString("Подпоследовательность(%1,%2) = [").arg(s).arg(e);
            for (size_t i = 0; i < sub->GetLength(); ++i) {
                if (i > 0) r += ", ";
                r += valueToString(sub->Get(i));
            }
            r += "]"; log(r); delete sub;
        };
        switch (currentType) {
            case DataType::Int: printSeq(currentSeqInt); break;
            case DataType::Double: printSeq(currentSeqDouble); break;
            case DataType::Char: printSeq(currentSeqChar); break;
        }
    } catch (const std::exception& ex) { 
        logError(ex.what()); 
    }
}

void MainWindow::onGetFirst()
{
    try {
        DISPATCH_READ(
            log("Первый элемент = " + QString::number(currentSeqInt->GetFirst())),
            log("Первый элемент = " + QString::number(currentSeqDouble->GetFirst())),
            log(QString("Первый элемент = '%1'").arg(valueToString(currentSeqChar->GetFirst())))
        );
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onGetLast()
{
    try {
        DISPATCH_READ(
            log("Последний элемент = " + QString::number(currentSeqInt->GetLast())),
            log("Последний элемент = " + QString::number(currentSeqDouble->GetLast())),
            log(QString("Последний элемент = '%1'").arg(valueToString(currentSeqChar->GetLast())))
        );
    } catch (const std::exception& e) { logError(e.what()); }
}

namespace {
    int g_mapChoice = 0, g_whereChoice = 0, g_reduceChoice = 0, g_flatMapChoice = 0;

    int mapInt (int x) { 
        switch(g_mapChoice) {
            case 1:
                return x*x; 
            case 2:
                return x+1; 
            case 3:
                return x < 0 ? -x : x; 
            case 4:
                return -x; 
            default:
                return x*2;
            } 
    }
    double mapDouble (double x) { 
        switch(g_mapChoice) {
            case 1:
                return x * x; 
            case 2:
                return x+1; 
            case 3:
                return x < 0 ? -x : x; 
            case 4:
                return -x; 
            default:
                return x * 2.0;
            } 
        }
    char16_t mapChar (char16_t x) { 
        QChar q(static_cast<ushort>(x)); 
        switch(g_mapChoice){
            case 1:
                return static_cast<char16_t>(x-1); 
            case 2:
                return (q.isLower() || q.isUpper()) ? static_cast<char16_t>(q.isLower() ? q.toUpper().unicode() : q.toLower().unicode()) : x; 
            case 3:
                return static_cast<char16_t>(q.toLower().unicode()); 
            case 4:
                return static_cast<char16_t>(q.toUpper().unicode()); 
            default:
                return static_cast<char16_t>(x+1);
            } 
        }

    bool whereInt (int x) { 
        switch(g_whereChoice) {
            case 1:
                return x % 2 == 0; 
            case 2:
                return x < 0; 
            case 3:
                return x != 0; 
            default:
                return x > 0;
            } 
        }
    bool whereDouble (double x) { 
        switch(g_whereChoice) {
            case 1:
                return fmod(x,2)==0; 
            case 2:
                return x<0; 
            case 3:
                return x != 0; 
            default:
                return x > 0;
            } 
        }
    bool whereChar(char16_t x) { 
        QChar q(static_cast<ushort>(x)); 
        switch(g_whereChoice) {
            case 1:
                return q.isLetter(); 
            case 2:
                return q.isDigit(); 
            case 3:
                return q.isLower(); 
            default:
                return q.isUpper();
            } 
        }

    int reduceInt(int a, int b) { 
        switch (g_reduceChoice) {
            case 1:  
                return a * b;  
            case 2:  
                return (a > b) ? a : b; 
            case 3:  
                return (a < b) ? a : b; 
            default: 
                return a + b; 
        } 
    }

    double reduceDouble(double a, double b) { 
        switch (g_reduceChoice) {
            case 1:  
                return a * b;
            case 2:  
                return (a > b) ? a : b;
            case 3:  
                return (a < b) ? a : b;
            default: 
                return a + b;
        } 
    }

    char16_t reduceChar(char16_t a, char16_t /* b */) 
    { 
        return a; 
    }

    Sequence<int>* fmInt(int x) 
    {
        switch (g_flatMapChoice) {
            case 1: {
                int a[2] = {x, -x};
                return new MutableArraySequence<int>(a, 2);
            }
            case 2: {
                int a[1] = {x};
                return new MutableArraySequence<int>(a, 1);
            }
            default: {
                int a[2] = {x, x * x};
                return new MutableArraySequence<int>(a, 2);
            }
        }
    }

    Sequence<double>* fmDouble(double x) 
    {
        switch (g_flatMapChoice) {
            case 1: {
                double a[2] = {x, -x};
                return new MutableArraySequence<double>(a, 2);
            }
            case 2: {
                double a[1] = {x};
                return new MutableArraySequence<double>(a, 1);
            }
            default: {
                double a[2] = {x, x * x};
                return new MutableArraySequence<double>(a, 2);
            }
        }
    }

    Sequence<char16_t>* fmChar(char16_t x) 
    {
        QChar q(static_cast<ushort>(x));
        
        switch (g_flatMapChoice) {
            case 1: {
                char16_t a[2] = {x, static_cast<char16_t>(q.toUpper().unicode())};
                return new MutableArraySequence<char16_t>(a, 2);
            }
            case 2: {
                char16_t a[1] = {x};
                return new MutableArraySequence<char16_t>(a, 1);
            }
            default: {
                char16_t a[2] = {x, x};
                return new MutableArraySequence<char16_t>(a, 2);
            }
        }
    }
}

void MainWindow::onMap()
{
    g_mapChoice = mapFuncCombo->currentIndex();
    try {
        DISPATCH_WRITE(
            currentSeqInt = currentSeqInt->Map(mapInt),
            currentSeqDouble = currentSeqDouble->Map(mapDouble),
            currentSeqChar = currentSeqChar->Map(mapChar)
        );
        log("Map: " + mapFuncCombo->currentText().split("(")[0].trimmed());
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onWhere()
{
    g_whereChoice = whereFuncCombo->currentIndex();
    try {
        DISPATCH_WRITE(
            currentSeqInt = currentSeqInt->Where(whereInt),
            currentSeqDouble = currentSeqDouble->Where(whereDouble),
            currentSeqChar = currentSeqChar->Where(whereChar)
        );
        log("Where: " + whereFuncCombo->currentText().split("(")[0].trimmed());
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onReduce()
{
    g_reduceChoice = reduceFuncCombo->currentIndex();
    try {
        DISPATCH_READ(
            log("Reduce = " + QString::number(currentSeqInt->Reduce(reduceInt))),
            log("Reduce = " + QString::number(currentSeqDouble->Reduce(reduceDouble))),
            log(QString("Reduce = '%1'").arg(valueToString(currentSeqChar->Reduce(reduceChar))))
        );
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onFlatMap()
{
    g_flatMapChoice = flatMapFuncCombo->currentIndex();
    try {
        DISPATCH_WRITE(
            currentSeqInt  = currentSeqInt->FlatMap(fmInt),
            currentSeqDouble = currentSeqDouble->FlatMap(fmDouble),
            currentSeqChar = currentSeqChar->FlatMap(fmChar)
        );
        log("FlatMap: " + flatMapFuncCombo->currentText().split("(")[0].trimmed());
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onSkip()
{
    size_t n; if (!parseIndex(skipInput->text(), n)) return;
    try {
        DISPATCH_WRITE(
            currentSeqInt = currentSeqInt->Skip(n),
            currentSeqDouble = currentSeqDouble->Skip(n),
            currentSeqChar = currentSeqChar->Skip(n)
        );
        log("Skip(" + QString::number(n) + ")");
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onSplice()
{
    size_t index, cnt;
    if (!parseIndex(spliceIndexInput->text(), index)) return;
    if (!parseIndex(spliceCountInput->text(), cnt)) return;
    try {
        DISPATCH_WRITE(
            currentSeqInt = currentSeqInt->Splice(index, cnt),
            currentSeqDouble = currentSeqDouble->Splice(index, cnt),
            currentSeqChar = currentSeqChar->Splice(index, cnt)
        );
        log(QString("Splice(%1, %2)").arg(index).arg(cnt));
    } catch (const std::exception& e) { logError(e.what()); }
}

//Утилиты─────────

void MainWindow::onZip()
{
    try {
        switch (currentType) {
            case DataType::Int: {
                if (!currentSeqInt) {
                    logError("Нет последовательности"); 
                    return; 
                }

                QList<int> elems; 
                if (!parseIntList(zipInput->text(), elems)) return;

                int* arr = new int[elems.size()];

                for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];

                MutableArraySequence<int> other(arr, elems.size()); 
                delete[] arr;

                auto* it = SequenceUtils::Zip(currentSeqInt, &other);
                QString r = "Zip = ["; bool first = true;

                while (it->MoveNext()) {
                    if (!first) r += ", ";
                    auto p = it->GetCurrent();
                    r += QString("(%1,%2)").arg(p.first).arg(p.second);
                    first = false;
                }
                r += "]"; delete it; log(r); 
                break;
            }
            case DataType::Double: {
                if (!currentSeqDouble) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                QList<double> elems; 
                if (!parseDoubleList(zipInput->text(), elems)) 
                return;

                double* arr = new double[elems.size()];
                for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];

                MutableArraySequence<double> other(arr, elems.size()); 
                delete[] arr;

                auto* it = SequenceUtils::Zip(currentSeqDouble, &other);
                QString r = "Zip = ["; bool first = true;

                while (it->MoveNext()) {
                    if (!first) r += ", ";
                    auto p = it->GetCurrent();
                    r += QString("(%1,%2)").arg(p.first).arg(p.second);
                    first = false;
                }
                r += "]"; delete it; log(r); 
                break;
            }
            case DataType::Char: {
                if (!currentSeqChar) { 
                    logError("Нет последовательности"); 
                    return; 
                }
                QList<char16_t> elems; 
                if (!parseCharList(zipInput->text(), elems)) 
                return;

                char16_t* arr = new char16_t[elems.size()];

                for (int i = 0; i < elems.size(); ++i) arr[i] = elems[i];

                MutableArraySequence<char16_t> other(arr, elems.size()); 
                delete[] arr;

                auto* it = SequenceUtils::Zip(currentSeqChar, &other);
                QString r = "Zip = ["; bool first = true;

                while (it->MoveNext()) {
                    if (!first) r += ", ";
                    auto p = it->GetCurrent();
                    r += QString("('%1','%2')").arg(valueToString(p.first)).arg(valueToString(p.second));
                    first = false;
                }
                r += "]"; delete it; log(r);
                break;
            }
        }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onUnzip()
{
    QString text = unzipLabel->text().trimmed();
    if (text.isEmpty()) { logError("Поле ввода пустое"); return; }

    text.remove('(').remove(')').replace(';', ' ');
    
    QStringList tokens = text.split(' ', Qt::SkipEmptyParts);

    try {
        switch (currentType) {
            case DataType::Int: {
                MutableArraySequence<Pair<int,int>> pairs;
                for (const QString& token : tokens) {
                    int comma = token.indexOf(',');
                    if (comma < 0) { 
                        logError("Ожидается формат «a,b», получено: \"" + token + "\""); 
                        return; 
                    }
                    int a, b;

                    if (!parseIntVal(token.left(comma),  a)) return;
                    if (!parseIntVal(token.mid(comma+1), b)) return;

                    pairs.Append(Pair<int,int>(a, b));
                }
                MutableArraySequence<int> s1, s2;
                auto res = SequenceUtils::Unzip<int>(&pairs, &s1, &s2);

                QString r1 = "A = [", r2 = "B = [";
                for (size_t i = 0; i < res.first->GetLength();  ++i) { 
                    if (i) r1 += ", "; 
                    r1 += QString::number(res.first->Get(i));  
                }
                for (size_t i = 0; i < res.second->GetLength(); ++i) { 
                    if (i) r2+=", "; 
                    r2+=QString::number(res.second->Get(i)); 
                }
                log("Unzip:  " + r1 + "]   " + r2 + "]"); 
                break;
            }
            case DataType::Double: {
                MutableArraySequence<Pair<double,double>> pairs;
                for (const QString& token : tokens) {
                    int comma = token.indexOf(',');
                    if (comma < 0) { 
                        logError("Ожидается формат «a,b», получено: \"" + token + "\"");
                        return; 
                    }
                    double a, b;
                    if (!parseDoubleVal(token.left(comma),  a)) return;
                    if (!parseDoubleVal(token.mid(comma+1), b)) return;

                    pairs.Append(Pair<double,double>(a, b));
                }
                MutableArraySequence<double> s1, s2;
                auto res = SequenceUtils::Unzip<double>(&pairs, &s1, &s2);
                QString r1 = "A = [", r2 = "B = [";
                for (size_t i = 0; i < res.first->GetLength();  ++i) { 
                    if (i) r1+=", "; 
                    r1 += QString::number(res.first->Get(i));  
                }

                for (size_t i = 0; i < res.second->GetLength(); ++i) { 
                    if (i) r2+=", "; r2+=QString::number(res.second->Get(i)); 
                }

                log("Unzip:  " + r1 + "]   " + r2 + "]"); break;
            }
            case DataType::Char: {
                MutableArraySequence<Pair<char16_t,char16_t>> pairs;
                for (const QString& token : tokens) {
                    int comma = token.indexOf(',');
                    if (comma < 0) { 
                        logError("Ожидается формат «a,b», получено: \"" + token + "\""); 
                        return; 
                    }

                    QString ls = token.left(comma).trimmed(), rs = token.mid(comma+1).trimmed();
                    if (ls.length()!=1||rs.length()!=1) { 
                        logError("Символ должен быть одним знаком: \"" + token + "\""); 
                        return; 
                    }
                    pairs.Append(Pair<char16_t,char16_t>(charFromQChar(ls[0]), charFromQChar(rs[0])));
                }
                MutableArraySequence<char16_t> s1, s2;
                auto res = SequenceUtils::Unzip<char16_t>(&pairs, &s1, &s2);
                QString r1 = "A = [", r2 = "B = [";

                for (size_t i = 0; i < res.first->GetLength();  ++i) { 
                    if (i) r1+=", "; r1 += QString("'%1'").arg(valueToString(res.first->Get(i)));  
                }
                for (size_t i = 0; i < res.second->GetLength(); ++i) { 
                    if (i) r2+=", "; r2 += QString("'%1'").arg(valueToString(res.second->Get(i))); 
                }
                log("Unzip:  " + r1 + "]   " + r2 + "]"); 
                break;
            }
        }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onSplit()
{
    try {
        switch (currentType) {
            case DataType::Int: {
                if (!currentSeqInt) { logError("Нет последовательности"); return; }
                MutableArraySequence<int> proto;
                auto* it = SequenceUtils::Split(currentSeqInt, intSplitIsZero, &proto);
                int index = 0;
                while (it->MoveNext()) {
                    auto* part = it->GetCurrent();
                    QString s = QString("Часть %1: [").arg(index++);
                    for (size_t i = 0; i < part->GetLength(); ++i) {
                        if (i > 0) s += ", ";
                        s += QString::number(part->Get(i));
                    }
                    log(s + "]"); delete part;
                }
                delete it; break;
            }
            case DataType::Double: {
                if (!currentSeqDouble) { 
                    logError("Нет последовательности"); 
                    return; 
                }

                MutableArraySequence<double> proto;
                auto* it = SequenceUtils::Split(currentSeqDouble, dblSplitIsZero, &proto);

                int index = 0;
                while (it->MoveNext()) {
                    auto* part = it->GetCurrent();
                    QString s = QString("Часть %1: [").arg(index++);

                    for (size_t i = 0; i < part->GetLength(); ++i) {
                        if (i > 0) s += ", ";
                        s += QString::number(part->Get(i));
                    }

                    log(s + "]"); 
                    delete part;
                }
                delete it; break;
            }
            case DataType::Char: {
                if (!currentSeqChar) { 
                    logError("Нет последовательности"); 
                    return; 
                }
                MutableArraySequence<char16_t> proto;
                auto* it = SequenceUtils::Split(currentSeqChar, charSplitIsSpace, &proto);
                int index = 0;
                while (it->MoveNext()) {
                    auto* part = it->GetCurrent();
                    QString s = QString("Часть %1: [").arg(index++);
                    for (size_t i = 0; i < part->GetLength(); ++i) {
                        if (i > 0) s += ", ";
                        s += QString("'%1'").arg(valueToString(part->Get(i)));
                    }
                    log(s + "]"); 
                    delete part;
                }
                delete it; 
                break;
            }
        }
    } catch (const std::exception& e) { 
        logError(e.what()); 
    }
}

void MainWindow::onRange()
{
    if (currentType == DataType::Char) { 
        logError("Range не поддерживается для символов"); 
        return; 
    }
    try {
        if (currentType == DataType::Int) {
            int s, e, step;
            if (!parseIntVal(rangeStartInput->text(), s)) return;
            if (!parseIntVal(rangeEndInput->text(), e)) return;
            if (!parseIntVal(rangeStepInput->text(), step)) return;
            delete currentSeqInt;
            currentSeqInt = SequenceUtils::Range<int>(s, e, step, new MutableArraySequence<int>());
            log(QString("Range(%1, %2, %3)").arg(s).arg(e).arg(step));
        } else {
            double s, e, step;
            if (!parseDoubleVal(rangeStartInput->text(), s)) return;
            if (!parseDoubleVal(rangeEndInput->text(), e)) return;
            if (!parseDoubleVal(rangeStepInput->text(), step)) return;
            delete currentSeqDouble;
            currentSeqDouble = SequenceUtils::Range<double>(s, e, step, new MutableArraySequence<double>());
            log(QString("Range(%1, %2, %3)").arg(s).arg(e).arg(step));
        }
        refreshDisplay();
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onBasicStats()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            IntRing ring;
            auto st = DataAnalyzer<int>::GetBasicStats(currentSeqInt, ring);
            log(QString("Min=%1  Max=%2  Avg=%3").arg(st.min).arg(st.max).arg(st.avg));
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            DoubleRing ring;
            auto st = DataAnalyzer<double>::GetBasicStats(currentSeqDouble, ring);
            log(QString("Min=%1  Max=%2  Avg=%3").arg(st.min).arg(st.max).arg(st.avg));
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onMedian()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            log("Медиана = " + QString::number(DataAnalyzer<int>::GetMedian(currentSeqInt)));
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            log("Медиана = " + QString::number(DataAnalyzer<double>::GetMedian(currentSeqDouble)));
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onInversions()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            log("Инверсий = " + QString::number(DataAnalyzer<int>::GetInversionsCount(currentSeqInt)));
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            log("Инверсий = " + QString::number(DataAnalyzer<double>::GetInversionsCount(currentSeqDouble)));
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onPrecedingSmaller()
{
    try {
        auto printResult = [&](auto* seq, auto* outPtr) {
            if (!seq) { logError("Нет последовательности"); return; }
            auto* result = DataAnalyzer<std::remove_pointer_t<decltype(seq->Get(0), seq)>>::
                GetPrecedingSmaller(seq, outPtr, seq);
            // Используем шаблонный вывод через Get
            for (size_t i = 0; i < result->GetLength(); ++i) {
                auto* sub = result->Get(i);
                QString s = QString("[%1]< %2: [").arg(i).arg(valueToString(seq->Get(i)));
                for (size_t j = 0; j < sub->GetLength(); ++j) {
                    if (j > 0) s += ", ";
                    s += valueToString(sub->Get(j));
                }
                log(s + "]");
            }
        };
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            MutableArraySequence<Sequence<int>*> out;
            auto* result = DataAnalyzer<int>::GetPrecedingSmaller(currentSeqInt, &out, currentSeqInt);
            for (size_t i = 0; i < result->GetLength(); ++i) {
                auto* sub = result->Get(i);
                QString s = QString("[%1] < %2: [").arg(i).arg(currentSeqInt->Get(i));
                for (size_t j = 0; j < sub->GetLength(); ++j) {
                    if (j > 0) s += ", ";
                    s += QString::number(sub->Get(j));
                }
                log(s + "]");
            }
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            MutableArraySequence<Sequence<double>*> out;
            auto* result = DataAnalyzer<double>::GetPrecedingSmaller(currentSeqDouble, &out, currentSeqDouble);
            for (size_t i = 0; i < result->GetLength(); ++i) {
                auto* sub = result->Get(i);
                QString s = QString("[%1] < %2: [").arg(i).arg(currentSeqDouble->Get(i));
                for (size_t j = 0; j < sub->GetLength(); ++j) {
                    if (j > 0) s += ", ";
                    s += QString::number(sub->Get(j));
                }
                log(s + "]");
            }
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onPrefixesPostfixes()
{
    try {
        auto print = [&](auto* seq) {
            if (!seq) { logError("Нет последовательности"); return; }
            MutableArraySequence<Sequence<std::remove_pointer_t<decltype(seq)>>*> out;
            // Нельзя шаблонно — делаем отдельно
        };
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            MutableArraySequence<Sequence<int>*> out;
            auto* result = DataAnalyzer<int>::GetPrefixesAndPostfixes(currentSeqInt, &out);
            for (size_t i = 0; i < result->GetLength(); ++i) {
                auto* sub = result->Get(i);
                QString s = "  [";
                for (size_t j = 0; j < sub->GetLength(); ++j) {
                    if (j > 0) s += ", ";
                    s += QString::number(sub->Get(j));
                }
                log(s + "]");
            }
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            MutableArraySequence<Sequence<double>*> out;
            auto* result = DataAnalyzer<double>::GetPrefixesAndPostfixes(currentSeqDouble, &out);
            for (size_t i = 0; i < result->GetLength(); ++i) {
                auto* sub = result->Get(i);
                QString s = "  [";
                for (size_t j = 0; j < sub->GetLength(); ++j) {
                    if (j > 0) s += ", ";
                    s += QString::number(sub->Get(j));
                }
                log(s + "]");
            }
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onMovingAverage()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            IntRing ring; MutableArraySequence<double> out;
            auto* r = DataAnalyzer<int>::GetMovingAverage(currentSeqInt, ring, &out);
            QString s = "Скользящее среднее: [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i), 'f', 2);
            }
            log(s + "]");
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            DoubleRing ring; MutableArraySequence<double> out;
            auto* r = DataAnalyzer<double>::GetMovingAverage(currentSeqDouble, ring, &out);
            QString s = "Скользящее среднее: [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i), 'f', 2);
            }
            log(s + "]");
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onSigmaDiff()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            IntRing ring; MutableArraySequence<double> out;
            auto* r = DataAnalyzer<int>::GetSigmaDifference(currentSeqInt, ring, &out);
            QString s = "√(σ²−aᵢ²): [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i), 'f', 2);
            }
            log(s + "]");
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            DoubleRing ring; MutableArraySequence<double> out;
            auto* r = DataAnalyzer<double>::GetSigmaDifference(currentSeqDouble, ring, &out);
            QString s = "√(σ²−aᵢ²): [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i), 'f', 2);
            }
            log(s + "]");
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}

void MainWindow::onReflectedSum()
{
    try {
        if (currentType == DataType::Int) {
            if (!currentSeqInt) { logError("Нет последовательности"); return; }
            IntRing ring; MutableArraySequence<int> out;
            auto* r = DataAnalyzer<int>::GetReflectedSum(currentSeqInt, ring, &out);
            QString s = "aᵢ + a[n-1-i]: [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i));
            }
            log(s + "]");
        } else if (currentType == DataType::Double) {
            if (!currentSeqDouble) { logError("Нет последовательности"); return; }
            DoubleRing ring; MutableArraySequence<double> out;
            auto* r = DataAnalyzer<double>::GetReflectedSum(currentSeqDouble, ring, &out);
            QString s = "aᵢ + a[n-1-i]: [";
            for (size_t i = 0; i < r->GetLength(); ++i) {
                if (i > 0) s += ", ";
                s += QString::number(r->Get(i));
            }
            log(s + "]");
        } else { logError("Анализ доступен только для int и double"); }
    } catch (const std::exception& e) { logError(e.what()); }
}
