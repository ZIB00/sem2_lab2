#include <gtest/gtest.h>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"

TEST(MutableArraySequenceTests, DefaultConstructorCreatesEmptySequence)
{
    MutableArraySequence<int> sequence;

    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(MutableArraySequenceTests, AppendPrependAndInsertAtModifyMutableSequence)
{
    int items[] = {2, 3};
    MutableArraySequence<int> sequence(items, 2);

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

TEST(MutableArraySequenceTests, GetSubsequenceReturnsInclusiveCopy)
{
    int items[] = {1, 2, 3, 4};
    MutableArraySequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);

    EXPECT_EQ(subsequence->GetLength(), 2);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(subsequence->Get(1), 3);

    delete subsequence;
}

TEST(MutableArraySequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    MutableArraySequence<int> left(leftItems, 2);
    MutableListSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(0), 1);
    EXPECT_EQ(left.Get(1), 2);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(MutableArraySequenceTests, InsertAtRejectsInvalidIndex)
{
    MutableArraySequence<int> sequence;

    EXPECT_THROW(sequence.InsertAt(1, -1), InvalidArgument);
    EXPECT_THROW(sequence.InsertAt(1, 1), OutOfRange);
}

TEST(MutableArraySequenceTests, GetSubsequenceRejectsInvalidRange)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(-1, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(1, 0), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(0, 3), OutOfRange);
}

TEST(MutableArraySequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> original(items, 3);

    MutableArraySequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 99);
}

TEST(ImmutableArraySequenceTests, AppendReturnsNewSequence)
{
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> original(items, 3);

    Sequence<int>* changed = original.Append(4);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->GetLast(), 4);

    delete changed;
}

TEST(ImmutableArraySequenceTests, InsertAtKeepsOriginalUnchanged)
{
    int items[] = {1, 3};
    ImmutableArraySequence<int> original(items, 2);

    Sequence<int>* changed = original.InsertAt(2, 1);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(original.Get(1), 3);
    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->Get(1), 2);

    delete changed;
}

TEST(ImmutableArraySequenceTests, ConcatReturnsNewSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ImmutableArraySequence<int> left(leftItems, 2);
    MutableListSequence<int> right(rightItems, 2);

    Sequence<int>* changed = left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 2);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->Get(2), 3);
    EXPECT_EQ(changed->Get(3), 4);

    delete changed;
}

TEST(ImmutableArraySequenceTests, GetSubsequenceReturnsImmutableSequence)
{
    int items[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);
    Sequence<int>* changed = subsequence->Append(9);

    EXPECT_EQ(subsequence->GetLength(), 2);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->Get(2), 9);

    delete subsequence;
    delete changed;
}

TEST(MutableListSequenceTests, BasicOperationsWork)
{
    MutableListSequence<int> sequence;

    sequence.Append(2)->Prepend(1)->InsertAt(3, 2);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetFirst(), 1);
    EXPECT_EQ(sequence.GetLast(), 3);
}

TEST(MutableListSequenceTests, GetSubsequenceReturnsListSequenceCopy)
{
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 3);

    EXPECT_EQ(subsequence->GetLength(), 3);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(subsequence->Get(2), 4);

    delete subsequence;
}

TEST(MutableListSequenceTests, ConcatAppendsElementsFromAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    MutableListSequence<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(MutableListSequenceTests, CopyConstructorCreatesIndependentCopy)
{
    int items[] = {1, 2};
    MutableListSequence<int> original(items, 2);

    MutableListSequence<int> copy(original);
    copy.Append(3);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(copy.GetLength(), 3);
}

TEST(ImmutableListSequenceTests, AppendReturnsNewSequence)
{
    int items[] = {1, 2};
    ImmutableListSequence<int> original(items, 2);

    Sequence<int>* changed = original.Append(3);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->GetLast(), 3);

    delete changed;
}

TEST(ImmutableListSequenceTests, InsertAtKeepsOriginalUnchanged)
{
    int items[] = {1, 3};
    ImmutableListSequence<int> original(items, 2);

    Sequence<int>* changed = original.InsertAt(2, 1);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(original.Get(1), 3);
    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->Get(1), 2);

    delete changed;
}

TEST(ImmutableListSequenceTests, ConcatReturnsNewSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ImmutableListSequence<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    Sequence<int>* changed = left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 2);
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->Get(2), 3);
    EXPECT_EQ(changed->Get(3), 4);

    delete changed;
}

TEST(ImmutableListSequenceTests, GetSubsequenceReturnsImmutableSequence)
{
    int items[] = {1, 2, 3, 4};
    ImmutableListSequence<int> sequence(items, 4);

    Sequence<int>* subsequence = sequence.GetSubsequence(1, 2);
    Sequence<int>* changed = subsequence->Append(9);

    EXPECT_EQ(subsequence->GetLength(), 2);
    EXPECT_EQ(subsequence->Get(0), 2);
    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->Get(2), 9);

    delete subsequence;
    delete changed;
}
