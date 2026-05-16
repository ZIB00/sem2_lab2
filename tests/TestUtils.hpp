#pragma once

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "Sequence.hpp" // Убедитесь, что путь правильный

namespace TestUtils {

    // Функция, которая проверяет всю последовательность разом
    template<class T>
    testing::AssertionResult CheckSequence(
        const std::vector<T>& expected, 
        Sequence<T>* actual, 
        const std::string& contextAction = "") 
    {
        // 1. Защита от нулевых указателей
        if (actual == nullptr) {
            return testing::AssertionFailure() 
                << "[Action: " << contextAction << "]\n"
                << "ERROR: Actual sequence pointer is NULL!";
        }

        // 2. Проверка на совпадение длины и элементов
        bool match = true;
        if (expected.size() != actual->GetLength()) {
            match = false;
        } else {
            for (size_t i = 0; i < expected.size(); ++i) {
                if (expected[i] != actual->Get(i)) {
                    match = false;
                    break;
                }
            }
        }

        // Если всё совпало — тест пройден тихо
        if (match) {
            return testing::AssertionSuccess();
        }

        // 3. ЕСЛИ ТЕСТ УПАЛ — формируем красивый лог для преподавателя
        testing::AssertionResult failure = testing::AssertionFailure();
        
        if (!contextAction.empty()) {
            failure << "ACTION PERFORMED: " << contextAction << "\n";
        }
        
        failure << "SEQUENCE MISMATCH DETECTED!\n";
        
        // Выводим, что мы ожидали
        failure << "EXPECTED: [";
        for (size_t i = 0; i < expected.size(); ++i) {
            failure << expected[i] << (i < expected.size() - 1 ? ", " : "");
        }
        failure << "] (length: " << expected.size() << ")\n";

        // Выводим, что получилось на самом деле
        failure << "RECEIVED: [";
        for (size_t i = 0; i < actual->GetLength(); ++i) {
            failure << actual->Get(i) << (i < actual->GetLength() - 1 ? ", " : "");
        }
        failure << "] (length: " << actual->GetLength() << ")\n";

        return failure;
    }
}