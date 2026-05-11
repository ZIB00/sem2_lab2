#include <gtest/gtest.h>

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


// Вспомогательные функции

namespace Helpers
{
    int Double(int x)         { return x * 2; }
    bool IsEven(int x)        { return x % 2 == 0; }
    bool IsPositive(int x)    { return x > 0; }
    int Sum(int a, int b)     { return a + b; }
    int ToAbsolute(int x)     { return x < 0 ? -x : x; }

    // FlatMap: каждый элемент разворачивается в [x, x+1]
    Sequence<int>* PairExpand(int x)
    {
        int items[] = { x, x + 1 };
        return new MutableArraySequence<int>(items, 2);
    }

    // FlatMap для ListSequence — возвращает ListSequence
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

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);

    delete result;
}

TEST(MutableArraySequenceExtendedTests, PlusOperatorWithEmptyRight)
{
    int a[] = { 1, 2, 3 };
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<int> empty;

    Sequence<int>* result = seqA + &empty;

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 1);

    delete result;
}

TEST(MutableArraySequenceExtendedTests, PlusOperatorWithEmptyLeft)
{
    int b[] = { 1, 2 };
    MutableArraySequence<int> empty;
    MutableArraySequence<int> seqB(b, 2);

    Sequence<int>* result = empty + &seqB;

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 1);

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

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(2), 3);

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

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(3), 4);

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
    // AdaptiveSequence::operator[] делегирует во внутренний Sequence<T>*,
    // у которого operator[] не объявлен — тест через Get()
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
    EXPECT_EQ(opt.GetValue(), 42);
}

TEST(OptionTests, GetValueOnNoneThrows)
{
    Option<int> opt;
    EXPECT_THROW(opt.GetValue(), LogicError);
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

TEST(OptionTests, MapTransformsSomeValue)
{
    Option<int> opt(5);
    Option<double> result = opt.Map(Helpers::AsDouble);
    EXPECT_TRUE(result.HasValue());
    EXPECT_DOUBLE_EQ(result.GetValue(), 5.0);
}

TEST(OptionTests, MapOnNoneStaysNone)
{
    Option<int> none;
    Option<double> result = none.Map(Helpers::AsDouble);
    EXPECT_FALSE(result.HasValue());
}

TEST(OptionTests, FlatMapReturnsSomeWhenFunctionSucceeds)
{
    Option<int> opt(4);
    Option<double> result = opt.FlatMap(Helpers::SafeReciprocal);
    EXPECT_TRUE(result.HasValue());
    EXPECT_DOUBLE_EQ(result.GetValue(), 0.25);
}

TEST(OptionTests, FlatMapReturnsNoneWhenFunctionReturnsNone)
{
    Option<int> opt(0);
    Option<double> result = opt.FlatMap(Helpers::SafeReciprocal);
    EXPECT_FALSE(result.HasValue());
}

TEST(OptionTests, FlatMapOnNoneStaysNone)
{
    Option<int> none;
    Option<double> result = none.FlatMap(Helpers::SafeReciprocal);
    EXPECT_FALSE(result.HasValue());
}


// GetFirst(predicate) / GetLast(predicate) — ArraySequence


TEST(ArraySequenceOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 3, 4, 6, 7 };
    MutableArraySequence<int> seq(items, 5);

    Option<int> result = seq.GetFirst(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 4);
}

TEST(ArraySequenceOptionTests, GetFirstWithPredicateReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Option<int> result = seq.GetFirst(Helpers::IsEven);
    EXPECT_FALSE(result.HasValue());
}

TEST(ArraySequenceOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 2, 4, 3, 6, 7 };
    MutableArraySequence<int> seq(items, 5);

    Option<int> result = seq.GetLast(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 6);
}

TEST(ArraySequenceOptionTests, GetLastWithPredicateReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Option<int> result = seq.GetLast(Helpers::IsEven);
    EXPECT_FALSE(result.HasValue());
}

TEST(ArraySequenceOptionTests, GetFirstOnEmptySequenceReturnsNone)
{
    MutableArraySequence<int> seq;
    Option<int> result = seq.GetFirst(Helpers::IsEven);
    EXPECT_FALSE(result.HasValue());
}


// GetFirst(predicate) / GetLast(predicate) — ListSequence


TEST(ListSequenceOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 3, 4, 5 };
    MutableListSequence<int> seq(items, 4);

    Option<int> result = seq.GetFirst(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 4);
}

TEST(ListSequenceOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 2, 5, 8, 9 };
    MutableListSequence<int> seq(items, 4);

    Option<int> result = seq.GetLast(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 8);
}


// GetFirst(predicate) / GetLast(predicate) — SegmentedList


