#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestMapIterator, NextFrontAppliesCallable)
{
    auto vec = std::vector{1, 2};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    ASSERT_EQ(it.nextFront(), 1);
    ASSERT_EQ(it.nextFront(), 4);
    ASSERT_EQ(it.nextFront(), std::nullopt);
}

TEST(TestMapIterator, NextBackAppliesCallable)
{
    auto vec = std::vector{1, 2};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    ASSERT_EQ(it.nextBack(), 4);
    ASSERT_EQ(it.nextBack(), 1);
    ASSERT_EQ(it.nextBack(), std::nullopt);
}

TEST(TestMapIterator, CollectedValuesHaveCallableApplied)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec}.map([](auto x) { return x * x; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 4, 9, 16}));
}
