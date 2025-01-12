#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::FieldsAre;

TEST(TestEnumerateIterator, TestNextReturnsIndexedValue)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.enumerate();

    EXPECT_THAT(it.next().value(), FieldsAre(0, 1));
    EXPECT_THAT(it.next().value(), FieldsAre(1, 2));
    EXPECT_THAT(it.next().value(), FieldsAre(2, 3));
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestEnumerateIterator, TestCollectedTuples)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.enumerate().collect();

    ASSERT_EQ(result.size(), 3);
    EXPECT_THAT(result[0], FieldsAre(0, 1));
    EXPECT_THAT(result[1], FieldsAre(1, 2));
    EXPECT_THAT(result[2], FieldsAre(2, 3));
}

TEST(TestEnumerateIterator, TestSizeHintReturnsUnderlyingSize)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.enumerate();

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestEnumerateIterator, TestCountReturnsRealCount)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.enumerate();

    it.next();

    ASSERT_EQ(it.count(), 2);
}
