#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    intMainSeq = nullptr;
    intAdditionalSeq = nullptr;
    intOriginalSeq = nullptr;

    doubleMainSeq = nullptr;
    doubleAdditionalSeq = nullptr;
    doubleOriginalSeq = nullptr;

    charMainSeq = nullptr;
    charAdditionalSeq = nullptr;
    charOriginalSeq = nullptr;

    SetupInterface();
}

MainWindow::~MainWindow() {
    if (intMainSeq) { delete intMainSeq; }
    if (intAdditionalSeq) { delete intAdditionalSeq; }
    if (intOriginalSeq) { delete intOriginalSeq; }

    if (doubleMainSeq) { delete doubleMainSeq; }
    if (doubleAdditionalSeq) { delete doubleAdditionalSeq; }
    if (doubleOriginalSeq) { delete doubleOriginalSeq; }

    if (charMainSeq) { delete charMainSeq; }
    if (charAdditionalSeq) { delete charAdditionalSeq; }
    if (charOriginalSeq) { delete charOriginalSeq; }
}

void MainWindow::ClearSequences() {
    if (intMainSeq) { delete intMainSeq; intMainSeq = nullptr; }
    if (intAdditionalSeq) { delete intAdditionalSeq; intAdditionalSeq = nullptr; }
    if (intOriginalSeq) { delete intOriginalSeq; intOriginalSeq = nullptr; }

    if (doubleMainSeq) { delete doubleMainSeq; doubleMainSeq = nullptr; }
    if (doubleAdditionalSeq) { delete doubleAdditionalSeq; doubleAdditionalSeq = nullptr; }
    if (doubleOriginalSeq) { delete doubleOriginalSeq; doubleOriginalSeq = nullptr; }

    if (charMainSeq) { delete charMainSeq; charMainSeq = nullptr; }
    if (charAdditionalSeq) { delete charAdditionalSeq; charAdditionalSeq = nullptr; }
    if (charOriginalSeq) { delete charOriginalSeq; charOriginalSeq = nullptr; }

    RefreshLists();
}

void MainWindow::SetupInterface() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Sequences");
    resize(1300, 800);

    QVBoxLayout* mainVBoxLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* topHBoxLayout = new QHBoxLayout();
    QGroupBox* topLeftBox = new QGroupBox("Создание последовательности", this);
    QGroupBox* topMiddleBox = new QGroupBox("Отображение последовательностей", this);
    QGroupBox* topRightBox = new QGroupBox("", this);

    topHBoxLayout->addWidget(topLeftBox);
    topHBoxLayout->addWidget(topMiddleBox);
    topHBoxLayout->addWidget(topRightBox);

    QGroupBox* logPanelBox = new QGroupBox("Лог операций", this);

    mainVBoxLayout->addLayout(topHBoxLayout);
    mainVBoxLayout->addWidget(logPanelBox);

    QHBoxLayout* middlePanelHBoxLayout = new QHBoxLayout(topMiddleBox);
    topMiddleBox->setLayout(middlePanelHBoxLayout);

    SetupLeftPanel(topLeftBox);
    SetupCentralPanel(middlePanelHBoxLayout);
    SetupRightPanel(topRightBox);
    SetupLogPanel(logPanelBox);

    connect(sequenceOptionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::HandleSequenceStructureChanged);
    connect(mutableOptionSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::HandleMutabilityChanged);

    HandleMutabilityChanged(mutableOptionSelector->currentIndex());
    HandleSequenceStructureChanged(sequenceOptionSelector->currentIndex());
}