TEST(SegmentedListOptionTests, GetFirstWithPredicateFindsMatch)
{
    int items[] = { 1, 2, 3, 4 };
    SegmentedList<int> list(items, 4);

    Option<int> result = list.GetFirst(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 2);
}

TEST(SegmentedListOptionTests, GetLastWithPredicateFindsLastMatch)
{
    int items[] = { 1, 2, 3, 4 };
    SegmentedList<int> list(items, 4);

    Option<int> result = list.GetLast(Helpers::IsEven);
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.GetValue(), 4);
}

TEST(SegmentedListOptionTests, GetFirstReturnsNoneWhenNoMatch)
{
    int items[] = { 1, 3, 5 };
    SegmentedList<int> list(items, 3);

    Option<int> result = list.GetFirst(Helpers::IsEven);
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


// Skip — ArraySequence


TEST(ArraySequenceSkipTests, SkipZeroReturnsWholeSequence)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.Skip(0);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 1);

    delete result;
}

TEST(ArraySequenceSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 1, 2, 3, 4, 5 };
    MutableArraySequence<int> seq(items, 5);

    Sequence<int>* result = seq.Skip(2);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 3);
    EXPECT_EQ(result->Get(2), 5);

    delete result;
}

TEST(ArraySequenceSkipTests, SkipAllReturnsEmpty)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.Skip(3);

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}

TEST(ArraySequenceSkipTests, SkipMoreThanLengthReturnsEmpty)
{
    int items[] = { 1, 2 };
    MutableArraySequence<int> seq(items, 2);

    Sequence<int>* result = seq.Skip(100);

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// Skip — ListSequence


TEST(ListSequenceSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 10, 20, 30, 40 };
    MutableListSequence<int> seq(items, 4);

    Sequence<int>* result = seq.Skip(2);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 30);

    delete result;
}

TEST(ListSequenceSkipTests, SkipAllOrMoreReturnsEmpty)
{
    int items[] = { 1, 2 };
    MutableListSequence<int> seq(items, 2);

    Sequence<int>* result = seq.Skip(5);
    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// Skip — SegmentedList


TEST(SegmentedListSkipTests, SkipSomeReturnsRemainder)
{
    int items[] = { 1, 2, 3, 4, 5 };
    SegmentedList<int> list(items, 5);

    Sequence<int>* result = list.Skip(3);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 4);

    delete result;
}


// Splice — ArraySequence


TEST(ArraySequenceSpliceTests, SpliceRemovesAndInserts)
{
    int items[] = { 1, 2, 3, 4, 5 };
    int ins[]   = { 9, 10 };
    MutableArraySequence<int> seq(items, 5);
    MutableArraySequence<int> insert(ins, 2);

    // Удалить 2 элемента с позиции 1, вставить [9, 10]
    // Ожидаем: {1, 9, 10, 4, 5}
    Sequence<int>* result = seq.Splice(1, 2, &insert);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 9);
    EXPECT_EQ(result->Get(2), 10);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceWithNullInsertJustDeletes)
{
    int items[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(items, 4);

    // Удалить 2 элемента с позиции 1, ничего не вставлять
    // Ожидаем: {1, 4}
    Sequence<int>* result = seq.Splice(1, 2, nullptr);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 4);

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceAtStartRemovesPrefix)
{
    int items[] = { 1, 2, 3, 4 };
    MutableArraySequence<int> seq(items, 4);

    Sequence<int>* result = seq.Splice(0, 2, nullptr);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 3);

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceAtEndRemovesSuffix)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.Splice(2, 1, nullptr);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(1), 2);

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceCountExceedingRemainingClampsToEnd)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    // Запросить удаление 100 элементов с позиции 1 — должно удалить только 2
    Sequence<int>* result = seq.Splice(1, 100, nullptr);

    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 1);

    delete result;
}

TEST(ArraySequenceSpliceTests, SpliceWithOutOfBoundsIndexThrows)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    EXPECT_THROW(seq.Splice(10, 1, nullptr), OutOfRange);
}


// Splice — ListSequence


TEST(ListSequenceSpliceTests, SpliceRemovesAndInserts)
{
    int items[] = { 1, 2, 3, 4, 5 };
    int ins[]   = { 9, 10 };
    MutableListSequence<int> seq(items, 5);
    MutableListSequence<int> insert(ins, 2);

    Sequence<int>* result = seq.Splice(1, 2, &insert);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(1), 9);
    EXPECT_EQ(result->Get(2), 10);

    delete result;
}

TEST(ListSequenceSpliceTests, SpliceWithNullInsertJustDeletes)
{
    int items[] = { 1, 2, 3 };
    MutableListSequence<int> seq(items, 3);

    Sequence<int>* result = seq.Splice(0, 1, nullptr);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 2);

    delete result;
}


// FlatMap — ArraySequence


