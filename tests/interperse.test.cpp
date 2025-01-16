#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestInterperseIterator, TestNextReturnsInterpersed)
{
    auto vec = std::vector{1, 2};

    auto item = 5;
    auto it   = LazyIterator{vec}.interperse(std::cref(item));

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 5);
    ASSERT_EQ(it.next(), 2);
    ASSERT_EQ(it.next(), 5);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestInterperseIterator, TestCollectReturnsAllValues)
{
    auto vec = std::vector{1, 2, 3};

    auto item = 5;
    auto it   = LazyIterator{vec}.interperse(std::cref(item));

    EXPECT_THAT(it.collect(), ElementsAreArray({1, 5, 2, 5, 3, 5}));
}

TEST(TestInterperseIterator, TestSizeHintIsDoubled)
{
    auto vec = std::vector{1, 2, 3};

    auto item = 5;
    auto it   = LazyIterator{vec}.interperse(std::cref(item));

    ASSERT_EQ(it.sizeHint(), 6);
}
