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
