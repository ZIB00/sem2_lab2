#include <gtest/gtest.h>

#include "ListSequence.hpp"
#include "ArraySequence.hpp"
#include "Exceptions.hpp"

namespace ListSequenceTests
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

TEST(MutableListSequenceTests, DefaultConstructorCreatesEmptySequence)
{
    MutableListSequence<int> sequence;

    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(MutableListSequenceTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> sequence(items, 3);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);
}

TEST(MutableListSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> original(items, 3);

    MutableListSequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 99);
}

TEST(MutableListSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> source(items, 3);
    MutableListSequence<int> target;

    target = source;
    target.Append(4);

    EXPECT_EQ(source.GetLength(), 3);
    EXPECT_EQ(target.GetLength(), 4);
    EXPECT_EQ(target.GetLast(), 4);
}

TEST(MutableListSequenceTests, AppendAddsElementsToEnd)
{
    MutableListSequence<int> sequence;

    sequence.Append(1)->Append(2)->Append(3);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetLast(), 3);
    EXPECT_EQ(sequence.Get(1), 2);
}

TEST(MutableListSequenceTests, PrependAddsElementsToBeginning)
{
    MutableListSequence<int> sequence;

    sequence.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetFirst(), 1);
    EXPECT_EQ(sequence.Get(2), 3);
}

TEST(MutableListSequenceTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    MutableListSequence<int> sequence(items, 2);

    sequence.InsertAt(2, 1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(1), 2);
}

TEST(MutableListSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    MutableListSequence<int> left(leftItems, 2);
    MutableListSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(2), 3);
    EXPECT_EQ(left.Get(3), 4);
}

TEST(MutableListSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_EQ(subSeq->GetLength(), 3);
    EXPECT_EQ(subSeq->Get(0), 2);
    EXPECT_EQ(subSeq->Get(2), 4);

    delete subSeq;
}

TEST(MutableListSequenceTests, GetSubsequenceRejectsInvalidRange)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(-1, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(2, 1), InvalidArgument);
    EXPECT_THROW(sequence.GetSubsequence(0, 5), OutOfRange);
}

TEST(MutableListSequenceTests, MapTransformsElements)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> sequence(items, 3);

    Sequence<int>* mapped = sequence.Map(ListSequenceTests::MultiplyByTwo);

    EXPECT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 6);

    delete mapped;
}

TEST(MutableListSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> sequence(items, 5);

    Sequence<int>* filtered = sequence.Where(ListSequenceTests::IsEven);

    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    delete filtered;
}

TEST(MutableListSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> sequence(items, 4);

    int sum = sequence.Reduce(ListSequenceTests::SumValues);

    EXPECT_EQ(sum, 10);
}

TEST(ImmutableListSequenceTests, AppendReturnsNewSequenceAndKeepsOriginal)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> original(items, 3);

    Sequence<int>* changed = original.Append(4);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetLast(), 3);
    
    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->GetLast(), 4);

    delete changed;
}

TEST(ImmutableListSequenceTests, PrependReturnsNewSequenceAndKeepsOriginal)
{
    int items[] = {2, 3};
    ImmutableListSequence<int> original(items, 2);

    Sequence<int>* changed = original.Prepend(1);

    EXPECT_EQ(original.GetLength(), 2);
    EXPECT_EQ(original.GetFirst(), 2);

    EXPECT_EQ(changed->GetLength(), 3);
    EXPECT_EQ(changed->GetFirst(), 1);

    delete changed;
}

TEST(ImmutableListSequenceTests, InsertAtReturnsNewSequenceAndKeepsOriginal)
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

TEST(ImmutableListSequenceTests, ConcatReturnsNewSequenceAndKeepsOriginal)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ImmutableListSequence<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    Sequence<int>* changed = left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 2);
    EXPECT_EQ(left.GetLast(), 2);

    EXPECT_EQ(changed->GetLength(), 4);
    EXPECT_EQ(changed->Get(2), 3);
    EXPECT_EQ(changed->Get(3), 4);

    delete changed;
}

TEST(ImmutableListSequenceTests, MapReturnsNewSequenceWithoutModifyingOriginal)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> original(items, 3);

    Sequence<int>* mapped = original.Map(ListSequenceTests::MultiplyByTwo);

    EXPECT_EQ(original.Get(0), 1);
    EXPECT_EQ(original.Get(2), 3);

    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(2), 6);

    delete mapped;
}

TEST(ImmutableListSequenceTests, WhereReturnsNewSequenceWithoutModifyingOriginal)
{
    int items[] = {1, 2, 3, 4};
    ImmutableListSequence<int> original(items, 4);

    Sequence<int>* filtered = original.Where(ListSequenceTests::IsEven);

    EXPECT_EQ(original.GetLength(), 4);
    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    delete filtered;
}