#include <gtest/gtest.h>

#include <string>

#include "LinkedList.hpp"

TEST(LinkedListTests, DefaultConstructorCreatesEmptyList)
{
    LinkedList<int> list;

    EXPECT_EQ(list.GetLength(), 0);
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

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, ItemsConstructorMakesIndependentCopy)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);
    items[0] = 100;

    EXPECT_EQ(list.Get(0), 1);
}

TEST(LinkedListTests, ItemsConstructorAllowsZeroCountWithNullItems)
{
    LinkedList<int> list(nullptr, 0);

    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListTests, ItemsConstructorAllowsZeroCountWithNonNullItems)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 0);

    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListTests, ItemsConstructorRejectsNegativeCount)
{
    int items[] = {1, 2, 3};

    EXPECT_THROW(LinkedList<int> list(items, -1), InvalidArgument);
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

    EXPECT_EQ(list.Get(0), 10);
    EXPECT_EQ(list.Get(1), 20);
    EXPECT_EQ(list.Get(2), 30);
}

TEST(LinkedListTests, GetRejectsNegativeIndex)
{
    LinkedList<int> list;

    EXPECT_THROW(list.Get(-1), InvalidArgument);
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

    EXPECT_EQ(list.GetLength(), 1);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 10);
}

TEST(LinkedListTests, AppendAddsElementToEnd)
{
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    list.Append(3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedListTests, PrependAddsElementToEmptyList)
{
    LinkedList<int> list;

    list.Prepend(10);

    EXPECT_EQ(list.GetLength(), 1);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 10);
}

TEST(LinkedListTests, PrependAddsElementToBeginning)
{
    int items[] = {2, 3};
    LinkedList<int> list(items, 2);

    list.Prepend(1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.GetFirst(), 1);
}

TEST(LinkedListTests, InsertAtZeroPrepends)
{
    int items[] = {2, 3};
    LinkedList<int> list(items, 2);

    list.InsertAt(1, 0);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, InsertAtMiddleInsertsElement)
{
    int items[] = {1, 3};
    LinkedList<int> list(items, 2);

    list.InsertAt(2, 1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, InsertAtLengthAppends)
{
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    list.InsertAt(3, 2);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedListTests, InsertAtRejectsNegativeIndex)
{
    LinkedList<int> list;

    EXPECT_THROW(list.InsertAt(10, -1), InvalidArgument);
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

    EXPECT_EQ(subList->GetLength(), 3);
    EXPECT_EQ(subList->Get(0), 2);
    EXPECT_EQ(subList->Get(1), 3);
    EXPECT_EQ(subList->Get(2), 4);

    delete subList;
}

TEST(LinkedListTests, GetSubListCanReturnSingleElement)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(1, 1);

    EXPECT_EQ(subList->GetLength(), 1);
    EXPECT_EQ(subList->Get(0), 2);

    delete subList;
}

TEST(LinkedListTests, GetSubListCanReturnWholeList)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(0, 2);

    EXPECT_EQ(subList->GetLength(), 3);
    EXPECT_EQ(subList->Get(0), 1);
    EXPECT_EQ(subList->Get(1), 2);
    EXPECT_EQ(subList->Get(2), 3);

    delete subList;
}

TEST(LinkedListTests, GetSubListReturnsIndependentList)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int>* subList = list.GetSubList(0, 1);
    subList->InsertAt(100, 0);

    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(subList->Get(0), 100);

    delete subList;
}

TEST(LinkedListTests, GetSubListRejectsNegativeStartIndex)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(-1, 1), InvalidArgument);
}

TEST(LinkedListTests, GetSubListRejectsNegativeEndIndex)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(0, -1), InvalidArgument);
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

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);

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

    EXPECT_EQ(first.GetLength(), 2);
    EXPECT_EQ(second.GetLength(), 2);
    EXPECT_EQ(first.Get(0), 1);
    EXPECT_EQ(second.Get(0), 3);

    delete result;
}

TEST(LinkedListTests, ConcatWithEmptyLeftListWorks)
{
    LinkedList<int> first;
    int secondItems[] = {1, 2};
    LinkedList<int> second(secondItems, 2);

    LinkedList<int>* result = first.Concat(&second);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);

    delete result;
}

TEST(LinkedListTests, ConcatWithEmptyRightListWorks)
{
    int firstItems[] = {1, 2};
    LinkedList<int> first(firstItems, 2);
    LinkedList<int> second;

    LinkedList<int>* result = first.Concat(&second);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);

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

    EXPECT_EQ(copy.GetLength(), 3);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
    EXPECT_EQ(copy.Get(2), 3);
}

TEST(LinkedListTests, CopyConstructorCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    LinkedList<int> original(items, 3);

    LinkedList<int> copy(original);
    copy.InsertAt(100, 0);

    EXPECT_EQ(original.Get(0), 1);
    EXPECT_EQ(copy.Get(0), 100);
}

TEST(LinkedListTests, CopyConstructorCopiesEmptyList)
{
    LinkedList<int> original;

    LinkedList<int> copy(original);

    EXPECT_EQ(copy.GetLength(), 0);
    EXPECT_THROW(copy.GetFirst(), OutOfRange);
}

TEST(LinkedListTests, AssignmentCopiesElements)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};
    LinkedList<int> source(sourceItems, 3);
    LinkedList<int> target(targetItems, 2);

    target = source;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);
}

TEST(LinkedListTests, AssignmentCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source(items, 3);
    LinkedList<int> target;

    target = source;
    target.InsertAt(100, 0);

    EXPECT_EQ(source.Get(0), 1);
    EXPECT_EQ(target.Get(0), 100);
}

TEST(LinkedListTests, AssignmentFromEmptyListClearsTarget)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source;
    LinkedList<int> target(items, 3);

    target = source;

    EXPECT_EQ(target.GetLength(), 0);
    EXPECT_THROW(target.GetFirst(), OutOfRange);
}

TEST(LinkedListTests, AssignmentToEmptyListCopiesSource)
{
    int items[] = {1, 2, 3};
    LinkedList<int> source(items, 3);
    LinkedList<int> target;

    target = source;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);
}

TEST(LinkedListTests, SelfAssignmentKeepsListValid)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    list = list;

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, WorksWithStringValues)
{
    LinkedList<std::string> list;

    list.Append("second");
    list.Prepend("first");
    list.Append("third");

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), "first");
    EXPECT_EQ(list.Get(1), "second");
    EXPECT_EQ(list.Get(2), "third");
}
