#include <gtest/gtest.h>

#include "AdaptiveSequence.hpp"
#include "Exceptions.hpp"

namespace AdaptiveSequenceTests
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

TEST(AdaptiveSequenceTests, DefaultConstructorCreatesEmptySequence)
{
    AdaptiveSequence<int> sequence;

    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(AdaptiveSequenceTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> sequence(items, 3);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);
}

TEST(AdaptiveSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> original(items, 3);

    AdaptiveSequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 99);
}

TEST(AdaptiveSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> source(items, 3);
    AdaptiveSequence<int> target;

    target = source;
    target.Append(4);

    EXPECT_EQ(source.GetLength(), 3);
    EXPECT_EQ(target.GetLength(), 4);
    EXPECT_EQ(target.GetLast(), 4);
}

TEST(AdaptiveSequenceTests, AppendAddsElementsToEnd)
{
    AdaptiveSequence<int> sequence;

    sequence.Append(1)->Append(2)->Append(3);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetLast(), 3);
    EXPECT_EQ(sequence.Get(1), 2);
}

TEST(AdaptiveSequenceTests, PrependAddsElementsToBeginning)
{
    AdaptiveSequence<int> sequence;

    sequence.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetFirst(), 1);
    EXPECT_EQ(sequence.Get(2), 3);
}

TEST(AdaptiveSequenceTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    AdaptiveSequence<int> sequence(items, 2);

    sequence.InsertAt(2, 1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(1), 2);
}

TEST(AdaptiveSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    AdaptiveSequence<int> left(leftItems, 2);
    AdaptiveSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(AdaptiveSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    AdaptiveSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_EQ(subSeq->GetLength(), 3);
    EXPECT_EQ(subSeq->Get(0), 2);
    EXPECT_EQ(subSeq->Get(2), 4);

    delete subSeq;
}

TEST(AdaptiveSequenceTests, GetSubsequenceRejectsInvalidRange)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(-1, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(2, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(0, 5), OutOfRange);
}

TEST(AdaptiveSequenceTests, MapTransformsElements)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> sequence(items, 3);

    Sequence<int>* mapped = sequence.Map(AdaptiveSequenceTests::MultiplyByTwo);

    EXPECT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 6);

    delete mapped;
}

TEST(AdaptiveSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    AdaptiveSequence<int> sequence(items, 5);

    Sequence<int>* filtered = sequence.Where(AdaptiveSequenceTests::IsEven);

    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    delete filtered;
}

TEST(AdaptiveSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    AdaptiveSequence<int> sequence(items, 4);

    int sum = sequence.Reduce(AdaptiveSequenceTests::SumValues);

    EXPECT_EQ(sum, 10);
}

TEST(AdaptiveSequenceTests, MultipleInsertsTriggerListBackendWithoutDataLoss)
{
    AdaptiveSequence<int> sequence;
    
    for (int i = 0; i < 10; ++i) {
        sequence.Append(i);
    }

    EXPECT_EQ(sequence.GetLength(), 10);
    EXPECT_EQ(sequence.GetFirst(), 0);
    EXPECT_EQ(sequence.GetLast(), 9);
    EXPECT_EQ(sequence.Get(5), 5);
}

TEST(AdaptiveSequenceTests, MultipleGetsTriggerArrayBackendWithoutDataLoss)
{
    int items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    AdaptiveSequence<int> sequence(items, 10);

    for (int i = 0; i < 15; ++i) {
        sequence.Get(i % 10);
    }

    sequence.Append(10);
    
    EXPECT_EQ(sequence.GetLength(), 11);
    EXPECT_EQ(sequence.Get(5), 5);
    EXPECT_EQ(sequence.GetLast(), 10);
}