void MainWindow::SetupLeftPanel(QGroupBox* leftPanelContainer) {
    QVBoxLayout* leftPanelLayout = new QVBoxLayout(leftPanelContainer);

    QPushButton* createHelpPushButton = new QPushButton("Справка", this);

    QLabel* sequenceOptionLabel = new QLabel("Вариант последовательности:", this);
    sequenceOptionSelector = new QComboBox(this);
    sequenceOptionSelector->addItems({"ArraySequence", 
                                    "ListSequence", 
                                    "BitSequence", 
                                    "AdaptiveArray", 
                                    "SegmentedList"});

    QLabel* mutableOptionLabel = new QLabel("Изменяемость:", this);
    mutableOptionSelector = new QComboBox(this);
    mutableOptionSelector->addItems({"Изменяемая", 
                                    "Неизменяемая"});

    QLabel* sequenceTypeLabel = new QLabel("Тип последовательности:", this);
    sequenceTypeSelector = new QComboBox(this);
    sequenceTypeSelector->addItems({"Целые числа", 
                                    "Вещественные числа", 
                                    "Символы"});

    QLabel* valueInputLabel = new QLabel("Ввод последовательности:", this);
    leftLineEdit = new QLineEdit(this);
    leftLineEdit->setPlaceholderText("Введите последовательность...");

    QLabel* createLabel = new QLabel("Создать:", this);
    QPushButton* createMainSequencePushButton = new QPushButton("Основную", this);
    createAdditionalSequencePushButton = new QPushButton("Дополнительную", this);

    leftPanelLayout->addWidget(createHelpPushButton);
    leftPanelLayout->addWidget(sequenceOptionLabel);
    leftPanelLayout->addWidget(sequenceOptionSelector);
    leftPanelLayout->addWidget(mutableOptionLabel);
    leftPanelLayout->addWidget(mutableOptionSelector);
    leftPanelLayout->addWidget(sequenceTypeLabel);
    leftPanelLayout->addWidget(sequenceTypeSelector);
    leftPanelLayout->addWidget(valueInputLabel);
    leftPanelLayout->addWidget(leftLineEdit);
    leftPanelLayout->addWidget(createLabel);
    leftPanelLayout->addWidget(createMainSequencePushButton);
    leftPanelLayout->addWidget(createAdditionalSequencePushButton);

    leftPanelLayout->addStretch();

    leftPanelContainer->setLayout(leftPanelLayout);

    connect(createHelpPushButton, &QPushButton::clicked, this, &MainWindow::onHelp);
    connect(createMainSequencePushButton, &QPushButton::clicked, this, &MainWindow::onMainCreate);
    connect(createAdditionalSequencePushButton, &QPushButton::clicked, this, &MainWindow::onAdditionalCreate);
}

void MainWindow::SetupCentralPanel(QHBoxLayout* centralPanelContainer) {
    QGroupBox* firstPanelBox = new QGroupBox("Основная", this);
    QVBoxLayout* firstPanelLayout = new QVBoxLayout(firstPanelBox);
    firstPanelList = new QListWidget(this);
    firstPanelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    firstPanelLayout->addWidget(firstPanelList);
    firstPanelBox->setLayout(firstPanelLayout);

    secondPanelBox = new QGroupBox("Дополнительная", this);
    QVBoxLayout* secondPanelLayout = new QVBoxLayout(secondPanelBox);
    secondPanelList = new QListWidget(this);
    secondPanelList->setSelectionMode(QAbstractItemView::NoSelection);
    secondPanelLayout->addWidget(secondPanelList);
    secondPanelBox->setLayout(secondPanelLayout);

    thirdPanelBox = new QGroupBox("Оригинал", this);
    QVBoxLayout* thirdPanelLayout = new QVBoxLayout(thirdPanelBox);
    thirdPanelList = new QListWidget(this);
    thirdPanelList->setSelectionMode(QAbstractItemView::NoSelection);
    thirdPanelLayout->addWidget(thirdPanelList);
    thirdPanelBox->setLayout(thirdPanelLayout);

    centralPanelContainer->addWidget(firstPanelBox);
    centralPanelContainer->addWidget(secondPanelBox);
    centralPanelContainer->addWidget(thirdPanelBox);
}

