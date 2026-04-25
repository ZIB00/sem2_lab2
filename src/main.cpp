#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "ListSequence.hpp"

namespace
{
    void ClearInput()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int ReadInt(const std::string& prompt)
    {
        while (true) {
            std::cout << prompt;

            int value = 0;
            if (std::cin >> value) {
                return value;
            }

            std::cout << "Please enter an integer.\n";
            ClearInput();
        }
    }

    void PrintSequence(Sequence<int>* sequence)
    {
        if (sequence == nullptr) {
            std::cout << "No active sequence.\n";
            return;
        }

        std::cout << "[";

        for (int index = 0; index < sequence->GetLength(); ++index) {
            if (index > 0) {
                std::cout << ", ";
            }

            std::cout << sequence->Get(index);
        }

        std::cout << "]\n";
    }

    std::unique_ptr<Sequence<int>> CreateSequence(int type)
    {
        int count = ReadInt("Element count: ");

        if (count < 0) {
            throw InvalidArgument("Count cannot be negative");
        }

        std::vector<int> items(count);

        for (int index = 0; index < count; ++index) {
            items[index] = ReadInt("Element " + std::to_string(index) + ": ");
        }

        std::unique_ptr<Sequence<int>> sequence;
        int* rawItems = count == 0 ? nullptr : items.data();

        if (type == 1) {
            sequence.reset(new ArraySequence<int>(rawItems, count));
        }
        else if (type == 2) {
            sequence.reset(new ListSequence<int>(rawItems, count));
        }
        else {
            throw InvalidArgument("Unknown sequence type");
        }

        return sequence;
    }

    void PrintMenu()
    {
        std::cout << "\nSequence menu:\n";
        std::cout << "1. Create array sequence\n";
        std::cout << "2. Create list sequence\n";
        std::cout << "4. Print current sequence\n";
        std::cout << "5. Append value\n";
        std::cout << "6. Prepend value\n";
        std::cout << "7. Insert value by index\n";
        std::cout << "8. Get value by index\n";
        std::cout << "9. Get first value\n";
        std::cout << "10. Get last value\n";
        std::cout << "11. Get subsequence\n";
        std::cout << "12. Concat another sequence\n";
        std::cout << "0. Exit\n";
    }
}

int main()
{
    std::unique_ptr<Sequence<int>> current;

    while (true) {
        try {
            PrintMenu();
            int action = ReadInt("Choose action: ");

            if (action == 0) {
                return 0;
            }

            if (action >= 1 && action <= 2) {
                current = CreateSequence(action);
                std::cout << "Sequence created: ";
                PrintSequence(current.get());
                continue;
            }

            if (current == nullptr) {
                std::cout << "Create a sequence first.\n";
                continue;
            }

            if (action == 4) {
                PrintSequence(current.get());
            }
            else if (action == 5) {
                int value = ReadInt("Value: ");
                current->Append(value);
                PrintSequence(current.get());
            }
            else if (action == 6) {
                int value = ReadInt("Value: ");
                current->Prepend(value);
                PrintSequence(current.get());
            }
            else if (action == 7) {
                int value = ReadInt("Value: ");
                int index = ReadInt("Index: ");
                current->InsertAt(value, index);
                PrintSequence(current.get());
            }
            else if (action == 8) {
                int index = ReadInt("Index: ");
                std::cout << "Value: " << current->Get(index) << "\n";
            }
            else if (action == 9) {
                std::cout << "First: " << current->GetFirst() << "\n";
            }
            else if (action == 10) {
                std::cout << "Last: " << current->GetLast() << "\n";
            }
            else if (action == 11) {
                int startIndex = ReadInt("Start index: ");
                int endIndex = ReadInt("End index: ");
                current.reset(current->GetSubsequence(startIndex, endIndex));
                std::cout << "Current subsequence: ";
                PrintSequence(current.get());
            }
            else if (action == 12) {
                int type = ReadInt("Other sequence type (1-array, 2-list): ");
                std::unique_ptr<Sequence<int>> other = CreateSequence(type);
                current->Concat(other.get());
                PrintSequence(current.get());
            }
            else {
                std::cout << "Unknown action.\n";
            }
        }
        catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
            ClearInput();
        }
    }
}