TEST(ArraySequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 1, 2, 3 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.FlatMap(Helpers::PairExpand);

    // {1,2} + {2,3} + {3,4} = {1,2,2,3,3,4}
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

    Sequence<int>* result = seq.FlatMap(Helpers::PairExpand);

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// FlatMap — ListSequence


TEST(ListSequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 10, 20 };
    MutableListSequence<int> seq(items, 2);

    Sequence<int>* result = seq.FlatMap(Helpers::PairExpandList);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 11);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 21);

    delete result;
}


// FlatMap — SegmentedList


TEST(SegmentedListFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 1, 2 };
    SegmentedList<int> list(items, 2);

    Sequence<int>* result = list.FlatMap(Helpers::PairExpand);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(2), 2);

    delete result;
}


// FlatMap — AdaptiveSequence


TEST(AdaptiveSequenceFlatMapTests, FlatMapExpandsEachElement)
{
    int items[] = { 3, 4 };
    AdaptiveSequence<int> seq(items, 2);

    Sequence<int>* result = seq.FlatMap(Helpers::PairExpand);

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

    EXPECT_EQ(arrSeq.Reduce(Helpers::Sum), 42);
    EXPECT_EQ(listSeq.Reduce(Helpers::Sum), 42);
    EXPECT_EQ(segList.Reduce(Helpers::Sum), 42);
    EXPECT_EQ(adaptSeq.Reduce(Helpers::Sum), 42);
}

TEST(ReduceEdgeCaseTests, ReduceOnEmptySequenceThrows)
{
    MutableArraySequence<int> arrSeq;
    MutableListSequence<int> listSeq;
    SegmentedList<int> segList;
    AdaptiveSequence<int> adaptSeq;

    EXPECT_THROW(arrSeq.Reduce(Helpers::Sum), OutOfRange);
    EXPECT_THROW(listSeq.Reduce(Helpers::Sum), OutOfRange);
    EXPECT_THROW(segList.Reduce(Helpers::Sum), OutOfRange);
    EXPECT_THROW(adaptSeq.Reduce(Helpers::Sum), OutOfRange);
}


// Where на последовательности, где все / ни один не проходит фильтр


TEST(WhereEdgeCaseTests, WhereAllMatchReturnsFullCopy)
{
    int items[] = { 2, 4, 6 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.Where(Helpers::IsEven);

    EXPECT_EQ(result->GetLength(), 3);

    delete result;
}

TEST(WhereEdgeCaseTests, WhereNoneMatchReturnsEmpty)
{
    int items[] = { 1, 3, 5 };
    MutableArraySequence<int> seq(items, 3);

    Sequence<int>* result = seq.Where(Helpers::IsEven);

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}


// SegmentedList — граничные случаи сегментов


TEST(SegmentedListSegmentBoundaryTests, InsertInMiddleOfSegmentPreservesOrder)
{
    // SEGMENT_SIZE = 8, заполняем ровно 8 элементов, затем вставляем в середину
    SegmentedList<int> list;
    for (int i = 0; i < 8; ++i) list.Append(i); // 0..7

    list.InsertAt(99, 4); // должно быть 0,1,2,3,99,4,5,6,7

    EXPECT_EQ(list.GetLength(), 9);
    EXPECT_EQ(list.Get(3), 3);
    EXPECT_EQ(list.Get(4), 99);
    EXPECT_EQ(list.Get(5), 4);
    EXPECT_EQ(list.Get(8), 7);
}

TEST(SegmentedListSegmentBoundaryTests, PrependForcesNewSegmentWhenFull)
{
    SegmentedList<int> list;
    for (int i = 0; i < 8; ++i) list.Append(i); // полный первый сегмент

    list.Prepend(100); // должен создать новый сегмент спереди

    EXPECT_EQ(list.GetLength(), 9);
    EXPECT_EQ(list.GetFirst(), 100);
    EXPECT_EQ(list.Get(1), 0);
}

TEST(SegmentedListSegmentBoundaryTests, GetAcrossSegmentBoundaryIsCorrect)
{
    SegmentedList<int> list;
    for (int i = 0; i < 20; ++i) list.Append(i); // span 3 segments (8+8+4)

    EXPECT_EQ(list.Get(7),  7);   // последний в первом сегменте
    EXPECT_EQ(list.Get(8),  8);   // первый во втором сегменте
    EXPECT_EQ(list.Get(15), 15);  // последний во втором сегменте
    EXPECT_EQ(list.Get(16), 16);  // первый в третьем сегменте
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
    int items[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // ровно 1 байт
    BitSequence<int> seq(items, 8);

    BitSequence<int>* result = seq.NOT();

    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(result->Get(i), 1) << "Bit " << i << " should be 1";
    }

    delete result;
}

TEST(BitSequenceExtendedTests, ANDWithSelfIsIdentity)
{
    int items[] = { 1, 0, 1, 1 };
    BitSequence<int> seq(items, 4);

    BitSequence<int>* result = seq.AND(&seq);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 0);
    EXPECT_EQ(result->Get(2), 1);
    EXPECT_EQ(result->Get(3), 1);

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
        EXPECT_EQ(result->Get(i), 0) << "Bit " << i << " should be 0";
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

    // Удалить 2 бита с позиции 1, вставить [0,0] → {1,0,0,0,1}
    Sequence<int>* result = seq.Splice(1, 2, &insert);

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
    Sequence<int>* result = SequenceUtils::Range(1, 5, 1, &seq);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(4), 5);
}