void MainWindow::SetupRightPanel(QGroupBox* rightPanelContainer) {
    QVBoxLayout *mainLayout = new QVBoxLayout(rightPanelContainer);

    rightTabWidget = new QTabWidget(this);

    QWidget* sequenceFunctions = new QWidget();
    QVBoxLayout* sequenceFunctionsVBoxLayout = new QVBoxLayout(sequenceFunctions);
    QLabel* valueInputFunctionalLabel = new QLabel("Ввод значения:", this);
    rightLineEditFunctional = new QLineEdit(this);
    rightLineEditFunctional->setPlaceholderText("Введите значение...");
    sequenceFunctionsVBoxLayout->addWidget(valueInputFunctionalLabel);
    sequenceFunctionsVBoxLayout->addWidget(rightLineEditFunctional);

    QPushButton* buttonPrepend = new QPushButton("Добавить в начало");
    QPushButton* buttonAppend = new QPushButton("Добавить в конец");
    QPushButton* buttonInsert = new QPushButton("Вставить после выбранного");
    QPushButton* buttonSubseq = new QPushButton("Получить подпослед.");
    QPushButton* buttonConcat = new QPushButton("Объеденить с новой послед.");
    QPushButton* buttonFirst = new QPushButton("Получить первое");
    QPushButton* buttonLast = new QPushButton("Получить последнее");
    QPushButton* buttonSelected = new QPushButton("Получить выбранное");

    sequenceFunctionsVBoxLayout->addWidget(buttonPrepend);
    sequenceFunctionsVBoxLayout->addWidget(buttonAppend);
    sequenceFunctionsVBoxLayout->addWidget(buttonInsert);
    sequenceFunctionsVBoxLayout->addWidget(buttonSubseq);
    sequenceFunctionsVBoxLayout->addWidget(buttonConcat);
    sequenceFunctionsVBoxLayout->addWidget(buttonFirst);
    sequenceFunctionsVBoxLayout->addWidget(buttonLast);
    sequenceFunctionsVBoxLayout->addWidget(buttonSelected);

    sequenceFunctionsVBoxLayout->addWidget(new QLabel("Логические операции (итог сверху)"));
    buttonNot = new QPushButton("НЕ", this);
    buttonAnd = new QPushButton("И", this);
    buttonOr = new QPushButton("ИЛИ", this);
    buttonXor = new QPushButton("Исключающее ИЛИ", this);

    sequenceFunctionsVBoxLayout->addWidget(buttonNot);
    sequenceFunctionsVBoxLayout->addWidget(buttonAnd);
    sequenceFunctionsVBoxLayout->addWidget(buttonOr);
    sequenceFunctionsVBoxLayout->addWidget(buttonXor);
    sequenceFunctionsVBoxLayout->addStretch();
    rightTabWidget->addTab(sequenceFunctions, "Функции");

    QWidget* sequenceUtilts = new QWidget();
    QVBoxLayout* sequenceUtiltsVBoxLayout = new QVBoxLayout(sequenceUtilts);
    QLabel* valueInputUtiltsLabel = new QLabel("Ввод значениий:", this);
    rightLineEditUtilts = new QLineEdit(this);
    rightLineEditUtilts->setPlaceholderText("Введите значения...");
    QPushButton* buttonZip = new QPushButton("Zip");
    QPushButton* buttonUnzip = new QPushButton("Unzip");
    QPushButton* buttonRange = new QPushButton("Range");
    QPushButton* buttonSplit = new QPushButton("Split");
    QPushButton* buttonFlatMap = new QPushButton("FlatMap");
    QPushButton* buttonSkip = new QPushButton("Skip");
    QPushButton* buttonSplice = new QPushButton("Splice");
    QPushButton* buttonMap = new QPushButton("Map");
    QPushButton* buttonWhere = new QPushButton("Where");
    QPushButton* buttonReduce = new QPushButton("Reduce");
    QPushButton* buttonGetFirstOpt = new QPushButton("GetFirst");
    QPushButton* buttonGetLastOpt = new QPushButton("GetLast");

    sequenceUtiltsVBoxLayout->addWidget(valueInputUtiltsLabel);
    sequenceUtiltsVBoxLayout->addWidget(rightLineEditUtilts);
    sequenceUtiltsVBoxLayout->addWidget(buttonZip);
    sequenceUtiltsVBoxLayout->addWidget(buttonUnzip);
    sequenceUtiltsVBoxLayout->addWidget(buttonRange);
    sequenceUtiltsVBoxLayout->addWidget(buttonSplit);
    sequenceUtiltsVBoxLayout->addWidget(buttonFlatMap);
    sequenceUtiltsVBoxLayout->addWidget(buttonSkip);
    sequenceUtiltsVBoxLayout->addWidget(buttonSplice);
    sequenceUtiltsVBoxLayout->addWidget(buttonMap);
    sequenceUtiltsVBoxLayout->addWidget(buttonWhere);
    sequenceUtiltsVBoxLayout->addWidget(buttonReduce);
    sequenceUtiltsVBoxLayout->addWidget(buttonGetFirstOpt);
    sequenceUtiltsVBoxLayout->addWidget(buttonGetLastOpt);
    sequenceUtiltsVBoxLayout->addStretch();
    rightTabWidget->addTab(sequenceUtilts, "Утилиты");

    QWidget* sequenceDataAnalysis = new QWidget();
    QVBoxLayout* sequenceDataAnalysisVBoxLayout = new QVBoxLayout(sequenceDataAnalysis);
    sequenceDataAnalysisVBoxLayout->addWidget(new QLabel("Получить значение"));
    QPushButton* buttonStats = new QPushButton("Мин/Макс/Средн.");
    QPushButton* buttonPermutations = new QPushButton("Кол-во перестановок");
    sequenceDataAnalysisVBoxLayout->addWidget(buttonStats);
    sequenceDataAnalysisVBoxLayout->addWidget(buttonPermutations);

    sequenceDataAnalysisVBoxLayout->addWidget(new QLabel("Получить список"));
    QPushButton* buttonSmaller = new QPushButton("Меньших элементов до него");
    QPushButton* buttonMovingAvg = new QPushButton("Скользящих средних");
    QPushButton* buttonSigma = new QPushButton("разниц сигм");
    QPushButton* buttonReflected = new QPushButton("сумм с «отражением»");

    sequenceDataAnalysisVBoxLayout->addWidget(buttonSmaller);
    sequenceDataAnalysisVBoxLayout->addWidget(buttonMovingAvg);
    sequenceDataAnalysisVBoxLayout->addWidget(buttonSigma);
    sequenceDataAnalysisVBoxLayout->addWidget(buttonReflected);
    sequenceDataAnalysisVBoxLayout->addStretch();
    rightTabWidget->addTab(sequenceDataAnalysis, "Анализ");

    mainLayout->addWidget(rightTabWidget);
    rightPanelContainer->setLayout(mainLayout);

    connect(rightTabWidget, &QTabWidget::currentChanged, this, &MainWindow::HandleTabChanged);
    connect(buttonPrepend, &QPushButton::clicked, this, &MainWindow::onPrepend);
    connect(buttonAppend, &QPushButton::clicked, this, &MainWindow::onAppend);
    connect(buttonInsert, &QPushButton::clicked, this, &MainWindow::onInsert);
    connect(buttonSubseq, &QPushButton::clicked, this, &MainWindow::onSubsequence);
    connect(buttonConcat, &QPushButton::clicked, this, &MainWindow::onConcat);
    connect(buttonFirst, &QPushButton::clicked, this, &MainWindow::onFirst);
    connect(buttonLast, &QPushButton::clicked, this, &MainWindow::onLast);
    connect(buttonSelected, &QPushButton::clicked, this, &MainWindow::onSelected);
    connect(buttonNot, &QPushButton::clicked, this, &MainWindow::onBitNot);
    connect(buttonAnd, &QPushButton::clicked, this, &MainWindow::onBitAnd);
    connect(buttonOr, &QPushButton::clicked, this, &MainWindow::onBitOr);
    connect(buttonXor, &QPushButton::clicked, this, &MainWindow::onBitXor);
    connect(buttonZip, &QPushButton::clicked, this, &MainWindow::onZip);
    connect(buttonUnzip, &QPushButton::clicked, this, &MainWindow::onUnzip);
    connect(buttonRange, &QPushButton::clicked, this, &MainWindow::onRange);
    connect(buttonSplit, &QPushButton::clicked, this, &MainWindow::onSplit);
    connect(buttonFlatMap, &QPushButton::clicked, this, &MainWindow::onFlatMap);
    connect(buttonSkip, &QPushButton::clicked, this, &MainWindow::onSkip);
    connect(buttonSplice, &QPushButton::clicked, this, &MainWindow::onSplice);
    connect(buttonMap, &QPushButton::clicked, this, &MainWindow::onMap);
    connect(buttonWhere, &QPushButton::clicked, this, &MainWindow::onWhere);
    connect(buttonReduce, &QPushButton::clicked, this, &MainWindow::onReduce);
    connect(buttonGetFirstOpt, &QPushButton::clicked, this, &MainWindow::onGetFirstOpt);
    connect(buttonGetLastOpt, &QPushButton::clicked, this, &MainWindow::onGetLastOpt);
    connect(buttonStats, &QPushButton::clicked, this, &MainWindow::onStats);
    connect(buttonPermutations, &QPushButton::clicked, this, &MainWindow::onPermutations);
    connect(buttonSmaller, &QPushButton::clicked, this, &MainWindow::onSmaller);
    connect(buttonMovingAvg, &QPushButton::clicked, this, &MainWindow::onMovingAvg);
    connect(buttonSigma, &QPushButton::clicked, this, &MainWindow::onSigma);
    connect(buttonReflected, &QPushButton::clicked, this, &MainWindow::onReflected);
}

