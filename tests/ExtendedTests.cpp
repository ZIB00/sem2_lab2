#include <gtest/gtest.h>
#include <functional>

#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "SegmentedList.hpp"
#include "AdaptiveSequence.hpp"
#include "BitSequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Option.hpp"
#include "SequenceUtils.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"

namespace Helpers
{
    int Double(int x) { return x * 2; }
    bool IsEven(int x) { return x % 2 == 0; }
    bool IsPositive(int x) { return x > 0; }
    int Sum(int a, int b) { return a + b; }
    int ToAbsolute(int x) { return x < 0 ? -x : x; }

    Sequence<int>* PairExpand(int x)
    {
        int items[] = { x, x + 1 };
        return new MutableArraySequence<int>(items, 2);
    }

    Sequence<int>* PairExpandList(int x)
    {
        int items[] = { x, x + 1 };
        return new MutableListSequence<int>(items, 2);
    }

    double AsDouble(int x) { return static_cast<double>(x); }

    Option<double> SafeReciprocal(int x)
    {
        if (x == 0) return Option<double>();
        return Option<double>(1.0 / x);
    }
}


// DynamicArray — оператор []

TEST(DynamicArrayExtendedTests, BracketOperatorReadsAndWrites)
{
    int items[] = { 10, 20, 30 };
    DynamicArray<int> arr(items, 3);

    EXPECT_EQ(arr[0], 10);
    arr[1] = 99;
    EXPECT_EQ(arr[1], 99);
    EXPECT_EQ(arr[2], 30);
}

TEST(DynamicArrayExtendedTests, BracketOperatorThrowsOnOutOfRange)
{
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr[2], OutOfRange);
    EXPECT_THROW(arr[100], OutOfRange);
}

TEST(DynamicArrayExtendedTests, ConstBracketOperatorReadsCorrectly)
{
    int items[] = { 5, 6, 7 };
    const DynamicArray<int> arr(items, 3);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[2], 7);
}


// LinkedList — оператор []

TEST(LinkedListExtendedTests, BracketOperatorReadsAndWrites)
{
    int items[] = { 1, 2, 3 };
    LinkedList<int> list(items, 3);

    EXPECT_EQ(list[0], 1);
    list[2] = 42;
    EXPECT_EQ(list[2], 42);
}

TEST(LinkedListExtendedTests, BracketOperatorThrowsOnOutOfRange)
{
    int items[] = { 1, 2 };
    LinkedList<int> list(items, 2);
    EXPECT_THROW(list[2], OutOfRange);
    EXPECT_THROW(list[10], OutOfRange);
}

TEST(LinkedListExtendedTests, ConstBracketOperatorReadsCorrectly)
{
    int items[] = { 7, 8 };
    const LinkedList<int> list(items, 2);
    EXPECT_EQ(list[0], 7);
    EXPECT_EQ(list[1], 8);
}


// MutableArraySequence — операторы [], +, ==, !=

TEST(MutableArraySequenceExtendedTests, BracketOperatorReadsElement)
{
    int items[] = { 10, 20, 30 };
    MutableArraySequence<int> seq(items, 3);
    EXPECT_EQ(seq[0], 10);
    EXPECT_EQ(seq[2], 30);
}

TEST(MutableArraySequenceExtendedTests, BracketOperatorWritesElement)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);
    seq[1] = 99;
    EXPECT_EQ(seq.Get(1), 99);
}

TEST(MutableArraySequenceExtendedTests, BracketOperatorThrowsOnOutOfRange)
{
    MutableArraySequence<int> seq;
    EXPECT_THROW(seq[0], OutOfRange);
}

TEST(MutableArraySequenceExtendedTests, PlusOperatorConcatenatesTwoSequences)
{
    int a[] = { 1, 2 };
    int b[] = { 3, 4 };
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 2);

    Sequence<int>* result = seqA + &seqB;

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, result, "Plus operator: concatenate two sequences"));

    delete result;
}

TEST(MutableArraySequenceExtendedTests, PlusOperatorWithEmptyRight)
{
    int a[] = { 1, 2, 3 };
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<int> empty;

    Sequence<int>* result = seqA + &empty;

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, result, "Plus operator: concatenate with empty right"));

    delete result;
}

TEST(MutableArraySequenceExtendedTests, PlusOperatorWithEmptyLeft)
{
    int b[] = { 1, 2 };
    MutableArraySequence<int> empty;
    MutableArraySequence<int> seqB(b, 2);

    Sequence<int>* result = empty + &seqB;

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2}, result, "Plus operator: concatenate with empty left"));

    delete result;
}