TEST(SequenceUtilsRangeTests, AscendingRangeWithStep2)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(0, 8, 2, &seq);

    // 0, 2, 4, 6, 8 — 5 элементов
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 0);
    EXPECT_EQ(result->Get(2), 4);
    EXPECT_EQ(result->Get(4), 8);
}

TEST(SequenceUtilsRangeTests, DescendingRangeWithNegativeStep)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(5, 1, -1, &seq);

    // 5, 4, 3, 2, 1
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(4), 1);
}

TEST(SequenceUtilsRangeTests, SingleElementRange)
{
    MutableArraySequence<int> seq;
    Sequence<int>* result = SequenceUtils::Range(7, 7, 1, &seq);

    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 7);
}

TEST(SequenceUtilsRangeTests, ZeroStepThrows)
{
    MutableArraySequence<int> seq;
    EXPECT_THROW(SequenceUtils::Range(0, 5, 0, &seq), InvalidArgument);
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

    SequenceUtils::ZipEnumerator<int>* zip = SequenceUtils::Zip<int>(&seqA, &seqB);

    EXPECT_TRUE(zip->MoveNext());
    auto p1 = zip->GetCurrent();
    EXPECT_EQ(p1.first, 1);
    EXPECT_EQ(p1.second, 10);

    EXPECT_TRUE(zip->MoveNext());
    auto p2 = zip->GetCurrent();
    EXPECT_EQ(p2.first, 2);
    EXPECT_EQ(p2.second, 20);

    EXPECT_FALSE(zip->MoveNext()); // seqB исчерпан

    delete zip;
}

TEST(SequenceUtilsZipTests, ZipOnEmptySequenceReturnsFalseImmediately)
{
    MutableArraySequence<int> empty;
    int b[] = { 1, 2 };
    MutableArraySequence<int> seqB(b, 2);

    SequenceUtils::ZipEnumerator<int>* zip = SequenceUtils::Zip<int>(&empty, &seqB);

    EXPECT_FALSE(zip->MoveNext());

    delete zip;
}

TEST(SequenceUtilsZipTests, ZipWithNullThrows)
{
    MutableArraySequence<int> seq;
    EXPECT_THROW(SequenceUtils::Zip<int>(nullptr, &seq), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Zip<int>(&seq, nullptr), InvalidArgument);
}


// SequenceUtils::Unzip


TEST(SequenceUtilsUnzipTests, UnzipSplitsPairsIntoTwoSequences)
{
    // Строим Sequence<Pair<int,int>>
    Pair<int,int> pairs[] = { {1, 10}, {2, 20}, {3, 30} };
    MutableArraySequence<Pair<int,int>> pairsSeq(pairs, 3);

    MutableArraySequence<int> out1;
    MutableArraySequence<int> out2;

    auto result = SequenceUtils::Unzip<int>(&pairsSeq, &out1, &out2);

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

    EXPECT_THROW(SequenceUtils::Unzip<int>(nullptr, &out, &out), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Unzip<int>(&pairsSeq, nullptr, &out), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Unzip<int>(&pairsSeq, &out, nullptr), InvalidArgument);
}


// SequenceUtils::Split


static bool IsZero(int x) { return x == 0; }

TEST(SequenceUtilsSplitTests, SplitByZeroProducesSegmentsBetweenDelimiters)
{
    int items[] = { 1, 2, 0, 3, 4, 0, 5 };
    MutableArraySequence<int> seq(items, 7);
    MutableArraySequence<int> proto;

    SequenceUtils::SplitEnumerator<int>* split = SequenceUtils::Split<int>(&seq, IsZero, &proto);

    // Ожидаем три сегмента: [1,2], [3,4], [5]
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

    EXPECT_THROW(SequenceUtils::Split<int>(nullptr, IsZero, &proto), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Split<int>(&seq, nullptr, &proto), InvalidArgument);
    EXPECT_THROW(SequenceUtils::Split<int>(&seq, IsZero, nullptr), InvalidArgument);
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
