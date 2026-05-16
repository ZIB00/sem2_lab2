#include <gtest/gtest.h>
#include <functional>

#include "AdaptiveSequence.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"
#include "SequenceUtils.hpp"

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

    EXPECT_TRUE(TestUtils::CheckSequence({}, &sequence, "Default constructor creating empty sequence"));
    EXPECT_THROW(sequence.GetFirst(), OutOfRange);
    EXPECT_THROW(sequence.GetLast(), OutOfRange);
}

TEST(AdaptiveSequenceTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> sequence(items, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Items constructor initialization"));
}

TEST(AdaptiveSequenceTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> original(items, 3);

    AdaptiveSequence<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &original, "Original sequence after copy modification"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 99, 2, 3}, &copy, "Copied sequence after InsertAt(99, 1)"));
}

TEST(AdaptiveSequenceTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    AdaptiveSequence<int> source(items, 3);
    AdaptiveSequence<int> target;

    target = source;
    target.Append(4);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &source, "Source sequence after assignment and target modification"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &target, "Target sequence after assignment and Append(4)"));
}

TEST(AdaptiveSequenceTests, AppendAddsElementsToEnd)
{
    AdaptiveSequence<int> sequence;

    sequence.Append(1)->Append(2)->Append(3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Chained Append: 1, 2, 3"));
}

TEST(AdaptiveSequenceTests, PrependAddsElementsToBeginning)
{
    AdaptiveSequence<int> sequence;

    sequence.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "Chained Prepend: 3, 2, 1"));
}

TEST(AdaptiveSequenceTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    AdaptiveSequence<int> sequence(items, 2);

    sequence.InsertAt(2, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &sequence, "InsertAt(2, 1) into {1, 3}"));
}

TEST(AdaptiveSequenceTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    AdaptiveSequence<int> left(leftItems, 2);
    AdaptiveSequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &left, "Concat {3, 4} to {1, 2}"));
}

TEST(AdaptiveSequenceTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    AdaptiveSequence<int> sequence(items, 5);

    Sequence<int>* subSeq = sequence.GetSubsequence(1, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({2, 3, 4}, subSeq, "GetSubsequence from index 1, length 3"));

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

    Sequence<int>* mapped = SequenceUtils::Map<int>(&sequence, std::function<int(int)>(AdaptiveSequenceTests::MultiplyByTwo));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4, 6}, mapped, "Map with MultiplyByTwo function"));

    delete mapped;
}

TEST(AdaptiveSequenceTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    AdaptiveSequence<int> sequence(items, 5);

    Sequence<int>* filtered = SequenceUtils::Where<int>(&sequence, std::function<bool(int)>(AdaptiveSequenceTests::IsEven));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4}, filtered, "Where with IsEven function"));

    delete filtered;
}

TEST(AdaptiveSequenceTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    AdaptiveSequence<int> sequence(items, 4);

    int sum = SequenceUtils::Reduce<int, int>(&sequence, std::function<int(int, int)>(AdaptiveSequenceTests::SumValues));

    EXPECT_EQ(sum, 10);
}

TEST(AdaptiveSequenceTests, MultipleInsertsTriggerListBackendWithoutDataLoss)
{
    AdaptiveSequence<int> sequence;
    
    for (int i = 0; i < 10; ++i) {
        sequence.Append(i);
    }

    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, &sequence, "10 sequential Appends"));
}

TEST(AdaptiveSequenceTests, MultipleGetsTriggerArrayBackendWithoutDataLoss)
{
    int items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    AdaptiveSequence<int> sequence(items, 10);

    for (int i = 0; i < 15; ++i) {
        SCOPED_TRACE("Accessing index: " + std::to_string(i % 10)); 
        sequence.Get(i % 10);
    }

    sequence.Append(10);
    
    EXPECT_TRUE(TestUtils::CheckSequence({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, &sequence, "Multiple Gets followed by Append(10)"));
}