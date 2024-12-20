#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestIteratorIntegration, TestFilterMap)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it =
        RustyIter{vec}.filter([](auto x) { return x % 2 == 0; }).map([](auto x) { return x * x; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{4, 16}));
}

TEST(TestIteratorIntegration, TestMapFilter)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it =
        RustyIter{vec}.map([](auto x) { return x * x; }).filter([](auto x) { return x % 2 == 0; });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{4, 16}));
}

TEST(TestIteratorIntegration, TestMultipleMaps)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}
                  .map([](auto x) { return x * x; })
                  .map([](auto x) { return x + 3; })
                  .map([](auto x) { return std::format("Hello {}", x); });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{"Hello 4", "Hello 7", "Hello 12"}));
}

TEST(TestIteratorIntegration, TestFilterMapCycled)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.filter([](auto x) { return x % 2 != 0; }).cycle().map([](auto x) {
        return x + 3;
    });

    ASSERT_EQ(it.next(), 4);
    ASSERT_EQ(it.next(), 6);
    ASSERT_EQ(it.next(), 4);
}

TEST(TestIteratorIntegration, TestTakeOnCycle)
{
    auto vec = std::vector{1, 2};
    auto it  = RustyIter{vec}.cycle().take(4);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 1, 2}));
}

TEST(TestIteratorIntegration, TestCheckIfIteratorIncremental)
{
    auto goodVec = std::vector{1, 2, 2, 4};
    auto r1      = RustyIter{goodVec}.movingWindow(2).all([](auto x) { return x[0] <= x[1]; });

    ASSERT_TRUE(r1);

    auto badVec = std::vector{1, 2, 4, 2};
    auto r2     = RustyIter{badVec}.movingWindow(2).all([](auto x) { return x[0] <= x[1]; });

    ASSERT_FALSE(r2);
}

TEST(TestIteratorIntegration, TestWindowsOverCycle)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec}.cycle().movingWindow(2);

    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{1, 2}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{2, 3}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{3, 1}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{1, 2}));
}

TEST(TestIteratorIntegration, TestCycleAdvanceBy)
{
    auto vec = std::vector{1, 2};
    auto it  = RustyIter{vec}.cycle().advanceBy(3);

    ASSERT_EQ(it.next().value(), 2);
    ASSERT_EQ(it.next().value(), 1);
}

TEST(TestIteratorIntegration, TestChainMultipleIterators)
{
    auto v1 = std::vector{1, 2};
    auto v2 = std::array{4, 5};
    auto v3 = std::vector{3, 6};

    auto it = RustyIter{v1}
                  .chain(RustyIter{v2})
                  .map([](auto x) { return x * 2; })
                  .chain(RustyIter{v3}.cycle())
                  .take(10);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{2, 4, 8, 10, 3, 6, 3, 6, 3, 6}));
}
