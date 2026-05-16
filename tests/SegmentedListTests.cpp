#include <gtest/gtest.h>
#include <functional>

#include "SegmentedList.hpp"
#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"
#include "SequenceUtils.hpp"

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

    EXPECT_TRUE(TestUtils::CheckSequence({}, &list, "Default constructor creates empty sequence"));
    EXPECT_THROW(list.GetFirst(), OutOfRange);
    EXPECT_THROW(list.GetLast(), OutOfRange);
}

TEST(SegmentedListTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> list(items, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &list, "Items constructor copies elements"));
}

TEST(SegmentedListTests, CopyConstructorCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> original(items, 3);

    SegmentedList<int> copy(original);
    copy.InsertAt(99, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &original, "Original sequence remains unchanged"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 99, 2, 3}, &copy, "Copied sequence creates independent storage"));
}

TEST(SegmentedListTests, AssignmentCreatesIndependentSequence)
{
    int items[] = {1, 2, 3};
    SegmentedList<int> source(items, 3);
    SegmentedList<int> target;

    target = source;
    target.Append(4);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &source, "Source sequence remains unchanged"));
    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &target, "Target sequence creates independent storage"));
}

TEST(SegmentedListTests, AppendAddsElementsToEnd)
{
    SegmentedList<int> list;

    list.Append(1)->Append(2)->Append(3);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &list, "Append elements to end"));
}

TEST(SegmentedListTests, PrependAddsElementsToBeginning)
{
    SegmentedList<int> list;

    list.Prepend(3)->Prepend(2)->Prepend(1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &list, "Prepend elements to beginning"));
}

TEST(SegmentedListTests, InsertAtPlacesElementCorrectly)
{
    int items[] = {1, 3};
    SegmentedList<int> list(items, 2);

    list.InsertAt(2, 1);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, &list, "InsertAt places element correctly"));
}

TEST(SegmentedListTests, ConcatAppendsAnotherSequence)
{
    int leftItems[] = {1, 2};
    int rightItems[] = {3, 4};
    SegmentedList<int> left(leftItems, 2);
    MutableArraySequence<int> right(rightItems, 2);

    left.Concat(&right);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, &left, "Concat appends another sequence"));
}

TEST(SegmentedListTests, GetSubsequenceReturnsCorrectRange)
{
    int items[] = {1, 2, 3, 4, 5};
    SegmentedList<int> list(items, 5);

    Sequence<int>* subSeq = list.GetSubsequence(1, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({2, 3, 4}, subSeq, "GetSubsequence returns correct range"));

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

    Sequence<int>* mapped = SequenceUtils::Map<int>(&list, std::function<int(int)>(SegmentedListTests::MultiplyByTwo));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4, 6}, mapped, "Map transforms elements"));

    delete mapped;
}

TEST(SegmentedListTests, WhereFiltersElements)
{
    int items[] = {1, 2, 3, 4, 5};
    SegmentedList<int> list(items, 5);

    Sequence<int>* filtered = SequenceUtils::Where<int>(&list, std::function<bool(int)>(SegmentedListTests::IsEven));

    EXPECT_TRUE(TestUtils::CheckSequence({2, 4}, filtered, "Where filters elements"));

    delete filtered;
}

TEST(SegmentedListTests, ReduceAccumulatesValues)
{
    int items[] = {1, 2, 3, 4};
    SegmentedList<int> list(items, 4);

    int sum = SequenceUtils::Reduce<int, int>(&list, std::function<int(int, int)>(SegmentedListTests::SumValues));

    EXPECT_EQ(sum, 10);
}

TEST(SegmentedListTests, AppendWorksAcrossMultipleSegments)
{
    SegmentedList<int> list;

    for (int i = 0; i < 25; ++i) {
        list.Append(i);
    }

    EXPECT_TRUE(TestUtils::CheckSequence(
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24}, 
        &list, 
        "Append 25 elements across multiple segments"
    ));
}

TEST(SegmentedListTests, PrependWorksAcrossMultipleSegments)
{
    SegmentedList<int> list;

    for (int i = 0; i < 25; ++i) {
        list.Prepend(i);
    }

    EXPECT_TRUE(TestUtils::CheckSequence(
        {24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, 
        &list, 
        "Prepend 25 elements across multiple segments"
    ));
}