#pragma once

#include <initializer_list>

namespace TestUtils {
    template<class T>
    testing::AssertionResult CheckSequence(
        std::initializer_list<T> expected, 
        Sequence<T>* actual, 
        const std::string& contextAction = "") 
    {
        if (actual == nullptr) {
            return testing::AssertionFailure() 
                << "[Action: " << contextAction << "]\n"
                << "ERROR: Actual sequence pointer is NULL!";
        }

        size_t expSize = expected.size();
        size_t actSize = actual->GetLength();
        
        bool match = (expSize == actSize);
        if (match) {
            size_t i = 0;
            for (const auto& val : expected) {
                if (val != actual->Get(i++)) {
                    match = false;
                    break;
                }
            }
        }

        if (match) return testing::AssertionSuccess();

        testing::AssertionResult failure = testing::AssertionFailure();
        if (!contextAction.empty()) {
            failure << "ACTION PERFORMED: " << contextAction << "\n";
        }
        failure << "SEQUENCE MISMATCH DETECTED!\n";

        failure << "EXPECTED: [";
        size_t idx = 0;
        for (const auto& val : expected) {
            failure << val << (idx++ < expSize - 1 ? ", " : "");
        }
        failure << "] (length: " << expSize << ")\n";

        failure << "RECEIVED: [";
        for (size_t i = 0; i < actSize; ++i) {
            failure << actual->Get(i) << (i < actSize - 1 ? ", " : "");
        }
        failure << "] (length: " << actSize << ")\n";

        return failure;
    }
}