TEST(MutableArraySequenceExtendedTests, EqualityOperatorReturnsTrueForEqual)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> a(items, 3);
    MutableArraySequence<int> b(items, 3);
    EXPECT_TRUE(a == &b);
}

TEST(MutableArraySequenceExtendedTests, EqualityOperatorReturnsFalseForDifferentContent)
{
    int a[] = { 1, 2, 3 };
    int b[] = { 1, 2, 4 };
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<int> seqB(b, 3);
    EXPECT_FALSE(seqA == &seqB);
}

TEST(MutableArraySequenceExtendedTests, EqualityOperatorReturnsFalseForDifferentLengths)
{
    int a[] = { 1, 2 };
    int b[] = { 1, 2, 3 };
    MutableArraySequence<int> seqA(a, 2);
    MutableArraySequence<int> seqB(b, 3);
    EXPECT_FALSE(seqA == &seqB);
}

TEST(MutableArraySequenceExtendedTests, InequalityOperatorConsistentWithEquality)
{
    int items[] = { 5, 6 };
    MutableArraySequence<int> a(items, 2);
    MutableArraySequence<int> b(items, 2);
    MutableArraySequence<int> c;
    EXPECT_FALSE(a != &b);
    EXPECT_TRUE(a != &c);
}

TEST(MutableArraySequenceExtendedTests, SelfEqualityIsTrue)
{
    int items[] = { 1, 2 };
    MutableArraySequence<int> seq(items, 2);
    EXPECT_TRUE(seq == &seq);
}


// MutableListSequence — операторы [], +, ==, !=

TEST(MutableListSequenceExtendedTests, BracketOperatorReadsAndWrites)
{
    int items[] = { 10, 20, 30 };
    MutableListSequence<int> seq(items, 3);

    EXPECT_EQ(seq[1], 20);
    seq[1] = 55;
    EXPECT_EQ(seq.Get(1), 55);
}

TEST(MutableListSequenceExtendedTests, PlusOperatorConcatenates)
{
    int a[] = { 1, 2 };
    int b[] = { 3, 4 };
    MutableListSequence<int> seqA(a, 2);
    MutableListSequence<int> seqB(b, 2);

    Sequence<int>* result = seqA + &seqB;

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, result, "Plus operator: concatenate two sequences"));

    delete result;
}

TEST(MutableListSequenceExtendedTests, EqualityOperatorWorks)
{
    int items[] = { 7, 8, 9 };
    MutableListSequence<int> a(items, 3);
    MutableListSequence<int> b(items, 3);
    EXPECT_TRUE(a == &b);
    EXPECT_FALSE(a != &b);
}


// SegmentedList — операторы [], +, ==, !=

TEST(SegmentedListExtendedTests, BracketOperatorReadsAndWrites)
{
    int items[] = { 1, 2, 3 };
    SegmentedList<int> list(items, 3);

    EXPECT_EQ(list[0], 1);
    list[2] = 99;
    EXPECT_EQ(list.Get(2), 99);
}

TEST(SegmentedListExtendedTests, PlusOperatorConcatenates)
{
    int a[] = { 1, 2 };
    int b[] = { 3, 4 };
    SegmentedList<int> seqA(a, 2);
    SegmentedList<int> seqB(b, 2);

    Sequence<int>* result = seqA + &seqB;

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3, 4}, result, "Plus operator: concatenate two sequences"));

    delete result;
}

TEST(SegmentedListExtendedTests, EqualityAndInequalityWork)
{
    int items[] = { 1, 2, 3 };
    SegmentedList<int> a(items, 3);
    SegmentedList<int> b(items, 3);
    EXPECT_TRUE(a == &b);
    EXPECT_FALSE(a != &b);

    b.Append(4);
    EXPECT_FALSE(a == &b);
    EXPECT_TRUE(a != &b);
}


// AdaptiveSequence — операторы [], +, ==, !=

TEST(AdaptiveSequenceExtendedTests, BracketOperatorReadsElement)
{
    int items[] = { 10, 20, 30 };
    AdaptiveSequence<int> seq(items, 3);

    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(1), 20);
    EXPECT_EQ(seq.Get(2), 30);
}


// Option<T>

TEST(OptionTests, DefaultConstructorCreatesNone)
{
    Option<int> opt;
    EXPECT_FALSE(opt.HasValue());
}

TEST(OptionTests, ValueConstructorCreatesSome)
{
    Option<int> opt(42);
    EXPECT_TRUE(opt.HasValue());
    EXPECT_EQ(opt.Value(), 42);
}

