#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "DynamicArray.hpp"

namespace {
    template<class T>
    testing::AssertionResult CheckDynamicArray(
        const std::vector<T>& expected, 
        DynamicArray<T>& actual, 
        const std::string& contextAction = "") 
    {
        bool match = true;
        if (expected.size() != actual.GetSize()) {
            match = false;
        } else {
            for (size_t i = 0; i < expected.size(); ++i) {
                if (expected[i] != actual.Get(i)) {
                    match = false;
                    break;
                }
            }
        }

        if (match) return testing::AssertionSuccess();

        testing::AssertionResult failure = testing::AssertionFailure();
        if (!contextAction.empty()) failure << "ACTION PERFORMED: " << contextAction << "\n";
        failure << "ARRAY MISMATCH DETECTED!\n";
        
        failure << "EXPECTED: [";
        for (size_t i = 0; i < expected.size(); ++i) {
            failure << expected[i] << (i < expected.size() - 1 ? ", " : "");
        }
        failure << "] (size: " << expected.size() << ")\n";

        failure << "RECEIVED: [";
        for (size_t i = 0; i < actual.GetSize(); ++i) {
            failure << actual.Get(i) << (i < actual.GetSize() - 1 ? ", " : "");
        }
        failure << "] (size: " << actual.GetSize() << ")\n";

        return failure;
    }
}

TEST(DynamicArrayTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, array, "Items constructor initialization"));
}

TEST(DynamicArrayTests, SizeConstructorAllowZeroSize)
{
    DynamicArray<int> array(0);

    EXPECT_TRUE(CheckDynamicArray({}, array, "Zero size constructor"));
}

TEST(DynamicArrayTests, SizeConstructorRequestedSize)
{
    DynamicArray<int> array(3);

    EXPECT_EQ(array.GetSize(), 3);
}

TEST(DynamicArrayTests, ItemsConstructorMakesIndependentCopy)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);
    
    items[0] = 100;

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, array, "Array after external source modification"));
}

TEST(DynamicArrayTests, ItemsConstructorAllowsZeroCountWithNullItems)
{
    DynamicArray<int> array(nullptr, 0);

    EXPECT_TRUE(CheckDynamicArray({}, array, "Constructor with nullptr and size 0"));
}

TEST(DynamicArrayTests, ItemsConstructorAllowsZeroCountWithNonNullItems)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 0);

    EXPECT_TRUE(CheckDynamicArray({}, array, "Constructor with valid pointer but size 0"));
}

TEST(DynamicArrayTests, ItemsConstructorRejectsNullItemsWhenCountIsPositive)
{
    EXPECT_THROW(DynamicArray<int> array(nullptr, 1), InvalidArgument);
}

TEST(DynamicArrayTests, GetReturnsValuesSetBySet)
{
    DynamicArray<int> array(3);

    array.Set(0, 10);
    array.Set(1, 20);
    array.Set(2, 30);

    EXPECT_TRUE(CheckDynamicArray({10, 20, 30}, array, "Sequential Set operations"));
}

TEST(DynamicArrayTests, GetRejectsIndexEqualToSize)
{
    DynamicArray<int> array(3);
    EXPECT_THROW(array.Get(3), OutOfRange);
}

TEST(DynamicArrayTests, GetRejectsIndexGreaterThanSize)
{
    DynamicArray<int> array(3);
    EXPECT_THROW(array.Get(10), OutOfRange);
}

TEST(DynamicArrayTests, GetRejectsIndexInEmptyArray)
{
    DynamicArray<int> array(0);
    EXPECT_THROW(array.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, SetChangesElement)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Set(1, 200);

    EXPECT_TRUE(CheckDynamicArray({1, 200, 3}, array, "Set(1, 200) on {1, 2, 3}"));
}

TEST(DynamicArrayTests, SetRejectsIndexEqualToSize)
{
    DynamicArray<int> array(3);
    EXPECT_THROW(array.Set(3, 10), OutOfRange);
}

TEST(DynamicArrayTests, SetRejectsIndexGreaterThanSize)
{
    DynamicArray<int> array(3);
    EXPECT_THROW(array.Set(10, 10), OutOfRange);
}

