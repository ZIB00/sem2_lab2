#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "LinkedList.hpp"

namespace {
    template<class T>
    testing::AssertionResult CheckLinkedList(
        const std::vector<T>& expected, 
        LinkedList<T>& actual, 
        const std::string& contextAction = "") 
    {
        bool match = true;
        if (expected.size() != actual.GetLength()) {
            match = false;
        } else {
            for (size_t i = 0; i < expected.size(); ++i) {
                if (expected[i] != actual.Get(i)) {
                    match = false;
                    break;
                }
            }
        }

        if (match) return testing::AssertionSuccess();

        testing::AssertionResult failure = testing::AssertionFailure();
        if (!contextAction.empty()) failure << "ACTION PERFORMED: " << contextAction << "\n";
        failure << "LIST MISMATCH DETECTED!\n";
        
        failure << "EXPECTED: [";
        for (size_t i = 0; i < expected.size(); ++i) {
            failure << expected[i] << (i < expected.size() - 1 ? ", " : "");
        }
        failure << "] (length: " << expected.size() << ")\n";

        failure << "RECEIVED: [";
        for (size_t i = 0; i < actual.GetLength(); ++i) {
            failure << actual.Get(i) << (i < actual.GetLength() - 1 ? ", " : "");
        }
        failure << "] (length: " << actual.GetLength() << ")\n";

        return failure;
    }
}

TEST(LinkedListTests, DefaultConstructorCreatesEmptyList)
{
    LinkedList<int> list;

    EXPECT_TRUE(CheckLinkedList({}, list, "Default empty list"));
}

TEST(LinkedListTests, EmptyListRejectsGetFirst)
{
    LinkedList<int> list;

    EXPECT_THROW(list.GetFirst(), OutOfRange);
}

TEST(LinkedListTests, EmptyListRejectsGetLast)
{
    LinkedList<int> list;

    EXPECT_THROW(list.GetLast(), OutOfRange);
}

TEST(LinkedListTests, EmptyListRejectsGetZero)
{
    LinkedList<int> list;

    EXPECT_THROW(list.Get(0), OutOfRange);
}

TEST(LinkedListTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "Items constructor"));
}

TEST(LinkedListTests, ItemsConstructorMakesIndependentCopy)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);
    items[0] = 100;

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "List after external array modification"));
}

TEST(LinkedListTests, ItemsConstructorAllowsZeroCountWithNullItems)
{
    LinkedList<int> list(nullptr, 0);

    EXPECT_TRUE(CheckLinkedList({}, list, "Constructor with nullptr and zero size"));
}

TEST(LinkedListTests, ItemsConstructorAllowsZeroCountWithNonNullItems)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 0);

    EXPECT_TRUE(CheckLinkedList({}, list, "Constructor with items and zero size"));
}

TEST(LinkedListTests, ItemsConstructorRejectsNullItemsWhenCountIsPositive)
{
    EXPECT_THROW(LinkedList<int> list(nullptr, 1), InvalidArgument);
}

TEST(LinkedListTests, GetFirstReturnsFirstElement)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_EQ(list.GetFirst(), 1);
}

TEST(LinkedListTests, GetLastReturnsLastElement)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedListTests, GetReturnsElementByIndex)
{
    int items[] = {10, 20, 30};
    LinkedList<int> list(items, 3);

    EXPECT_TRUE(CheckLinkedList({10, 20, 30}, list, "Get elements verification"));
}

TEST(LinkedListTests, GetRejectsIndexEqualToLength)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.Get(3), OutOfRange);
}

TEST(LinkedListTests, GetRejectsIndexGreaterThanLength)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.Get(10), OutOfRange);
}

TEST(LinkedListTests, AppendAddsElementToEmptyList)
{
    LinkedList<int> list;

    list.Append(10);

    EXPECT_TRUE(CheckLinkedList({10}, list, "Append to empty list"));
}

TEST(LinkedListTests, AppendAddsElementToEnd)
{
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    list.Append(3);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "Append 3"));
}

TEST(LinkedListTests, PrependAddsElementToEmptyList)
{
    LinkedList<int> list;

    list.Prepend(10);

    EXPECT_TRUE(CheckLinkedList({10}, list, "Prepend to empty list"));
}

TEST(LinkedListTests, PrependAddsElementToBeginning)
{
    int items[] = {2, 3};
    LinkedList<int> list(items, 2);

    list.Prepend(1);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "Prepend 1"));
}

TEST(LinkedListTests, InsertAtZeroPrepends)
{
    int items[] = {2, 3};
    LinkedList<int> list(items, 2);

    list.InsertAt(1, 0);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "InsertAt index 0"));
}

TEST(LinkedListTests, InsertAtMiddleInsertsElement)
{
    int items[] = {1, 3};
    LinkedList<int> list(items, 2);

    list.InsertAt(2, 1);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "InsertAt index 1"));
}

TEST(LinkedListTests, InsertAtLengthAppends)
{
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    list.InsertAt(3, 2);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "InsertAt end length"));
}

TEST(LinkedListTests, InsertAtRejectsIndexGreaterThanLength)
{
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    EXPECT_THROW(list.InsertAt(3, 3), OutOfRange);
}

TEST(LinkedListTests, InsertAtRejectsIndexGreaterThanZeroInEmptyList)
{
    LinkedList<int> list;

    EXPECT_THROW(list.InsertAt(10, 1), OutOfRange);
}

TEST(LinkedListTests, GetSubListReturnsInclusiveRange)
{
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);

    LinkedList<int>* subList = list.GetSubList(1, 3);

    EXPECT_TRUE(CheckLinkedList({2, 3, 4}, *subList, "GetSubList(1, 3)"));

    delete subList;
}

