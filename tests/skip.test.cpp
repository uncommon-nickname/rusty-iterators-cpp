#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestSkipIterator, NextSkipsElements)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.skip(3);

    ASSERT_EQ(it.next(), 4);
    ASSERT_EQ(it.next(), 5);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestSkipIterator, NextOnTooSmallIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.skip(5);

    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestSkipIterator, CollectAllItemsExceptSkippedOnes)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.skip(3);

    EXPECT_THAT(it.collect(), ElementsAreArray({4, 5}));
}

TEST(TestSkipIterator, SizeHintGivesEstimatedSize)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto it  = LazyIterator{vec}.skip(3);

    ASSERT_EQ(it.sizeHint(), 2);
}

TEST(TestSkipIterator, SizeHintOnInfiniteIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle().skip(3);

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestSkipIterator, TestSizeHintOnTooSmallIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.skip(10);

    ASSERT_EQ(it.sizeHint(), 0);
}