void MainWindow::SetupLogPanel(QGroupBox* logPanelContainer) {
    QVBoxLayout* logPanelLayout = new QVBoxLayout(logPanelContainer);

    globalLog = new QTextEdit(logPanelContainer);
    globalLog->setReadOnly(true);

    logPanelLayout->addWidget(globalLog);
}

void MainWindow::onHelp() {
    QString helpText =
        "=== Руководство пользователя ===\n\n"

        "[ Создание последовательности ]\n"
        "1. Выберите структуру, тип данных и вариант изменяемости.\n"
        "2. Введите начальные элементы через пробел или запятую в левое поле.\n"
        "3. Нажмите 'Основную' или 'Дополнительную', чтобы создать список.\n\n"

        "[ Списки элементов (Панель по центру) ]\n"
        "- Основная: Ваша текущая рабочая последовательность. Вы можете кликать по элементам, чтобы выделить их. Для выделения нескольких элементов зажмите Ctrl или Shift.\n"
        "- Дополнительная: Она нужна как второй участник для объединения (Concat), Zip и битовых операций.\n"
        "- Оригинал: Показывает исходную версию списка в момент создания (доступно только для неизменяемых структур).\n\n"

        "[ Правое текстовое поле (Ввод значений) ]\n"
        "Используется для передачи параметров в кнопки:\n"
        "- Добавить / Вставить: одно значение.\n"
        "- Range (Генерация): три числа через пробел (начало конец шаг, пример: 0 10 2).\n"
        "- Skip (Пропуск): число элементов, которые нужно пропустить с начала.\n"
        "- Splice (Удаление): индекс элемента (начиная с 0).\n"
        "- GetFirst / GetLast: число-порог для поиска (>= или <=).\n\n"

        "[ Вкладка 'Функции' ]\n"
        "- Добавить в начало/конец: берет число из правого поля.\n"
        "- Вставить после выбранного: выделите один элемент в 'Основной', введите число справа и нажмите кнопку.\n"
        "- Получить подпослед.: выделите несколько элементов в 'Основной' (рамкой или зажав Ctrl/Shift). Программа сама возьмет отрезок от самого верхнего до самого нижнего выделенного.\n"
        "- Объединить (Concat): приклеивает 'Дополнительную' в конец 'Основной'.\n"
        "- Получить первое/последнее/выбранное: печатает элемент в лог внизу.\n"
        "- Битовые операции (НЕ, И, ИЛИ, XOR): для двуместных операций нужны заполненные 'Основная' и 'Дополнительная' последовательности.\n\n"

        "[ Вкладка 'Утилиты' (Демонстрация работы) ]\n"
        "- Zip: собирает элементы двух списков в пары.\n"
        "- Unzip: разбивает пары обратно на два списка (выводит в лог).\n"
        "- Range: создает новый список чисел по вашим параметрам из правого поля.\n"
        "- Split: разрезает список там, где встречаются нули (для чисел) или символ '_' (для букв).\n"
        "- FlatMap: берет элемент X и делает из него два: [X, X*2].\n"
        "- Skip: пропускает N элементов с начала.\n"
        "- Splice: вырезает элемент по указанному справа индексу.\n"
        "- Map: умножает все элементы на 2.\n"
        "- Where: оставляет только неотрицательные числа.\n"
        "- Reduce: складывает все числа вместе (сумма в лог).\n"
        "- GetFirst / GetLast: находит первое или последнее число, подходящее под порог из поля.\n\n"

        "[ Вкладка 'Анализ' (Все результаты идут в Лог) ]\n"
        "- Мин/Макс/Средн.: сбор статистики.\n"
        "- Кол-во перестановок: расчет числа инверсий.\n"
        "- Меньших элементов: для каждого числа показывает, какие числа до него были меньше.\n"
        "- Скользящих средних: сглаживание по 3 элементам.\n"
        "- Разниц сигм / Отражение: применение формул анализа к списку.";

    QMessageBox::information(this, "Справка", helpText);
}