TEST(OptionTests, GetValueOnNoneThrows)
{
    Option<int> opt;
    EXPECT_THROW(opt.Value(), BadOptionalAccess);
}

TEST(OptionTests, ValueOrReturnsValueWhenPresent)
{
    Option<int> opt(10);
    EXPECT_EQ(opt.ValueOr(99), 10);
}

TEST(OptionTests, ValueOrReturnsDefaultWhenNone)
{
    Option<int> opt;
    EXPECT_EQ(opt.ValueOr(99), 99);
}

// GetFirst(predicate) / GetLast(predicate) — ArraySequence

TEST(ArraySequenceOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 3, 4, 6, 7 };
    MutableArraySequence<int> seq(items, 5);

    Option<int> result = SequenceUtils::GetFirst<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 4);
}

TEST(ArraySequenceOptionTests, GetFirstWithPredicateReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Option<int> result = SequenceUtils::GetFirst<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_FALSE(result.HasValue());
}

TEST(ArraySequenceOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 2, 4, 3, 6, 7 };
    MutableArraySequence<int> seq(items, 5);

    Option<int> result = SequenceUtils::GetLast<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 6);
}

TEST(ArraySequenceOptionTests, GetLastWithPredicateReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Option<int> result = SequenceUtils::GetLast<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_FALSE(result.HasValue());
}

TEST(ArraySequenceOptionTests, GetFirstOnEmptySequenceReturnsNone)
{
    MutableArraySequence<int> seq;
    Option<int> result = SequenceUtils::GetFirst<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_FALSE(result.HasValue());
}


// GetFirst(predicate) / GetLast(predicate) — ListSequence

TEST(ListSequenceOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 3, 4, 5 };
    MutableListSequence<int> seq(items, 4);

    Option<int> result = SequenceUtils::GetFirst<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 4);
}

TEST(ListSequenceOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 2, 5, 8, 9 };
    MutableListSequence<int> seq(items, 4);

    Option<int> result = SequenceUtils::GetLast<int>(&seq, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 8);
}


// GetFirst(predicate) / GetLast(predicate) — SegmentedList

TEST(SegmentedListOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 2, 3, 4 };
    SegmentedList<int> list(items, 4);

    Option<int> result = SequenceUtils::GetFirst<int>(&list, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 2);
}

TEST(SegmentedListOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 1, 2, 3, 4 };
    SegmentedList<int> list(items, 4);

    Option<int> result = SequenceUtils::GetLast<int>(&list, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 4);
}

TEST(SegmentedListOptionTests, GetFirstReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    SegmentedList<int> list(items, 3);

    Option<int> result = SequenceUtils::GetFirst<int>(&list, std::function<bool(int)>(Helpers::IsEven));
    EXPECT_FALSE(result.HasValue());
}


// IEnumerator — ArraySequence

TEST(ArraySequenceEnumeratorTests, EnumeratorIteratesAllElements)
{
    int items[] = { 10, 20, 30 };
    MutableArraySequence<int> seq(items, 3);

    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 10);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 20);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 30);
    EXPECT_FALSE(it->MoveNext());

    delete it;
}

TEST(ArraySequenceEnumeratorTests, EnumeratorOnEmptySequenceReturnsFalseImmediately)
{
    MutableArraySequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());

    delete it;
}

TEST(ArraySequenceEnumeratorTests, GetCurrentBeforeMoveNextThrows)
{
    int items[] = { 1, 2 };
    MutableArraySequence<int> seq(items, 2);
    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_THROW(it->GetCurrent(), OutOfRange);

    delete it;
}

TEST(ArraySequenceEnumeratorTests, ResetAllowsSecondIteration)
{
    int items[] = { 5, 6 };
    MutableArraySequence<int> seq(items, 2);
    IEnumerator<int>* it = seq.GetEnumerator();

    it->MoveNext(); it->MoveNext(); // исчерпан
    it->Reset();

    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 5);

    delete it;
}


// IEnumerator — ListSequence

TEST(ListSequenceEnumeratorTests, EnumeratorIteratesAllElements)
{
    int items[] = { 1, 2, 3 };
    MutableListSequence<int> seq(items, 3);

    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 1);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 2);
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 3);
    EXPECT_FALSE(it->MoveNext());

    delete it;
}

