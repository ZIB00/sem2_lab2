#include <gtest/gtest.h>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"

TEST(ArraySequenceTests, DefaultConstructorCreatesEmptySequence)
{
    ArraySequence<int> sequence;

    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(ArraySequenceTests, AppendPrependAndInsertAtModifyMutableSequence)
{
    int items[] = {2, 3};
    ArraySequence<int> sequence(items, 2);

    EXPECT_EQ(sequence.Append(4), &sequence);
    EXPECT_EQ(sequence.Prepend(1), &sequence);
    EXPECT_EQ(sequence.InsertAt(99, 2), &sequence);

    EXPECT_EQ(sequence.GetLength(), 5);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 99);
    EXPECT_EQ(sequence.Get(3), 3);
    EXPECT_EQ(sequence.Get(4), 4);
}

TEST(ArraySequenceTests, GetSubsequenceReturnsInclusiveCopy)
{
    int items[] = {1, 2, 3, 4};
    ArraySequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);

    EXPECT_EQ(subsequence->GetLength(), 2);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(subsequence->Get(1), 3);

    delete subsequence;
}

TEST(ArraySequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ArraySequence<int> left(leftItems, 2);
    ListSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(0), 1);
    EXPECT_EQ(left.Get(1), 2);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(ArraySequenceTests, InsertAtRejectsInvalidIndex)
{
    ArraySequence<int> sequence;

    EXPECT_THROW(sequence.InsertAt(1, -1), InvalidArgument);
    EXPECT_THROW(sequence.InsertAt(1, 1), OutOfRange);
}

TEST(ArraySequenceTests, GetSubsequenceRejectsInvalidRange)
{
    int items[] = {1, 2, 3};
    ArraySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(-1, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(1, 0), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(0, 3), OutOfRange);
}

TEST(ArraySequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    ArraySequence<int> original(items, 3);

    ArraySequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 99);
}

TEST(ListSequenceTests, BasicOperationsWork)
{
    ListSequence<int> sequence;

    sequence.Append(2)->Prepend(1)->InsertAt(3, 2);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetFirst(), 1);
    EXPECT_EQ(sequence.GetLast(), 3);
}

TEST(ListSequenceTests, GetSubsequenceReturnsListSequenceCopy)
{
    int items[] = {1, 2, 3, 4};
    ListSequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 3);

    EXPECT_EQ(subsequence->GetLength(), 3);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(subsequence->Get(2), 4);

    delete subsequence;
}

TEST(ListSequenceTests, ConcatAppendsElementsFromAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ListSequence<int> left(leftItems, 2);
    ArraySequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(ListSequenceTests, CopyConstructorCreatesIndependentCopy)
{
    int items[] = {1, 2};
    ListSequence<int> original(items, 2);

    ListSequence<int> copy(original);
    copy.Append(3);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(copy.GetLength(), 3);
}
