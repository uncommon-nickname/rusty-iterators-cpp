#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestFilterIterator, NextFrontChecksIfValid)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.nextFront(), 2);
    ASSERT_EQ(it.nextFront(), std::nullopt);
}

TEST(TestFilterIterator, NextBackChecksIfValid)
{
    auto vec = std::vector{1, 3, 4};
    auto it  = RustyIter{vec}.filter([](auto x) { return x % 2 == 0; });

    ASSERT_EQ(it.nextBack(), 4);
    ASSERT_EQ(it.nextBack(), std::nullopt);
}

TEST(TestFilterIterator, CollectedValuesAreFiltered)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec}.filter([](auto x) { return x % 2 == 0; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{2, 4}));
}