void MainWindow::HandleSequenceStructureChanged(int index) {
    if (index == 0 || index == 1) {
        mutableOptionSelector->setEnabled(true);
    } else {
        mutableOptionSelector->setEnabled(false);
        mutableOptionSelector->setCurrentIndex(0);
    }

    bool isBitSequence = (index == 2);
    buttonNot->setEnabled(isBitSequence);
    buttonAnd->setEnabled(isBitSequence);
    buttonOr->setEnabled(isBitSequence);
    buttonXor->setEnabled(isBitSequence);

    if (isBitSequence) {
        sequenceTypeSelector->setCurrentIndex(0);
        sequenceTypeSelector->setEnabled(false);

        QRegularExpression rx("[01\\s,]*");
        
        if (!leftLineEdit->validator()) {
            leftLineEdit->setValidator(new QRegularExpressionValidator(rx, leftLineEdit));
        }
        if (!rightLineEditFunctional->validator()) {
            rightLineEditFunctional->setValidator(new QRegularExpressionValidator(rx, rightLineEditFunctional));
        }
    } else {
        sequenceTypeSelector->setEnabled(true);
        
        auto* model = qobject_cast<QStandardItemModel*>(sequenceTypeSelector->model());
        if (model) {
            model->item(1)->setEnabled(true);
            if (rightTabWidget->currentIndex() != 2) {
                model->item(2)->setEnabled(true);
            }
        }

        if (leftLineEdit->validator()) {
            const_cast<QValidator*>(leftLineEdit->validator())->deleteLater();
            leftLineEdit->setValidator(nullptr);
        }
        if (rightLineEditFunctional->validator()) {
            const_cast<QValidator*>(rightLineEditFunctional->validator())->deleteLater();
            rightLineEditFunctional->setValidator(nullptr);
        }
    }
}

