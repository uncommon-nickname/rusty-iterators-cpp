#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestFilterIterator, NextChecksIfValid)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.next(), 2);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestFilterIterator, CollectedValuesAreFiltered)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{2, 4}));
}

TEST(TestFilterIterator, SizeHintReturnsSizeOfUnderlyingIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestFilterIterator, CountReturnsRealAmountOfElements)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.count(), 2);
}

TEST(TestFilterIterator, MaxDoesNotReceiveFilteredOutItems)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 != 0; });

    ASSERT_EQ(it.max(), 3);
}

TEST(TestFilterIterator, MinDoesNotReceiveFilteredOutItems)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.min(), 2);
}
