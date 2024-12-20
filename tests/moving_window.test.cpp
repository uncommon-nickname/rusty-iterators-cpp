#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>
#include <stdexcept>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestMovingWindowIterator, TestCollectWindows)
{
    auto vec    = std::vector{1, 2, 3, 4};
    auto result = RustyIter{vec}.movingWindow(2).collect();

    ASSERT_EQ(result.size(), 3);
    EXPECT_THAT(result[0], ElementsAreArray(std::array{1, 2}));
    EXPECT_THAT(result[1], ElementsAreArray(std::array{2, 3}));
    EXPECT_THAT(result[2], ElementsAreArray(std::array{3, 4}));
}

TEST(TestMovingWindowIterator, TestWindowSizeBiggerThanIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.movingWindow(4);

    ASSERT_TRUE(it.collect().empty());
}

TEST(TestMovingWindowIterator, TestZeroSizeWindow)
{
    auto vec = std::vector{1, 2, 3};

    EXPECT_THROW(auto _ = RustyIter{vec}.movingWindow(0), std::length_error);
}

TEST(TestMovingWindowIterator, TestMovingWindowCount)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec}.movingWindow(2);

    ASSERT_EQ(it.count(), vec.size() - 1);
}