void MainWindow::HandleMutabilityChanged(int index) {
    bool isImmutable = (index == 1);
    
    thirdPanelBox->setVisible(isImmutable);
}

void MainWindow::HandleTabChanged(int index) {
    auto* model = qobject_cast<QStandardItemModel*>(sequenceTypeSelector->model());
    
    if (index == 2) { 
        if (sequenceTypeSelector->currentIndex() == 2) {
            sequenceTypeSelector->setCurrentIndex(0);
        }
        
        if (model) {
            model->item(2)->setEnabled(false);
        }

        secondPanelBox->setVisible(false);
        thirdPanelBox->setVisible(false);
        createAdditionalSequencePushButton->setEnabled(false);
        firstPanelList->setSelectionMode(QAbstractItemView::NoSelection);
        
        ClearSequences();
        
    } else { 
        if (model) {
            model->item(2)->setEnabled(true);
        }
        
        if (sequenceOptionSelector->currentIndex() != 2) {
            sequenceTypeSelector->setEnabled(true);
        }
        
        secondPanelBox->setVisible(true);
        createAdditionalSequencePushButton->setEnabled(true);
        firstPanelList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void MainWindow::RefreshLists() {
    int currentType = sequenceTypeSelector->currentIndex();

    if (currentType == 0) {
        TemplatePopulateList(firstPanelList, intMainSeq);
        TemplatePopulateList(secondPanelList, intAdditionalSeq);
        TemplatePopulateList(thirdPanelList, intOriginalSeq);
    } else if (currentType == 1) {
        TemplatePopulateList(firstPanelList, doubleMainSeq);
        TemplatePopulateList(secondPanelList, doubleAdditionalSeq);
        TemplatePopulateList(thirdPanelList, doubleOriginalSeq);
    } else {
        TemplatePopulateList(firstPanelList, charMainSeq);
        TemplatePopulateList(secondPanelList, charAdditionalSeq);
        TemplatePopulateList(thirdPanelList, charOriginalSeq);
    }
}

void MainWindow::onMainCreate() {
    int option = sequenceOptionSelector->currentIndex();
    int mutability = mutableOptionSelector->currentIndex();
    int type = sequenceTypeSelector->currentIndex();
    QString inputData = leftLineEdit->text();
    
    try {
        if (type == 0) {
            TemplateCreate(intMainSeq, intOriginalSeq, option, mutability, inputData);
        } else if (type == 1) {
            TemplateCreate(doubleMainSeq, doubleOriginalSeq, option, mutability, inputData);
        } else {
            TemplateCreate(charMainSeq, charOriginalSeq, option, mutability, inputData);
        }
        
        globalLog->append("Основная последовательность успешно инициализирована.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}

void MainWindow::onAdditionalCreate() {
    int option = sequenceOptionSelector->currentIndex();
    int mutability = mutableOptionSelector->currentIndex();
    int type = sequenceTypeSelector->currentIndex();
    QString inputData = leftLineEdit->text();
    
    try {
        if (type == 0) {
            TemplateCreate(intAdditionalSeq, option, mutability, inputData);
        } else if (type == 1) {
            TemplateCreate(doubleAdditionalSeq, option, mutability, inputData);
        } else {
            TemplateCreate(charAdditionalSeq, option, mutability, inputData);
        }
        
        globalLog->append("Дополнительная последовательность успешно инициализирована.");
        RefreshLists();
    } CATCH_ALL_EXCEPTIONS
}