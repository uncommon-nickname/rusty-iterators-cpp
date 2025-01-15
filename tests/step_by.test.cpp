#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>
#include <stdexcept>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestStepByIterator, ThrowWhenSizeIsZero)
{
    auto vec = std::vector{1, 2, 3};

    EXPECT_THROW(auto _ = LazyIterator{vec}.stepBy(0), std::length_error);
}

TEST(TestStepByIterator, NextReturnsFirstElement)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.stepBy(2);

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 3);
    ASSERT_EQ(it.next(), 5);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestStepByIterator, NextWhenStepLargerThanIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.stepBy(10);

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestStepByIterator, TestCollectIterator)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.stepBy(2);

    EXPECT_THAT(it.collect(), ElementsAreArray({1, 3, 5}));
}

TEST(TestStepByIterator, TestSizeHintWhenUnderlyingInfinite)
{
    auto vec = std::vector{1, 2};
    auto it  = LazyIterator{vec}.cycle().stepBy(3);

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestStepByIterator, TestSizeHintFirstTake)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.stepBy(2);

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestStepByIterator, TestSizeHintOtherTake)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.stepBy(2);

    it.next();

    ASSERT_EQ(it.sizeHint(), 2);
}
