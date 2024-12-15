#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestIteratorCopying, TestCopyHasItsOwnPointers)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = RustyIter{vec};
    auto cp = it;

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
    EXPECT_THAT(cp.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
}

TEST(TestIteratorCopying, TestCopySavesIteratorState)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = RustyIter{vec};
    it.nextFront();
    auto cp = it;

    auto x = it.collect();
    auto y = cp.collect();

    EXPECT_THAT(x, ElementsAreArray(std::array{2, 3, 4}));
    EXPECT_THAT(y, ElementsAreArray(std::array{2, 3, 4}));
}

TEST(TestIteratorNextFront, NextFrontReturnsItemsFromTheHead)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.nextFront(), 1);
    ASSERT_EQ(it.nextFront(), 2);
}

TEST(TestIteratorNextFront, NextFrontReturnsNoneWhenDepleted)
{
    auto vec = std::vector{1};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.nextFront(), 1);
    ASSERT_EQ(it.nextFront(), std::nullopt);
}

TEST(TestIteratorNextBack, NextBackReturnsItemsFromTheTail)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.nextBack(), 3);
    ASSERT_EQ(it.nextBack(), 2);
}

TEST(TestIteratorNextBack, NextBackReturnsNoneWhenDepleted)
{
    auto vec = std::vector{1};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.nextBack(), 1);
    ASSERT_EQ(it.nextBack(), std::nullopt);
}

TEST(TestIteratorPointers, DepletingIteratorFromBothSides)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.nextFront(), 1);
    ASSERT_EQ(it.nextBack(), 4);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{2, 3}));
}

TEST(TestIteratorCollection, CollectingAlreadyCollected)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3}));
    EXPECT_THAT(it.collect(), ElementsAreArray(std::array<int, 0>{}));
}
