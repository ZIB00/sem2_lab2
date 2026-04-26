#include <gtest/gtest.h>

#include "SegmentedList.hpp"
#include "ArraySequence.hpp"
#include "Exceptions.hpp"

namespace SegmentedListTests
{
    int MultiplyByTwo(int x) {
        return x * 2;
    }

    bool IsEven(int x) {
        return x % 2 == 0;
    }

    int SumValues(int acc, int current) {
        return acc + current;
    }
}

TEST(SegmentedListTests, DefaultConstructorCreatesEmptySequence)
{
    SegmentedList<int> list;

    EXPECT_EQ(list.GetLength(), 0);
    EXPECT_THROW(list.GetFirst(), OutOfRange);
    EXPECT_THROW(list.GetLast(), OutOfRange);
}

TEST(SegmentedListTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> list(items, 3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(SegmentedListTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> original(items, 3);

    SegmentedList<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 99);
}

TEST(SegmentedListTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> source(items, 3);
    SegmentedList<int> target;

    target = source;
    target.Append(4);

    EXPECT_EQ(source.GetLength(), 3);
    EXPECT_EQ(target.GetLength(), 4);
    EXPECT_EQ(target.GetLast(), 4);
}

TEST(SegmentedListTests, AppendAddsElementsToEnd)
{
    SegmentedList<int> list;

    list.Append(1)->Append(2)->Append(3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetLast(), 3);
    EXPECT_EQ(list.Get(1), 2);
}

TEST(SegmentedListTests, PrependAddsElementsToBeginning)
{
    SegmentedList<int> list;

    list.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(SegmentedListTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    SegmentedList<int> list(items, 2);

    list.InsertAt(2, 1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(1), 2);
}

TEST(SegmentedListTests, InsertAtRejectsInvalidIndices)
{
    SegmentedList<int> list;

    EXPECT_THROW(list.InsertAt(10, -1), InvalidArgument);
    EXPECT_THROW(list.InsertAt(10, 1), OutOfRange);
}

TEST(SegmentedListTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    SegmentedList<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(SegmentedListTests, GetRejectsInvalidIndices)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> list(items, 3);

    EXPECT_THROW(list.Get(-1), InvalidArgument);
    EXPECT_THROW(list.Get(3), OutOfRange);
}

TEST(SegmentedListTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    SegmentedList<int> list(items, 5);

    Sequence<int>* subSeq = list.GetSubsequence(1, 3);

    EXPECT_EQ(subSeq->GetLength(), 3);
    EXPECT_EQ(subSeq->Get(0), 2);
    EXPECT_EQ(subSeq->Get(2), 4);

    delete subSeq;
}

TEST(SegmentedListTests, GetSubsequenceRejectsInvalidRange)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubsequence(-1, 1), InvalidArgument);
    EXPECT_THROW(list.GetSubsequence(2, 1), InvalidArgument);
    EXPECT_THROW(list.GetSubsequence(0, 5), OutOfRange);
}

TEST(SegmentedListTests, MapTransformsElements)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> list(items, 3);

    Sequence<int>* mapped = list.Map(SegmentedListTests::MultiplyByTwo);

    EXPECT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 6);

    delete mapped;
}

TEST(SegmentedListTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    SegmentedList<int> list(items, 5);

    Sequence<int>* filtered = list.Where(SegmentedListTests::IsEven);

    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    delete filtered;
}

TEST(SegmentedListTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    SegmentedList<int> list(items, 4);

    int sum = list.Reduce(SegmentedListTests::SumValues);

    EXPECT_EQ(sum, 10);
}

TEST(SegmentedListTests, AppendWorksAcrossMultipleSegments)
{
    SegmentedList<int> list;

    for (int i = 0; i < 25; ++i) {
        list.Append(i);
    }

    EXPECT_EQ(list.GetLength(), 25);
    EXPECT_EQ(list.GetFirst(), 0);
    EXPECT_EQ(list.GetLast(), 24);
    EXPECT_EQ(list.Get(10), 10);
    EXPECT_EQ(list.Get(24), 24);
}

TEST(SegmentedListTests, PrependWorksAcrossMultipleSegments)
{
    SegmentedList<int> list;

    for (int i = 0; i < 25; ++i) {
        list.Prepend(i);
    }

    EXPECT_EQ(list.GetLength(), 25);
    EXPECT_EQ(list.GetFirst(), 24);
    EXPECT_EQ(list.GetLast(), 0);
    EXPECT_EQ(list.Get(10), 14);
}