TEST(ListSequenceEnumeratorTests, ResetAllowsSecondIteration)
{
    int items[] = { 9, 8 };
    MutableListSequence<int> seq(items, 2);
    IEnumerator<int>* it = seq.GetEnumerator();

    it->MoveNext(); it->MoveNext();
    it->Reset();

    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent(), 9);

    delete it;
}


// SequenceUtils::Skip — ArraySequence

TEST(ArraySequenceSkipTests, SkipZeroReturnsWholeSequence)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 0);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2, 3}, result, "Skip: skip 0 elements"));

    delete result;
}

TEST(ArraySequenceSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seq(items, 5);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 2);

    EXPECT_TRUE(TestUtils::CheckSequence({3, 4, 5}, result, "Skip: skip 2 elements"));

    delete result;
}

TEST(ArraySequenceSkipTests, SkipAllReturnsEmpty)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({}, result, "Skip: skip all elements"));

    delete result;
}

TEST(ArraySequenceSkipTests, SkipMoreThanLengthReturnsEmpty)
{
    int items[] = { 1, 2 };
    MutableArraySequence<int> seq(items, 2);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 100);

    EXPECT_TRUE(TestUtils::CheckSequence({}, result, "Skip: skip more than length"));

    delete result;
}


// SequenceUtils::Skip — ListSequence

TEST(ListSequenceSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 10, 20, 30, 40 };
    MutableListSequence<int> seq(items, 4);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 2);

    EXPECT_TRUE(TestUtils::CheckSequence({30, 40}, result, "Skip: skip 2 elements"));

    delete result;
}

TEST(ListSequenceSkipTests, SkipAllOrMoreReturnsEmpty)
{
    int items[] = { 1, 2 };
    MutableListSequence<int> seq(items, 2);

    Sequence<int>* result = SequenceUtils::Skip<int>(&seq, 5);
    EXPECT_TRUE(TestUtils::CheckSequence({}, result, "Skip: skip more than length"));

    delete result;
}


// SequenceUtils::Skip — SegmentedList

TEST(SegmentedListSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 1, 2, 3, 4, 5 };
    SegmentedList<int> list(items, 5);

    Sequence<int>* result = SequenceUtils::Skip<int>(&list, 3);

    EXPECT_TRUE(TestUtils::CheckSequence({4, 5}, result, "Skip: skip 3 elements"));

    delete result;
}


// SequenceUtils::Splice — ArraySequence

TEST(ArraySequenceSpliceTests, SpliceRemovesAndInserts)
{
    int items[] = { 1, 2, 3, 4, 5 };
    int ins[]   = { 9, 10 };
    MutableArraySequence<int> seq(items, 5);
    MutableArraySequence<int> insert(ins, 2);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 1, 2, &insert);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 9, 10, 4, 5}, result, "Splice: removed 2 elements from index 1, inserted {9, 10}"));

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceWithNullInsertJustDeletes)
{
    int items[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(items, 4);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 1, 2, nullptr);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 4}, result, "Splice: removed 2 elements from index 1, no insert"));

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceAtStartRemovesPrefix)
{
    int items[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(items, 4);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 0, 2, nullptr);

    EXPECT_TRUE(TestUtils::CheckSequence({3, 4}, result, "Splice: removed 2 elements from start, no insert"));

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceAtEndRemovesSuffix)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 2, 1, nullptr);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 2}, result, "Splice: removed 1 element from index 2, no insert"));

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceCountExceedingRemainingClampsToEnd)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 1, 100, nullptr);

    EXPECT_TRUE(TestUtils::CheckSequence({1}, result, "Splice: removed from index 1 to end (count > remaining), no insert"));

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceWithOutOfBoundsIndexThrows)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    EXPECT_THROW(SequenceUtils::Splice<int>(&seq, 10, 1, nullptr), OutOfRange);
}


// SequenceUtils::Splice — ListSequence

TEST(ListSequenceSpliceTests, SpliceRemovesAndInserts)
{
    int items[] = { 1, 2, 3, 4, 5 };
    int ins[]   = { 9, 10 };
    MutableListSequence<int> seq(items, 5);
    MutableListSequence<int> insert(ins, 2);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 1, 2, &insert);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 9, 10, 4, 5}, result, "Splice: removed 2 elements from index 1, inserted {9, 10}"));

    delete result;
}

TEST(ListSequenceSpliceTests, SpliceWithNullInsertJustDeletes)
{
    int items[] = { 1, 2, 3 };
    MutableListSequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 0, 1, nullptr);

    EXPECT_TRUE(TestUtils::CheckSequence({2, 3}, result, "Splice: removed 1 element from index 0, no insert"));

    delete result;
}


