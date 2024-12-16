#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestMapIterator, NextAppliesCallable)
{
    auto vec = std::vector{1, 2};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 4);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestMapIterator, CollectedValuesHaveCallableApplied)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 4, 9, 16}));
}

TEST(TestMapIterator, SizeHintReturnsSizeOfUnderlyingIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestMapIterator, CountReturnsTheAmountOfElements)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    ASSERT_EQ(it.count(), 3);
}
