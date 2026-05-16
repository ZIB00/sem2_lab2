#include <gtest/gtest.h>
#include <functional>

#include "BitSequence.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"
#include "SequenceUtils.hpp"

int InvertBit(int x) {
    if (x == 0) {
        return 1;
    } else {
        return 0;
    }
}

bool IsBitSet(int x) {
    return x != 0;
}

int SumBits(int acc, int current) {
    return acc + current;
}

TEST(BitSequenceTests, DefaultConstructorCreatesEmptySequence)
{
    BitSequence<int> sequence;

    EXPECT_TRUE(TestUtils::CheckSequence({}, &sequence, "Default empty BitSequence"));
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(BitSequenceTests, ItemsConstructorCopiesElementsAndCastsToBool)
{
    int items[] = {1, 0, 42, 0, -5}; 
    BitSequence<int> sequence(items, 5);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 1, 0, 1}, &sequence, "Items constructor with non-boolean ints"));
}

TEST(BitSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 0, 1};
    BitSequence<int> original(items, 3);

    BitSequence<int> copy(original);
    copy.InsertAt(0, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 1}, &original, "Original sequence remains unchanged"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 0, 1}, &copy, "Copied sequence after InsertAt(0, 1)"));
}

TEST(BitSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 1, 0};
    BitSequence<int> source(items, 3);
    BitSequence<int> target;

    target = source;
    target.Append(1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 1, 0}, &source, "Source sequence remains unchanged"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 1, 0, 1}, &target, "Target sequence after Append(1)"));
}

TEST(BitSequenceTests, AppendWorksAcrossByteBoundaries)
{
    BitSequence<int> sequence;

    for (int i = 0; i < 20; ++i) {
        sequence.Append(i % 2);
    }

    EXPECT_TRUE(TestUtils::CheckSequence(
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, 
        &sequence, 
        "Appending 20 alternating bits across byte boundaries"
    ));
}

TEST(BitSequenceTests, PrependAddsElementsToBeginningAndShiftsProperly)
{
    BitSequence<int> sequence;

    sequence.Prepend(1)->Prepend(0)->Prepend(1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 1}, &sequence, "Chained Prepend: 1, then 0, then 1"));
}

TEST(BitSequenceTests, InsertAtPlacesElementCorrectlyWithoutDataLoss)
{
    int items[] = {1, 1, 1};
    BitSequence<int> sequence(items, 3);

    sequence.InsertAt(0, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 1, 1}, &sequence, "InsertAt(0, 1) into {1, 1, 1}"));
}

TEST(BitSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 0};
    int rightItems[] = {0, 1};
    BitSequence<int> left(leftItems, 2);
    BitSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 0, 1}, &left, "Concat {0, 1} to {1, 0}"));
}

TEST(BitSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 0, 1, 1, 0};
    BitSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 1}, subSeq, "GetSubsequence from index 1, length 3"));

    delete subSeq;
}

TEST(BitSequenceTests, MapTransformsElements)
{
    int items[] = {1, 0, 1};
    BitSequence<int> sequence(items, 3);

    Sequence<int>* mapped = SequenceUtils::Map<int>(&sequence, std::function<int(int)>(InvertBit));

    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 0}, mapped, "Map using InvertBit"));

    delete mapped;
}

TEST(BitSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 0, 1, 0, 1};
    BitSequence<int> sequence(items, 5);

    Sequence<int>* filtered = SequenceUtils::Where<int>(&sequence, std::function<bool(int)>(IsBitSet));

    EXPECT_TRUE(TestUtils::CheckSequence({1, 1, 1}, filtered, "Where filtering only set bits"));

    delete filtered;
}

TEST(BitSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 0, 1, 1};
    BitSequence<int> sequence(items, 4);

    int sum = SequenceUtils::Reduce<int, int>(&sequence, std::function<int(int, int)>(SumBits));

    EXPECT_EQ(sum, 3);
}

TEST(BitSequenceTests, NOTInvertsAllBits)
{
    int items[] = {1, 0, 1, 0, 0};
    BitSequence<int> sequence(items, 5);

    BitSequence<int>* result = sequence.NOT();

    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 0, 1, 1}, result, "NOT operation on {1, 0, 1, 0, 0}"));

    delete result;
}

TEST(BitSequenceTests, ANDPerformsLogicalAndWithDifferentLengths)
{
    int items1[] = {1, 0, 1};
    int items2[] = {1, 1, 0, 1, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 5);

    BitSequence<int>* result = seq1.AND(&seq2);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 0, 0, 0}, result, "AND with different lengths (3 and 5)"));

    delete result;
}

TEST(BitSequenceTests, ORPerformsLogicalOrWithDifferentLengths)
{
    int items1[] = {1, 0, 0};
    int items2[] = {0, 1, 0, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 4);

    BitSequence<int>* result = seq1.OR(&seq2);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 1, 0, 1}, result, "OR with different lengths (3 and 4)"));

    delete result;
}

TEST(BitSequenceTests, XORPerformsLogicalXorWithDifferentLengths)
{
    int items1[] = {1, 1, 0};
    int items2[] = {1, 0, 1, 1};
    BitSequence<int> seq1(items1, 3);
    BitSequence<int> seq2(items2, 4);

    BitSequence<int>* result = seq1.XOR(&seq2);

    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 1, 1}, result, "XOR with different lengths (3 and 4)"));

    delete result;
}