// SequenceUtils::FlatMap — ArraySequence

TEST(ArraySequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::FlatMap<int>(&seq, std::function<Sequence<int>*(int)>(Helpers::PairExpand));

    EXPECT_EQ(result->GetLength(), 6);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 2);
    EXPECT_EQ(result->Get(5), 4);

    delete result;
}

TEST(ArraySequenceFlatMapTests, FlatMapOnEmptyReturnsEmpty)
{
    MutableArraySequence<int> seq;

    Sequence<int>* result = SequenceUtils::FlatMap<int>(&seq, std::function<Sequence<int>*(int)>(Helpers::PairExpand));

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// SequenceUtils::FlatMap — ListSequence

TEST(ListSequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 10, 20 };
    MutableListSequence<int> seq(items, 2);

    Sequence<int>* result = SequenceUtils::FlatMap<int>(&seq, std::function<Sequence<int>*(int)>(Helpers::PairExpandList));

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 11);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 21);

    delete result;
}


// SequenceUtils::FlatMap — SegmentedList

TEST(SegmentedListFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 1, 2 };
    SegmentedList<int> list(items, 2);

    Sequence<int>* result = SequenceUtils::FlatMap<int>(&list, std::function<Sequence<int>*(int)>(Helpers::PairExpand));

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(2), 2);

    delete result;
}


// SequenceUtils::FlatMap — AdaptiveSequence

TEST(AdaptiveSequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 3, 4 };
    AdaptiveSequence<int> seq(items, 2);

    Sequence<int>* result = SequenceUtils::FlatMap<int>(&seq, std::function<Sequence<int>*(int)>(Helpers::PairExpand));

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 3);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 4);
    EXPECT_EQ(result->Get(3), 5);

    delete result;
}


// Reduce на одном элементе

TEST(ReduceEdgeCaseTests, ReduceOnSingleElementReturnsThatElement)
{
    int items[] = { 42 };
    MutableArraySequence<int> arrSeq(items, 1);
    MutableListSequence<int> listSeq(items, 1);
    SegmentedList<int> segList(items, 1);
    AdaptiveSequence<int> adaptSeq(items, 1);

    EXPECT_EQ((SequenceUtils::Reduce<int, int>(&arrSeq, std::function<int(int, int)>(Helpers::Sum))), 42);
    EXPECT_EQ((SequenceUtils::Reduce<int, int>(&listSeq, std::function<int(int, int)>(Helpers::Sum))), 42);
    EXPECT_EQ((SequenceUtils::Reduce<int, int>(&segList, std::function<int(int, int)>(Helpers::Sum))), 42);
    EXPECT_EQ((SequenceUtils::Reduce<int, int>(&adaptSeq, std::function<int(int, int)>(Helpers::Sum))), 42);
}

TEST(ReduceEdgeCaseTests, ReduceOnEmptySequenceThrows)
{
    MutableArraySequence<int> arrSeq;
    MutableListSequence<int> listSeq;
    SegmentedList<int> segList;
    AdaptiveSequence<int> adaptSeq;

    auto reduceArr = [&]() { SequenceUtils::Reduce<int, int>(&arrSeq, std::function<int(int, int)>(Helpers::Sum)); };
    auto reduceList = [&]() { SequenceUtils::Reduce<int, int>(&listSeq, std::function<int(int, int)>(Helpers::Sum)); };
    auto reduceSeg = [&]() { SequenceUtils::Reduce<int, int>(&segList, std::function<int(int, int)>(Helpers::Sum)); };
    auto reduceAdapt = [&]() { SequenceUtils::Reduce<int, int>(&adaptSeq, std::function<int(int, int)>(Helpers::Sum)); };

    EXPECT_THROW(reduceArr(), OutOfRange);
    EXPECT_THROW(reduceList(), OutOfRange);
    EXPECT_THROW(reduceSeg(), OutOfRange);
    EXPECT_THROW(reduceAdapt(), OutOfRange);
}


// Where на последовательности, где все / ни один не проходит фильтр