TEST(LinkedListTests, GetSubListCanReturnSingleElement)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(1, 1);

    EXPECT_TRUE(CheckLinkedList({2}, *subList, "GetSubList(1, 1)"));

    delete subList;
}

TEST(LinkedListTests, GetSubListCanReturnWholeList)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(0, 2);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, *subList, "GetSubList(0, 2)"));

    delete subList;
}

TEST(LinkedListTests, GetSubListReturnsIndependentList)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(0, 1);
    subList->InsertAt(100, 0);

    EXPECT_EQ(list.Get(0), 1);
    EXPECT_TRUE(CheckLinkedList({100, 1, 2}, *subList, "Independent SubList modification"));

    delete subList;
}

TEST(LinkedListTests, GetSubListRejectsStartGreaterThanEnd)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(2, 1), InvalidArgument);
}

TEST(LinkedListTests, GetSubListRejectsEndIndexEqualToLength)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(0, 3), OutOfRange);
}

TEST(LinkedListTests, GetSubListRejectsIndexInEmptyList)
{
    LinkedList<int> list;

    EXPECT_THROW(list.GetSubList(0, 0), OutOfRange);
}

TEST(LinkedListTests, ConcatReturnsCombinedList)
{
    int firstItems[] = {1, 2};
    int secondItems[] = {3, 4};
    LinkedList<int> first(firstItems, 2);
    LinkedList<int> second(secondItems, 2);

    LinkedList<int>* result = first.Concat(&second);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3, 4}, *result, "Concat two populated lists"));

    delete result;
}

TEST(LinkedListTests, ConcatDoesNotChangeSourceLists)
{
    int firstItems[] = {1, 2};
    int secondItems[] = {3, 4};
    LinkedList<int> first(firstItems, 2);
    LinkedList<int> second(secondItems, 2);

    LinkedList<int>* result = first.Concat(&second);
    result->InsertAt(100, 0);

    EXPECT_TRUE(CheckLinkedList({1, 2}, first, "First list after Concat result modification"));
    EXPECT_TRUE(CheckLinkedList({3, 4}, second, "Second list after Concat result modification"));

    delete result;
}

TEST(LinkedListTests, ConcatWithEmptyLeftListWorks)
{
    LinkedList<int> first;
    int secondItems[] = {1, 2};
    LinkedList<int> second(secondItems, 2);

    LinkedList<int>* result = first.Concat(&second);

    EXPECT_TRUE(CheckLinkedList({1, 2}, *result, "Concat empty and {1, 2}"));

    delete result;
}

TEST(LinkedListTests, ConcatWithEmptyRightListWorks)
{
    int firstItems[] = {1, 2};
    LinkedList<int> first(firstItems, 2);
    LinkedList<int> second;

    LinkedList<int>* result = first.Concat(&second);

    EXPECT_TRUE(CheckLinkedList({1, 2}, *result, "Concat {1, 2} and empty"));

    delete result;
}

TEST(LinkedListTests, ConcatRejectsNullList)
{
    LinkedList<int> list;

    EXPECT_THROW(list.Concat(nullptr), InvalidArgument);
}

TEST(LinkedListTests, CopyConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    LinkedList<int> original(items, 3);

    LinkedList<int> copy(original);

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, copy, "Copy constructor"));
}

TEST(LinkedListTests, CopyConstructorCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    LinkedList<int> original(items, 3);

    LinkedList<int> copy(original);
    copy.InsertAt(100, 0);

    EXPECT_EQ(original.Get(0), 1);
    EXPECT_TRUE(CheckLinkedList({100, 1, 2, 3}, copy, "Copied list after modification"));
}

TEST(LinkedListTests, CopyConstructorCopiesEmptyList)
{
    LinkedList<int> original;

    LinkedList<int> copy(original);

    EXPECT_TRUE(CheckLinkedList({}, copy, "Copy empty list"));
    EXPECT_THROW(copy.GetFirst(), OutOfRange);
}

TEST(LinkedListTests, AssignmentCopiesElements)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};
    LinkedList<int> source(sourceItems, 3);
    LinkedList<int> target(targetItems, 2);

    target = source;

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, target, "Assignment operator"));
}

TEST(LinkedListTests, AssignmentCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source(items, 3);
    LinkedList<int> target;

    target = source;
    target.InsertAt(100, 0);

    EXPECT_EQ(source.Get(0), 1);
    EXPECT_TRUE(CheckLinkedList({100, 1, 2, 3}, target, "Assigned list after modification"));
}

TEST(LinkedListTests, AssignmentFromEmptyListClearsTarget)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source;
    LinkedList<int> target(items, 3);

    target = source;

    EXPECT_TRUE(CheckLinkedList({}, target, "Assignment from empty list"));
    EXPECT_THROW(target.GetFirst(), OutOfRange);
}

TEST(LinkedListTests, AssignmentToEmptyListCopiesSource)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source(items, 3);
    LinkedList<int> target;

    target = source;

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, target, "Assignment to empty list"));
}

TEST(LinkedListTests, SelfAssignmentKeepsListValid)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    list = list;

    EXPECT_TRUE(CheckLinkedList({1, 2, 3}, list, "Self-assignment"));
}

TEST(LinkedListTests, WorksWithStringValues)
{
    LinkedList<std::string> list;

    list.Append("second");
    list.Prepend("first");
    list.Append("third");

    EXPECT_TRUE(CheckLinkedList<std::string>({"first", "second", "third"}, list, "String list operations"));
}