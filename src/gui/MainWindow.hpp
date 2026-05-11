#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDialog>
#include <QMessageBox>
#include <variant>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "AdaptiveSequence.hpp"
#include "SegmentedList.hpp"
#include "BitSequence.hpp"
#include "SequenceUtils.hpp"
#include "DataAnalyzer.hpp"
#include "Realizations.hpp"

// Тип активной последовательности
enum class DataType { Int, Double, Char };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // ── Текущие последовательности ────────────────────────────────────────
    DataType currentType = DataType::Int;

    Sequence<int>*    currentSeqInt    = nullptr;
    Sequence<double>* currentSeqDouble = nullptr;
    Sequence<char16_t>*   currentSeqChar   = nullptr;

    // Для immutable — хранится оригинал до операции
    Sequence<int>*    originalSeqInt    = nullptr;
    Sequence<double>* originalSeqDouble = nullptr;
    Sequence<char16_t>*   originalSeqChar   = nullptr;

    bool isImmutable() const { return mutabilityCombo->currentIndex() == 1; }
    bool hasNoMutability() const {
        QString t = seqTypeCombo->currentText();
        return t == "AdaptiveSequence" || t == "SegmentedList" || t == "BitSequence";
    }

    // ── Строители панелей ─────────────────────────────────────────────────
    QSplitter*  mainSplitter;
    QGroupBox*  createPanel();
    QGroupBox*  displayPanel();
    QTabWidget* operationsPanel();
    QWidget*    buildMethodsTab();
    QWidget*    buildFunctionsTab();
    QWidget*    buildUtilsTab();
    QWidget*    buildAnalysisTab();

    // ── Вспомогательные методы ────────────────────────────────────────────
    void   log(const QString& msg);
    void   logError(const QString& msg);   // красный + без выполнения
    void   refreshDisplay();

    // Парсеры с валидацией — возвращают false и логируют если ввод некорректный
    bool   parseIntVal(const QString& text, int& out);
    bool   parseDoubleVal(const QString& text, double& out);
    bool   parseIndex(const QString& text, size_t& out);
    bool   parseIntList(const QString& text, QList<int>& out);
    bool   parseDoubleList(const QString& text, QList<double>& out);
    bool   parseCharList(const QString& text, QList<char16_t>& out);

    // Сохранить оригинал перед immutable-операцией
    void   saveOriginal();
    void   clearOriginal();

    // ── Виджеты левой панели ──────────────────────────────────────────────
    QPushButton* instructionButton;
    QComboBox*   dataTypeCombo;    // int / double / char16_t
    QComboBox*   seqTypeCombo;
    QComboBox*   mutabilityCombo;
    QLineEdit*   elementsInput;
    QPushButton* createButton;

    // ── Виджеты центральной панели ────────────────────────────────────────
    QListWidget* seqDisplay;
    QListWidget* origDisplay;    // оригинал для immutable
    QLabel*      seqInfoLabel;
    QLabel*      origInfoLabel;
    QGroupBox*   origGroup;      // скрывается в mutable режиме

    // ── Вкладка "Методы" ──────────────────────────────────────────────────
    QLineEdit* indexInput;
    QLineEdit* appendInput;
    QLineEdit* prependInput;
    QLineEdit* insertValueInput;
    QLineEdit* insertIndexInput;
    QLineEdit* subStartInput;
    QLineEdit* subEndInput;

    // ── Вкладка "Функции" ─────────────────────────────────────────────────
    QComboBox* mapFuncCombo;
    QComboBox* whereFuncCombo;
    QComboBox* reduceFuncCombo;
    QComboBox* flatMapFuncCombo;
    QLineEdit* skipInput;
    QLineEdit* spliceIndexInput;
    QLineEdit* spliceCountInput;

    // ── Вкладка "Утилиты" ─────────────────────────────────────────────────
    QLineEdit* zipInput;
    QLineEdit* unzipLabel;   // только отображение
    QLineEdit* rangeStartInput;
    QLineEdit* rangeEndInput;
    QLineEdit* rangeStepInput;

    // ── Лог ───────────────────────────────────────────────────────────────
    QTextEdit* logOutput;

private slots:
    void onCreateSequence();
    void onSeqTypeChanged();
    void onShowInstructions();
    // Методы
    void onGet();
    void onAppend();
    void onPrepend();
    void onInsertAt();
    void onGetSubsequence();
    void onGetFirst();
    void onGetLast();
    // Функции
    void onMap();
    void onWhere();
    void onReduce();
    void onFlatMap();
    void onSkip();
    void onSplice();
    // Утилиты
    void onZip();
    void onUnzip();
    void onSplit();
    void onRange();
    // Анализ
    void onBasicStats();
    void onMedian();
    void onInversions();
    void onPrecedingSmaller();
    void onPrefixesPostfixes();
    void onMovingAverage();
    void onSigmaDiff();
    void onReflectedSum();
};

// ── Диалог справки ────────────────────────────────────────────────────────────
class InstructionDialog : public QDialog {
public:
    InstructionDialog(QWidget* parent) : QDialog(parent) {
        setWindowTitle("Справка");
        setMinimumSize(500, 400);
        QVBoxLayout* layout = new QVBoxLayout(this);
        QTextEdit* text = new QTextEdit(this);
        text->setReadOnly(true);
        text->setHtml(
            "<h2>Создание</h2>"
            "<p><b>Тип данных:</b> int / double / char16_t.</p>"
            "<p><b>Структура:</b> Array, List, Adaptive, Segmented, Bit.</p>"
            "<p><b>Adaptive / Segmented / Bit</b> — только Mutable.</p>"
            "<p><b>Immutable:</b> операции не меняют оригинал — результат показывается рядом.</p>"
            "<h2>Ввод элементов</h2>"
            "<p>Числа — как обычно; для символов вводите строку подряд, каждый знак станет элементом.</p>"
            "<p>Некорректные символы отклоняются с сообщением об ошибке.</p>"
            "<h2>Утилиты</h2>"
            "<p><b>Zip</b> — объединить текущую с другой в пары.</p>"
            "<p><b>Unzip</b> — разбить текущую последовательность пар обратно.</p>"
            "<p><b>Split</b> — разбить по нулям (int) / пробелам (char16_t).</p>"
            "<p><b>Range</b> — заполнить диапазоном [start, end] с шагом step.</p>"
        );
        QPushButton* btn = new QPushButton("Понятно", this);
        connect(btn, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(text);
        layout->addWidget(btn);
    }
};