TEST(WhereEdgeCaseTests, WhereAllMatchReturnsFullCopy)
{
    int items[] = { 2, 4, 6 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Where<int>(&seq, std::function<bool(int)>(Helpers::IsEven));

    EXPECT_EQ(result->GetLength(), 3);

    delete result;
}

TEST(WhereEdgeCaseTests, WhereNoneMatchReturnsEmpty)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = SequenceUtils::Where<int>(&seq, std::function<bool(int)>(Helpers::IsEven));

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// SegmentedList — граничные случаи сегментов

TEST(SegmentedListSegmentBoundaryTests, InsertInMiddleOfSegmentPreservesOrder)
{
    SegmentedList<int> list;
    for (int i = 0; i < 8; ++i) list.Append(i); 

    list.InsertAt(99, 4);

    EXPECT_EQ(list.GetLength(), 9);
    EXPECT_EQ(list.Get(3), 3);
    EXPECT_EQ(list.Get(4), 99);
    EXPECT_EQ(list.Get(5), 4);
    EXPECT_EQ(list.Get(8), 7);
}

TEST(SegmentedListSegmentBoundaryTests, PrependForcesNewSegmentWhenFull)
{
    SegmentedList<int> list;
    for (int i = 0; i < 8; ++i) list.Append(i); 

    list.Prepend(100);

    EXPECT_EQ(list.GetLength(), 9);
    EXPECT_EQ(list.GetFirst(), 100);
    EXPECT_EQ(list.Get(1), 0);
}

TEST(SegmentedListSegmentBoundaryTests, GetAcrossSegmentBoundaryIsCorrect)
{
    SegmentedList<int> list;
    for (int i = 0; i < 20; ++i) list.Append(i); 

    EXPECT_EQ(list.Get(7),  7);   
    EXPECT_EQ(list.Get(8),  8);   
    EXPECT_EQ(list.Get(15), 15);  
    EXPECT_EQ(list.Get(16), 16);  
    EXPECT_EQ(list.Get(19), 19);
}

TEST(SegmentedListSegmentBoundaryTests, SelfAssignmentIsValid)
{
    int items[] = { 1, 2, 3 };
    SegmentedList<int> list(items, 3);

    list = list;

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
}


// BitSequence — граничные случаи

TEST(BitSequenceExtendedTests, NOTOnAllZerosGivesAllOnes)
{
    int items[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    BitSequence<int> seq(items, 8);

    BitSequence<int>* result = seq.NOT();

    EXPECT_TRUE(TestUtils::CheckSequence({1, 1, 1, 1, 1, 1, 1, 1}, result, "NOT: invert all bits"));

    delete result;
}

TEST(BitSequenceExtendedTests, ANDWithSelfIsIdentity)
{
    int items[] = { 1, 0, 1, 1 };
    BitSequence<int> seq(items, 4);

    BitSequence<int>* result = seq.AND(&seq);

    EXPECT_TRUE(TestUtils::CheckSequence({1, 0, 1, 1}, result, "AND: with self"));

    delete result;
}

TEST(BitSequenceExtendedTests, ORWithZerosIsIdentity)
{
    int ones[] = { 1, 0, 1 };
    int zeros[] = { 0, 0, 0 };
    BitSequence<int> seq(ones, 3);
    BitSequence<int> zeroSeq(zeros, 3);

    BitSequence<int>* result = seq.OR(&zeroSeq);

    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 0);
    EXPECT_EQ(result->Get(2), 1);

    delete result;
}

TEST(BitSequenceExtendedTests, XORWithSelfIsAllZeros)
{
    int items[] = { 1, 0, 1, 1 };
    BitSequence<int> seq(items, 4);

    BitSequence<int>* result = seq.XOR(&seq);

    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(result->Get(i), 0);
    }

    delete result;
}

TEST(BitSequenceExtendedTests, SetChangesExistingBit)
{
    int items[] = { 0, 0, 0 };
    BitSequence<int> seq(items, 3);

    seq.Set(1, 1);

    EXPECT_EQ(seq.Get(0), 0);
    EXPECT_EQ(seq.Get(1), 1);
    EXPECT_EQ(seq.Get(2), 0);
}

TEST(BitSequenceExtendedTests, SetOutOfRangeThrows)
{
    int items[] = { 1, 0 };
    BitSequence<int> seq(items, 2);
    EXPECT_THROW(seq.Set(2, 1), OutOfRange);
}

TEST(BitSequenceExtendedTests, GetOnEmptyThrows)
{
    BitSequence<int> seq;
    EXPECT_THROW(seq.Get(0), OutOfRange);
}

TEST(BitSequenceExtendedTests, GetSubsequenceInvalidArgThrows)
{
    int items[] = { 1, 0, 1 };
    BitSequence<int> seq(items, 3);
    EXPECT_THROW(seq.GetSubsequence(0, 5), OutOfRange);
}