TEST(DynamicArrayTests, SetRejectsIndexInEmptyArray)
{
    DynamicArray<int> array(0);
    EXPECT_THROW(array.Set(0, 10), OutOfRange);
}

TEST(DynamicArrayTests, ResizeToLargerSizeChangesSizeAndKeepsExistingElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(5);

    EXPECT_EQ(array.GetSize(), 5);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, ResizeToSmallerSizeChangesSizeAndKeepsPrefix)
{
    int items[] = {1, 2, 3, 4, 5};
    DynamicArray<int> array(items, 5);

    array.Resize(3);

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, array, "Resize from 5 to 3"));
    EXPECT_THROW(array.Get(3), OutOfRange);
}

TEST(DynamicArrayTests, ResizeToSameSizeKeepsElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(3);

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, array, "Resize to same size"));
}

TEST(DynamicArrayTests, ResizeToZeroClearsArray)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(0);

    EXPECT_TRUE(CheckDynamicArray({}, array, "Resize to 0"));
    EXPECT_THROW(array.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, ResizeFromZeroToPositiveCreatesUsableArray)
{
    DynamicArray<int> array(0);

    array.Resize(2);
    array.Set(0, 10);
    array.Set(1, 20);

    EXPECT_TRUE(CheckDynamicArray({10, 20}, array, "Resize from 0 to 2 and populate"));
}

TEST(DynamicArrayTests, ResizeCanGrowAfterClear)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(0);
    array.Resize(1);
    array.Set(0, 42);

    EXPECT_TRUE(CheckDynamicArray({42}, array, "Resize to 0, then to 1 and populate"));
}

TEST(DynamicArrayTests, CopyConstructorCopiesSizeAndElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> original(items, 3);

    DynamicArray<int> copy(original);

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, copy, "Copy constructor"));
}

TEST(DynamicArrayTests, CopyConstructorCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> original(items, 3);

    DynamicArray<int> copy(original);
    copy.Set(0, 100);

    EXPECT_EQ(original.Get(0), 1);
    EXPECT_TRUE(CheckDynamicArray({100, 2, 3}, copy, "Copied array after Set(0, 100)"));
}

TEST(DynamicArrayTests, CopyConstructorCopiesEmptyArray)
{
    DynamicArray<int> original(0);
    DynamicArray<int> copy(original);

    EXPECT_TRUE(CheckDynamicArray({}, copy, "Copy of empty array"));
    EXPECT_THROW(copy.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, AssignmentCopiesSizeAndElements)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};
    DynamicArray<int> source(sourceItems, 3);
    DynamicArray<int> target(targetItems, 2);

    target = source;

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, target, "Assignment from larger array"));
}

TEST(DynamicArrayTests, AssignmentCreatesIndependentStorage)
{
    int sourceItems[] = {1, 2, 3};
    DynamicArray<int> source(sourceItems, 3);
    DynamicArray<int> target(0);

    target = source;
    target.Set(0, 100);

    EXPECT_EQ(source.Get(0), 1);
    EXPECT_TRUE(CheckDynamicArray({100, 2, 3}, target, "Assigned array after Set(0, 100)"));
}

TEST(DynamicArrayTests, AssignmentFromEmptyArrayClearsTarget)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> source(0);
    DynamicArray<int> target(items, 3);

    target = source;

    EXPECT_TRUE(CheckDynamicArray({}, target, "Assignment from empty array"));
    EXPECT_THROW(target.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, AssignmentToEmptyArrayCopiesSource)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> source(items, 3);
    DynamicArray<int> target(0);

    target = source;

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, target, "Assignment to empty array"));
}

TEST(DynamicArrayTests, SelfAssignmentKeepsArrayValid)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array = array;

    EXPECT_TRUE(CheckDynamicArray({1, 2, 3}, array, "Self-assignment"));
}

TEST(DynamicArrayTests, WorksWithStringValues)
{
    DynamicArray<std::string> array(2);

    array.Set(0, "first");
    array.Set(1, "second");

    EXPECT_TRUE(CheckDynamicArray<std::string>({"first", "second"}, array, "Array of strings"));
}