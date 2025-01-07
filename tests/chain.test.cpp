#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestChainIterator, TestCollect)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = LazyIterator{v1}.chain(LazyIterator{v2});

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4, 5, 6}));
}

TEST(TestChainIterator, TestSizeHintWhenOneIsInf)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = LazyIterator{v1}.chain(LazyIterator{v2}.cycle());

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestChainIterator, TestSizeHintSums)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = LazyIterator{v1}.chain(LazyIterator{v2});

    ASSERT_EQ(it.sizeHint().value(), 6);
}

TEST(TestChainIterator, TestOneIteratorEmpty)
{
    auto v1 = std::vector<int>{};
    auto v2 = std::vector{4, 5, 6};

    auto it = LazyIterator{v1}.chain(LazyIterator{v2});

    ASSERT_EQ(it.next().value(), 4);
}

TEST(TestChainIterator, TestIteratorCount)
{
    auto v1 = std::vector{1, 2};
    auto v2 = std::vector{3, 5, 6};

    auto it = LazyIterator{v1}.chain(LazyIterator{v2});

    ASSERT_EQ(it.count(), 5);
}
