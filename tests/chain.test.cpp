#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestChainIterator, TestCollect)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = RustyIter{v1}.chain(RustyIter{v2});

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4, 5, 6}));
}

TEST(TestChainIterator, TestSizeHintWhenOneIsInf)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = RustyIter{v1}.chain(RustyIter{v2}.cycle());

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestChainIterator, TestSizeHintSums)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto it = RustyIter{v1}.chain(RustyIter{v2});

    ASSERT_EQ(it.sizeHint().value(), 6);
}

TEST(TestChainIterator, TestOneIteratorEmpty)
{
    auto v1 = std::vector<int>{};
    auto v2 = std::vector{4, 5, 6};

    auto it = RustyIter{v1}.chain(RustyIter{v2});

    ASSERT_EQ(it.next().value(), 4);
}
