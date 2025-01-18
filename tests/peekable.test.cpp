#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestPeekableIterator, TestNextReturnsNextItem)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 2);
    ASSERT_EQ(it.next(), 3);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestPeekableIterator, TestPeekGivesNextItem)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    ASSERT_EQ(it.peek(), 1);
    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.peek(), 2);
    ASSERT_EQ(it.next(), 2);
}

TEST(TestPeekableIterator, TestPeekMultipleTimes)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    ASSERT_EQ(it.peek(), 1);
    ASSERT_EQ(it.peek(), 1);
    ASSERT_EQ(it.peek(), 1);
    ASSERT_EQ(it.next(), 1);
}

TEST(TestPeekableIterator, TestCollectPeekedIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    it.peek();

    EXPECT_THAT(it.collect(), ElementsAreArray({1, 2, 3}));
}

TEST(TestPeekableIterator, TestSizeHintOnNotPeeked)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestPeekableIterator, TestSizeHinttPeeked)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.peekable();

    it.peek();

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestPeekableIterator, TestSizeHintOnPeekedInfinite)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle().peekable();

    it.peek();

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}
