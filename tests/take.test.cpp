#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestTakeIterator, CollectOnlyTakenItems)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.take(2);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2}));
}

TEST(TestTakeIterator, SizeHintReturnsTakeSize)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.take(2);

    ASSERT_EQ(it.sizeHint(), 2);
}

TEST(TestTakeIterator, SizeHintReturnsSmallerSize)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.take(4);

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestTakeIterator, SizeHintOnEmptyIterator)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec}.take(3);

    ASSERT_EQ(it.sizeHint(), 0);
}

TEST(TestTakeIterator, CountWorksOnlyOnTakeItems)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.take(2);

    ASSERT_EQ(it.count(), 2);
}