TEST(BitSequenceExtendedTests, SpliceRemovesAndInsertsBits)
{
    int items[] = { 1, 0, 1, 0, 1 };
    int ins[]   = { 0, 0 };
    BitSequence<int> seq(items, 5);
    BitSequence<int> insert(ins, 2);

    Sequence<int>* result = SequenceUtils::Splice<int>(&seq, 1, 2, &insert);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 0);
    EXPECT_EQ(result->Get(2), 0);
    EXPECT_EQ(result->Get(3), 0);
    EXPECT_EQ(result->Get(4), 1);

    delete result;
}


// AdaptiveSequence — дополнительные случаи

TEST(AdaptiveSequenceExtendedTests, PrependOnEmptyWorks)
{
    AdaptiveSequence<int> seq;
    seq.Prepend(1);

    EXPECT_EQ(seq.GetLength(), 1);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 1);
}

TEST(AdaptiveSequenceExtendedTests, GetSubsequenceOnSingleElement)
{
    int items[] = { 42 };
    AdaptiveSequence<int> seq(items, 1);

    Sequence<int>* sub = seq.GetSubsequence(0, 0);

    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 42);

    delete sub;
}

TEST(AdaptiveSequenceExtendedTests, SelfAssignmentIsValid)
{
    int items[] = { 1, 2, 3 };
    AdaptiveSequence<int> seq(items, 3);

    seq = seq;

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 2);
}


// SequenceUtils::Range

TEST(SequenceUtilsRangeTests, AscendingRangeWithStep1)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(1, 5, 1, (Sequence<int>*)&seq);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(4), 5);
}

TEST(SequenceUtilsRangeTests, AscendingRangeWithStep2)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(0, 8, 2, (Sequence<int>*)&seq);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 0);
    EXPECT_EQ(result->Get(2), 4);
    EXPECT_EQ(result->Get(4), 8);
}

TEST(SequenceUtilsRangeTests, DescendingRangeWithNegativeStep)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(5, 1, -1, (Sequence<int>*)&seq);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(4), 1);
}

TEST(SequenceUtilsRangeTests, SingleElementRange)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(7, 7, 1, (Sequence<int>*)&seq);

    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 7);
}

TEST(SequenceUtilsRangeTests, ZeroStepThrows)
{
    MutableArraySequence<int> seq;
    EXPECT_THROW(SequenceUtils::Range(0, 5, 0, (Sequence<int>*)&seq), InvalidArgument);
}

TEST(SequenceUtilsRangeTests, NullSequenceThrows)
{
    EXPECT_THROW(SequenceUtils::Range<int>(0, 5, 1, nullptr), InvalidArgument);
}


// SequenceUtils::Zip

TEST(SequenceUtilsZipTests, ZipIteratesPairsUntilShorterExhausted)
{
    int a[] = { 1, 2, 3 };
    int b[] = { 10, 20 };
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<int> seqB(b, 2);

    SequenceUtils::ZipEnumerator<int, int>* zip = SequenceUtils::Zip<int, int>(&seqA, &seqB);

    EXPECT_TRUE(zip->MoveNext());
    auto p1 = zip->GetCurrent();
    EXPECT_EQ(p1.first, 1);
    EXPECT_EQ(p1.second, 10);

    EXPECT_TRUE(zip->MoveNext());
    auto p2 = zip->GetCurrent();
    EXPECT_EQ(p2.first, 2);
    EXPECT_EQ(p2.second, 20);

    EXPECT_FALSE(zip->MoveNext());

    delete zip;
}

TEST(SequenceUtilsZipTests, ZipOnEmptySequenceReturnsFalseImmediately)
{
    MutableArraySequence<int> empty;
    int b[] = { 1, 2 };
    MutableArraySequence<int> seqB(b, 2);

    SequenceUtils::ZipEnumerator<int, int>* zip = SequenceUtils::Zip<int, int>(&empty, &seqB);

    EXPECT_FALSE(zip->MoveNext());

    delete zip;
}

TEST(SequenceUtilsZipTests, ZipWithNullThrows)
{
    MutableArraySequence<int> seq;
    // Оборачиваем в лямбду для защиты от запятых внутри макроса
    auto action1 = [&]() { SequenceUtils::Zip<int, int>((Sequence<int>*)nullptr, &seq); };
    auto action2 = [&]() { SequenceUtils::Zip<int, int>(&seq, (Sequence<int>*)nullptr); };
    
    EXPECT_THROW(action1(), InvalidArgument);
    EXPECT_THROW(action2(), InvalidArgument);
}


// SequenceUtils::Unzip

