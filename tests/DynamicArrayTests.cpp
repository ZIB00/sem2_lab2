#include <gtest/gtest.h>
#include <string>
#include "DynamicArray.hpp"

TEST(DynamicArrayTests, ItemsConstructorCopiesElements)
{
    int items[] = {1, 2, 3};

    DynamicArray<int> array(items, 3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, SizeConstructorAllowZeroSize)
{
    DynamicArray<int> array(0);

    EXPECT_EQ(array.GetSize(), 0);
}

TEST(DynamicArrayTests, SizeConstructorRequestedSize)
{
    DynamicArray<int> array(3);

    EXPECT_EQ(array.GetSize(), 3);
}

TEST(DynamicArrayTests, SizeConstructorNegativeSize)
{
    EXPECT_THROW(DynamicArray<int> array(-1), InvalidArgument);
}

TEST(DynamicArrayTests, ItemsConstructorMakesIndependentCopy)
{
    int items[] = {1, 2, 3};

    DynamicArray<int> array(items, 3);
    items[0] = 100;

    EXPECT_EQ(array.Get(0), 1);
}

TEST(DynamicArrayTests, ItemsConstructorAllowsZeroCountWithNullItems)
{
    DynamicArray<int> array(nullptr, 0);

    EXPECT_EQ(array.GetSize(), 0);
}

TEST(DynamicArrayTests, ItemsConstructorAllowsZeroCountWithNonNullItems)
{
    int items[] = {1, 2, 3};

    DynamicArray<int> array(items, 0);

    EXPECT_EQ(array.GetSize(), 0);
}

TEST(DynamicArrayTests, ItemsConstructorRejectsNegativeCount)
{
    int items[] = {1, 2, 3};

    EXPECT_THROW(DynamicArray<int> array(items, -1), InvalidArgument);
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

    EXPECT_EQ(array.Get(0), 10);
    EXPECT_EQ(array.Get(1), 20);
    EXPECT_EQ(array.Get(2), 30);
}

TEST(DynamicArrayTests, GetRejectsNegativeIndex)
{
    DynamicArray<int> array(3);

    EXPECT_THROW(array.Get(-1), InvalidArgument);
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

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 200);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, SetRejectsNegativeIndex)
{
    DynamicArray<int> array(3);

    EXPECT_THROW(array.Set(-1, 10), InvalidArgument);
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

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
    EXPECT_THROW(array.Get(3), OutOfRange);
}

TEST(DynamicArrayTests, ResizeToSameSizeKeepsElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, ResizeToZeroClearsArray)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(0);

    EXPECT_EQ(array.GetSize(), 0);
    EXPECT_THROW(array.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, ResizeFromZeroToPositiveCreatesUsableArray)
{
    DynamicArray<int> array(0);

    array.Resize(2);
    array.Set(0, 10);
    array.Set(1, 20);

    EXPECT_EQ(array.GetSize(), 2);
    EXPECT_EQ(array.Get(0), 10);
    EXPECT_EQ(array.Get(1), 20);
}

TEST(DynamicArrayTests, ResizeCanGrowAfterClear)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(0);
    array.Resize(1);
    array.Set(0, 42);

    EXPECT_EQ(array.GetSize(), 1);
    EXPECT_EQ(array.Get(0), 42);
}

TEST(DynamicArrayTests, ResizeRejectsNegativeSize)
{
    DynamicArray<int> array(3);

    EXPECT_THROW(array.Resize(-1), InvalidArgument);
}

TEST(DynamicArrayTests, CopyConstructorCopiesSizeAndElements)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> original(items, 3);

    DynamicArray<int> copy(original);

    EXPECT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
    EXPECT_EQ(copy.Get(2), 3);
}

TEST(DynamicArrayTests, CopyConstructorCreatesIndependentStorage)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> original(items, 3);

    DynamicArray<int> copy(original);
    copy.Set(0, 100);

    EXPECT_EQ(original.Get(0), 1);
    EXPECT_EQ(copy.Get(0), 100);
}

TEST(DynamicArrayTests, CopyConstructorCopiesEmptyArray)
{
    DynamicArray<int> original(0);

    DynamicArray<int> copy(original);

    EXPECT_EQ(copy.GetSize(), 0);
    EXPECT_THROW(copy.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, AssignmentCopiesSizeAndElements)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};
    DynamicArray<int> source(sourceItems, 3);
    DynamicArray<int> target(targetItems, 2);

    target = source;

    EXPECT_EQ(target.GetSize(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);
}

TEST(DynamicArrayTests, AssignmentCreatesIndependentStorage)
{
    int sourceItems[] = {1, 2, 3};
    DynamicArray<int> source(sourceItems, 3);
    DynamicArray<int> target(0);

    target = source;
    target.Set(0, 100);

    EXPECT_EQ(source.Get(0), 1);
    EXPECT_EQ(target.Get(0), 100);
}

TEST(DynamicArrayTests, AssignmentFromEmptyArrayClearsTarget)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> source(0);
    DynamicArray<int> target(items, 3);

    target = source;

    EXPECT_EQ(target.GetSize(), 0);
    EXPECT_THROW(target.Get(0), OutOfRange);
}

TEST(DynamicArrayTests, AssignmentToEmptyArrayCopiesSource)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> source(items, 3);
    DynamicArray<int> target(0);

    target = source;

    EXPECT_EQ(target.GetSize(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);
}

TEST(DynamicArrayTests, SelfAssignmentKeepsArrayValid)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array = array;

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, ConstArrayAllowsGetAndGetSize)
{
    int items[] = {1, 2, 3};
    const DynamicArray<int> array(items, 3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
}

TEST(DynamicArrayTests, WorksWithStringValues)
{
    DynamicArray<std::string> array(2);

    array.Set(0, "first");
    array.Set(1, "second");

    EXPECT_EQ(array.GetSize(), 2);
    EXPECT_EQ(array.Get(0), "first");
    EXPECT_EQ(array.Get(1), "second");
}