#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QStringList>
#include <QMessageBox>
#include <QDialog>
#include <QStandardItemModel>
#include <QRegularExpression>

#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "BitSequence.hpp"
#include "AdaptiveSequence.hpp"
#include "SegmentedList.hpp"
#include "SequenceUtils.hpp"
#include "DataAnalyzer.hpp"

#define CATCH_ALL_EXCEPTIONS \
    catch (const OutOfRange& e) { globalLog->append(QString(e.what())); } \
    catch (const InvalidArgument& e) { globalLog->append(QString(e.what())); } \
    catch (const LogicError& e) { globalLog->append(QString(e.what())); } \
    catch (const OtherError& e) { globalLog->append(QString(e.what())); }

inline QString ElementToString(int value) { return QString::number(value); }
inline QString ElementToString(double value) { return QString::number(value); }
inline QString ElementToString(char16_t value) { return QString(QChar(static_cast<ushort>(value))); }

inline void ParseStringToken(const QString& token, int& outValue) {
    bool ok = false;
    outValue = token.toInt(&ok);
    
    if (!ok) {
        QRegularExpression regex("^[-+]?\\d+$");
        if (regex.match(token).hasMatch()) {
            throw OutOfRange("Ошибка ввода: число '" + token.toStdString() + "' выходит за границы вместимости типа int.");
        } else {
            throw InvalidArgument("Ошибка ввода: '" + token.toStdString() + "' содержит недопустимые символы.");
        }
    }
}

inline void ParseStringToken(const QString& token, double& outValue) {
    bool ok = false;
    outValue = token.toDouble(&ok);
    if (!ok) {
        throw InvalidArgument("Ошибка ввода: '" + token.toStdString() + "' не является вещественным числом.");
    }
}

inline void ParseStringToken(const QString& token, char16_t& outValue) {
    if (token.length() != 1) {
        throw InvalidArgument("Ошибка ввода: ожидается ровно один символ, а получено '" + token.toStdString() + "'.");
    }
    outValue = token.at(0).unicode();
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void SetupInterface();
    void SetupLeftPanel(QGroupBox* leftPanelContainer);
    void SetupCentralPanel(QHBoxLayout* centralPanelContainer);
    void SetupRightPanel(QGroupBox* rightPanelContainer);
    void SetupLogPanel(QGroupBox* logPanelContainer);
    void ClearSequences();

    void RefreshLists();

    void onHelp();
    void onMainCreate();
    void onAdditionalCreate();
    void onPrepend();
    void onAppend();
    void onInsert();
    void onSubsequence();
    void onConcat();
    void onFirst();
    void onLast();
    void onSelected();
    void onBitNot();
    void onBitAnd();
    void onBitOr();
    void onBitXor();
    void onZip();
    void onUnzip();
    void onRange();
    void onSplit();
    void onFlatMap();
    void onSkip();
    void onSplice();
    void onMap();
    void onWhere();
    void onReduce();
    void onGetFirstOpt();
    void onGetLastOpt();
    void onStats();
    void onPermutations();
    void onSmaller();
    void onMovingAvg();
    void onSigma();
    void onReflected();

    void HandleTabChanged(int index);
    void HandleSequenceStructureChanged(int index);
    void HandleMutabilityChanged(int index);

    template<typename T>
    Sequence<T>* NewSequenceInstance(int option, int mutability);

    template<typename T>
    void TemplatePopulateList(QListWidget* targetList, Sequence<T>* targetSequence);

    template<typename T>
    void TemplateCreate(Sequence<T>*& mainSeq, Sequence<T>*& origSeq, int option, int mutability, const QString& input);

    template<typename T>
    void TemplateCreate(Sequence<T>*& mainSeq, int option, int mutability, const QString& input);

    QComboBox* sequenceOptionSelector;
    QComboBox* mutableOptionSelector;
    QComboBox* sequenceTypeSelector;
    QLineEdit* leftLineEdit;
    QLineEdit* rightLineEditFunctional;
    QLineEdit* rightLineEditUtilts;

    QGroupBox* secondPanelBox;
    QGroupBox* thirdPanelBox;

    QListWidget* firstPanelList;
    QListWidget* secondPanelList;
    QListWidget* thirdPanelList;

    QTextEdit* globalLog;

    QPushButton* createAdditionalSequencePushButton;
    QPushButton* buttonNot;
    QPushButton* buttonAnd;
    QPushButton* buttonOr;
    QPushButton* buttonXor;

    QTabWidget* rightTabWidget;

    Sequence<int>* intMainSeq;
    Sequence<int>* intAdditionalSeq;
    Sequence<int>* intOriginalSeq;

    Sequence<double>* doubleMainSeq;
    Sequence<double>* doubleAdditionalSeq;
    Sequence<double>* doubleOriginalSeq;

    Sequence<char16_t>* charMainSeq;
    Sequence<char16_t>* charAdditionalSeq;
    Sequence<char16_t>* charOriginalSeq;
};

template<typename T>
void MainWindow::TemplatePopulateList(QListWidget* targetList, Sequence<T>* targetSequence) {
    targetList->clear();
    if (targetSequence == nullptr) return;
    for (size_t index = 0; index < targetSequence->GetLength(); ++index) {
        targetList->addItem(ElementToString(targetSequence->Get(index)));
    }
}

template<typename T>
void MainWindow::TemplateCreate(Sequence<T>*& mainSeq, Sequence<T>*& origSeq, int option, int mutability, const QString& input) {
    if (mainSeq) { delete mainSeq; mainSeq = nullptr; }
    if (origSeq) { delete origSeq; origSeq = nullptr; }

    mainSeq = NewSequenceInstance<T>(option, mutability);
    origSeq = NewSequenceInstance<T>(option, mutability);

    QStringList tokens = input.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    for (const QString& token : tokens) {
        T value;
        ParseStringToken(token, value); 

        if (option == 2) {
            int intVal = static_cast<int>(value);
            if (intVal != 0 && intVal != 1) {
                throw InvalidArgument("Ошибка: для BitSequence биты должны вводиться через пробел. '" + token.toStdString() + "' недопустимо.");
            }
        }
        
        mainSeq = mainSeq->Append(value);
        origSeq = origSeq->Append(value);
    }
}

template<typename T>
void MainWindow::TemplateCreate(Sequence<T>*& targetSeq, int option, int mutability, const QString& input) {
    if (targetSeq) { delete targetSeq; targetSeq = nullptr; }

    targetSeq = NewSequenceInstance<T>(option, mutability);

    QStringList tokens = input.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    for (const QString& token : tokens) {
        T value;
        ParseStringToken(token, value); 

        if (option == 2) {
            int intVal = static_cast<int>(value);
            if (intVal != 0 && intVal != 1) {
                throw InvalidArgument("Ошибка: для BitSequence биты должны вводиться через пробел. '" + token.toStdString() + "' недопустимо.");
            }
        }

        targetSeq = targetSeq->Append(value);
    }
}

template<typename T>
Sequence<T>* MainWindow::NewSequenceInstance(int option, int mutability) {
    if (option == 0) {
        if (mutability == 0) { return new MutableArraySequence<T>(); }
        else { return new ImmutableArraySequence<T>(); }
    }
    if (option == 1) {
        if (mutability == 0) { return new MutableListSequence<T>(); }
        else { return new ImmutableListSequence<T>(); }
    }
    if (option == 2) { return new BitSequence<T>(); }
    if (option == 3) { return new AdaptiveSequence<T>(); }
    if (option == 4) { return new SegmentedList<T>(); }
    return nullptr;
}