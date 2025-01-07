#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>
#include <stdexcept>

using ::rusty_iterators::iterator::CycleType;
using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestCycleIterator, NextReturnsCycledItems)
{
    auto vec = std::vector{1, 2};
    auto it  = LazyIterator{vec}.cycle();

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 2);
    ASSERT_EQ(it.next(), 1);
}

TEST(TestCycleIterator, NextOnEmptyCycleReturnsNull)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec}.cycle();

    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestCycleIterator, SizeHintReturnsNull)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestCycleIterator, SizeHintReturnsZero)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec}.cycle();

    ASSERT_EQ(it.sizeHint(), 0);
}

TEST(TestCycleIterator, CollectThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.collect(), std::length_error);
}

TEST(TestCycleIterator, CollectEmptyCycle)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array<int, 0>{}));
}

TEST(TestCycleIterator, CountThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.count(), std::length_error);
}

TEST(TestCycleIterator, CountEmptyCycle)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec}.cycle();

    ASSERT_EQ(it.count(), 0);
}

TEST(TestCycleIterator, FoldThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.fold(0, [](auto x, auto y) { return x + y; }), std::length_error);
}

TEST(TestCycleIterator, ForEachThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(it.forEach([](auto x) { auto _ = x * x; }), std::length_error);
}

TEST(TestCycleIterator, MaxThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.max(), std::length_error);
}

TEST(TestCycleIterator, MinThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.min(), std::length_error);
}

TEST(TestCycleIterator, SumThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.map([](auto x) { return x * x; }).cycle();

    EXPECT_THROW(auto _ = it.sum(), std::length_error);
}

TEST(TestCycleIterator, ReduceThrowsException)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle();

    EXPECT_THROW(auto _ = it.reduce([](auto x, auto y) { return x; }), std::length_error);
}

TEST(TestCycleIterator, CycleSavesTheCurrentState)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    it.next();

    auto c = it.cycle();

    ASSERT_EQ(c.next(), 2);
    ASSERT_EQ(c.next(), 3);
    ASSERT_EQ(c.next(), 2);
}

TEST(TestCycleIterator, ConstructTheCacheCycle)
{
    auto vec = std::vector{1, 2};
    auto it  = LazyIterator{vec}.cycle<CycleType::Cache>();

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 2);
    ASSERT_EQ(it.next(), 1);
}