TEST(SequenceUtilsUnzipTests, UnzipSplitsPairsIntoTwoSequences)
{
    Pair<int,int> pairs[] = { {1, 10}, {2, 20}, {3, 30} };
    MutableArraySequence<Pair<int,int>> pairsSeq(pairs, 3);

    MutableArraySequence<int> out1;
    MutableArraySequence<int> out2;

    auto result = SequenceUtils::Unzip<int, int>(&pairsSeq, &out1, &out2);

    EXPECT_EQ(result.first->GetLength(), 3);
    EXPECT_EQ(result.second->GetLength(), 3);

    EXPECT_EQ(result.first->Get(0), 1);
    EXPECT_EQ(result.first->Get(2), 3);
    EXPECT_EQ(result.second->Get(0), 10);
    EXPECT_EQ(result.second->Get(2), 30);
}

TEST(SequenceUtilsUnzipTests, UnzipWithNullThrows)
{
    MutableArraySequence<Pair<int,int>> pairsSeq;
    MutableArraySequence<int> out;

    // Используем лямбды, чтобы макрос EXPECT_THROW не сломался от запятых
    auto action1 = [&]() { SequenceUtils::Unzip<int, int>(nullptr, &out, &out); };
    auto action2 = [&]() { SequenceUtils::Unzip<int, int>(&pairsSeq, nullptr, &out); };
    auto action3 = [&]() { SequenceUtils::Unzip<int, int>(&pairsSeq, &out, nullptr); };

    EXPECT_THROW(action1(), InvalidArgument);
    EXPECT_THROW(action2(), InvalidArgument);
    EXPECT_THROW(action3(), InvalidArgument);
}


// SequenceUtils::Split

static bool IsZero(int x) { return x == 0; }

TEST(SequenceUtilsSplitTests, SplitByZeroProducesSegmentsBetweenDelimiters)
{
    int items[] = { 1, 2, 0, 3, 4, 0, 5 };
    MutableArraySequence<int> seq(items, 7);
    MutableArraySequence<int> proto;

    SequenceUtils::SplitEnumerator<int>* split = SequenceUtils::Split<int>(&seq, std::function<bool(int)>(IsZero), &proto);

    EXPECT_TRUE(split->MoveNext());
    Sequence<int>* seg1 = split->GetCurrent();
    EXPECT_EQ(seg1->GetLength(), 2);
    EXPECT_EQ(seg1->Get(0), 1);
    EXPECT_EQ(seg1->Get(1), 2);

    EXPECT_TRUE(split->MoveNext());
    Sequence<int>* seg2 = split->GetCurrent();
    EXPECT_EQ(seg2->GetLength(), 2);
    EXPECT_EQ(seg2->Get(0), 3);

    EXPECT_TRUE(split->MoveNext());
    Sequence<int>* seg3 = split->GetCurrent();
    EXPECT_EQ(seg3->GetLength(), 1);
    EXPECT_EQ(seg3->Get(0), 5);

    EXPECT_FALSE(split->MoveNext());

    delete split;
}

TEST(SequenceUtilsSplitTests, SplitWithNullThrows)
{
    MutableArraySequence<int> seq;
    MutableArraySequence<int> proto;
    std::function<bool(int)> pred = IsZero;

    EXPECT_THROW(SequenceUtils::Split<int>(nullptr, pred, &proto), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Split<int>(&seq, nullptr, &proto), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Split<int>(&seq, pred, nullptr), InvalidArgument);
}


// Одноэлементные граничные случаи для всех контейнеров

TEST(SingleElementEdgeCaseTests, ArraySequencePrependOnSingleElement)
{
    MutableArraySequence<int> seq;
    seq.Prepend(5);
    EXPECT_EQ(seq.GetLength(), 1);
    EXPECT_EQ(seq.GetFirst(), 5);
    EXPECT_EQ(seq.GetLast(), 5);
}

TEST(SingleElementEdgeCaseTests, ListSequenceInsertAtEndOnSingleElement)
{
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.InsertAt(2, 1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetLast(), 2);
}

TEST(SingleElementEdgeCaseTests, SegmentedListGetSubsequenceOfWholeList)
{
    int items[] = { 42 };
    SegmentedList<int> list(items, 1);

    Sequence<int>* sub = list.GetSubsequence(0, 0);
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 42);

    delete sub;
}

TEST(SingleElementEdgeCaseTests, BitSequenceOfOneBitNOT)
{
    int items[] = { 1 };
    BitSequence<int> seq(items, 1);

    BitSequence<int>* result = seq.NOT();
    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 0);

    delete result;
}