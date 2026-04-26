#include <gtest/gtest.h>

#include "BitSequence.hpp"
#include "Exceptions.hpp"

inline int InvertBit(int x) {
    if (x == 0) {
        return 1;
    } else {
        return 0;
    }
}

inline bool IsBitSet(int x) {
    return x != 0;
}

inline int SumBits(int acc, int current) {
    return acc + current;
}

TEST(BitSequenceTests, DefaultConstructorCreatesEmptySequence)
{
    BitSequence<int> sequence;

    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(BitSequenceTests, ItemsConstructorCopiesElementsAndCastsToBool)
{
    int items[] = {1, 0, 42, 0, -5}; 
    BitSequence<int> sequence(items, 5);

    EXPECT_EQ(sequence.GetLength(), 5);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 0);
    EXPECT_EQ(sequence.Get(2), 1);
    EXPECT_EQ(sequence.Get(3), 0); 
    EXPECT_EQ(sequence.Get(4), 1);
}

TEST(BitSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 0, 1};
    BitSequence<int> original(items, 3);

    BitSequence<int> copy(original);
    copy.InsertAt(0, 1);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(1), 0);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(1), 0);
}

TEST(BitSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 1, 0};
    BitSequence<int> source(items, 3);
    BitSequence<int> target;

    target = source;
    target.Append(1);

    EXPECT_EQ(source.GetLength(), 3);
    EXPECT_EQ(target.GetLength(), 4);
    EXPECT_EQ(target.GetLast(), 1);
}

TEST(BitSequenceTests, AppendWorksAcrossByteBoundaries)
{
    BitSequence<int> sequence;

    for (int i = 0; i < 20; ++i) {
        sequence.Append(i % 2);
    }

    EXPECT_EQ(sequence.GetLength(), 20);
    EXPECT_EQ(sequence.Get(0), 0);
    EXPECT_EQ(sequence.Get(1), 1);
    EXPECT_EQ(sequence.Get(8), 0);
    EXPECT_EQ(sequence.Get(19), 1);
}

TEST(BitSequenceTests, PrependAddsElementsToBeginningAndShiftsProperly)
{
    BitSequence<int> sequence;

    sequence.Prepend(1)->Prepend(0)->Prepend(1);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.GetFirst(), 1);
    EXPECT_EQ(sequence.Get(1), 0);
    EXPECT_EQ(sequence.GetLast(), 1);
}

TEST(BitSequenceTests, InsertAtPlacesElementCorrectlyWithoutDataLoss)
{
    int items[] = {1, 1, 1};
    BitSequence<int> sequence(items, 3);

    sequence.InsertAt(0, 1);

    EXPECT_EQ(sequence.GetLength(), 4);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 0);
    EXPECT_EQ(sequence.Get(2), 1);
    EXPECT_EQ(sequence.Get(3), 1);
}

TEST(BitSequenceTests, InsertAtRejectsInvalidIndices)
{
    BitSequence<int> sequence;

    EXPECT_THROW(sequence.InsertAt(1, -1), InvalidArgument);
    EXPECT_THROW(sequence.InsertAt(1, 1), OutOfRange);
}

TEST(BitSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 0};
    int rightItems[] = {0, 1};
    BitSequence<int> left(leftItems, 2);
    BitSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_EQ(left.GetLength(), 4);
    EXPECT_EQ(left.Get(0), 1);
    EXPECT_EQ(left.Get(1), 0);
    EXPECT_EQ(left.Get(2), 0);
    EXPECT_EQ(left.Get(3), 1);
}

TEST(BitSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 0, 1, 1, 0};
    BitSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_EQ(subSeq->GetLength(), 3);
    EXPECT_EQ(subSeq->Get(0), 0);
    EXPECT_EQ(subSeq->Get(1), 1);
    EXPECT_EQ(subSeq->Get(2), 1);

    delete subSeq;
}

TEST(BitSequenceTests, MapTransformsElements)
{
    int items[] = {1, 0, 1};
    BitSequence<int> sequence(items, 3);

    Sequence<int>* mapped = sequence.Map(InvertBit);

    EXPECT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->Get(0), 0);
    EXPECT_EQ(mapped->Get(1), 1);
    EXPECT_EQ(mapped->Get(2), 0);

    delete mapped;
}

TEST(BitSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 0, 1, 0, 1};
    BitSequence<int> sequence(items, 5);

    Sequence<int>* filtered = sequence.Where(IsBitSet);

    EXPECT_EQ(filtered->GetLength(), 3);
    EXPECT_EQ(filtered->Get(0), 1);
    EXPECT_EQ(filtered->Get(1), 1);
    EXPECT_EQ(filtered->Get(2), 1);

    delete filtered;
}

TEST(BitSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 0, 1, 1};
    BitSequence<int> sequence(items, 4);

    int sum = sequence.Reduce(SumBits);

    EXPECT_EQ(sum, 3);
}

TEST(BitSequenceTests, NOTInvertsAllBits)
{
    int items[] = {1, 0, 1, 0, 0};
    BitSequence<int> sequence(items, 5);

    BitSequence<int>* result = sequence.NOT();

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 0);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 0);
    EXPECT_EQ(result->Get(3), 1);
    EXPECT_EQ(result->Get(4), 1);

    delete result;
}

TEST(BitSequenceTests, ANDPerformsLogicalAndWithDifferentLengths)
{
    int items1[] = {1, 0, 1};
    int items2[] = {1, 1, 0, 1, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 5);

    BitSequence<int>* result = seq1.AND(&seq2);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 0);
    EXPECT_EQ(result->Get(2), 0);
    EXPECT_EQ(result->Get(3), 0);
    EXPECT_EQ(result->Get(4), 0);

    delete result;
}

TEST(BitSequenceTests, ORPerformsLogicalOrWithDifferentLengths)
{
    int items1[] = {1, 0, 0};
    int items2[] = {0, 1, 0, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 4);

    BitSequence<int>* result = seq1.OR(&seq2);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 0);
    EXPECT_EQ(result->Get(3), 1);

    delete result;
}

TEST(BitSequenceTests, XORPerformsLogicalXorWithDifferentLengths)
{
    int items1[] = {1, 1, 0};
    int items2[] = {1, 0, 1, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 4);

    BitSequence<int>* result = seq1.XOR(&seq2);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 0);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 1);
    EXPECT_EQ(result->Get(3), 1);

    delete result;
}