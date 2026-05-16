#include <gtest/gtest.h>
#include <functional>

#include "ListSequence.hpp"
#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"
#include "SequenceUtils.hpp"

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

    EXPECT_TRUE(TestUtils::CheckSequence({}, &sequence, "Default empty list sequence"));
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(MutableListSequenceTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> sequence(items, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Items constructor"));
}

TEST(MutableListSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> original(items, 3);

    MutableListSequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &original, "Original sequence remains unchanged"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 99, 2, 3}, &copy, "Copied sequence after InsertAt"));
}

TEST(MutableListSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> source(items, 3);
    MutableListSequence<int> target;

    target = source;
    target.Append(4);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &source, "Source sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &target, "Target sequence after Append"));
}

TEST(MutableListSequenceTests, AppendAddsElementsToEnd)
{
    MutableListSequence<int> sequence;

    sequence.Append(1)->Append(2)->Append(3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Chained Append"));
}

TEST(MutableListSequenceTests, PrependAddsElementsToBeginning)
{
    MutableListSequence<int> sequence;

    sequence.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Chained Prepend"));
}

TEST(MutableListSequenceTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    MutableListSequence<int> sequence(items, 2);

    sequence.InsertAt(2, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "InsertAt index 1"));
}

TEST(MutableListSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    MutableListSequence<int> left(leftItems, 2);
    MutableListSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &left, "Concat two populated sequences"));
}

TEST(MutableListSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({2, 3, 4}, subSeq, "GetSubsequence(1, 3)"));

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

    Sequence<int>* mapped = SequenceUtils::Map<int>(&sequence, std::function<int(int)>(ListSequenceTests::MultiplyByTwo));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4, 6}, mapped, "Map MultiplyByTwo"));

    delete mapped;
}

TEST(MutableListSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> sequence(items, 5);

    Sequence<int>* filtered = SequenceUtils::Where<int>(&sequence, std::function<bool(int)>(ListSequenceTests::IsEven));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4}, filtered, "Where IsEven"));

    delete filtered;
}

TEST(MutableListSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> sequence(items, 4);

    int sum = SequenceUtils::Reduce<int, int>(&sequence, std::function<int(int, int)>(ListSequenceTests::SumValues));

    EXPECT_EQ(sum, 10);
}

TEST(ImmutableListSequenceTests, AppendReturnsNewSequenceAndKeepsOriginal)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> original(items, 3);

    Sequence<int>* changed = original.Append(4);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &original, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, changed, "New sequence after Append"));

    delete changed;
}

TEST(ImmutableListSequenceTests, PrependReturnsNewSequenceAndKeepsOriginal)
{
    int items[] = {2, 3};
    ImmutableListSequence<int> original(items, 2);

    Sequence<int>* changed = original.Prepend(1);

    EXPECT_TRUE(TestUtils::CheckSequence({2, 3}, &original, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, changed, "New sequence after Prepend"));

    delete changed;
}

TEST(ImmutableListSequenceTests, InsertAtReturnsNewSequenceAndKeepsOriginal)
{
    int items[] = {1, 3};
    ImmutableListSequence<int> original(items, 2);

    Sequence<int>* changed = original.InsertAt(2, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 3}, &original, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, changed, "New sequence after InsertAt"));

    delete changed;
}

TEST(ImmutableListSequenceTests, ConcatReturnsNewSequenceAndKeepsOriginal)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    ImmutableListSequence<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    Sequence<int>* changed = left.Concat(&right);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2}, &left, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, changed, "New sequence after Concat"));

    delete changed;
}

TEST(ImmutableListSequenceTests, MapReturnsNewSequenceWithoutModifyingOriginal)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> original(items, 3);

    Sequence<int>* mapped = SequenceUtils::Map<int>(&original, std::function<int(int)>(ListSequenceTests::MultiplyByTwo));

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &original, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({2, 4, 6}, mapped, "New sequence after Map"));

    delete mapped;
}

TEST(ImmutableListSequenceTests, WhereReturnsNewSequenceWithoutModifyingOriginal)
{
    int items[] = {1, 2, 3, 4};
    ImmutableListSequence<int> original(items, 4);

    Sequence<int>* filtered = SequenceUtils::Where<int>(&original, std::function<bool(int)>(ListSequenceTests::IsEven));

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &original, "Original immutable sequence"));
    EXPECT_TRUE(TestUtils::CheckSequence({2, 4}, filtered, "New sequence after Where"));

    delete filtered;
}