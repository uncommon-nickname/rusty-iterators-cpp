#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/file_iterator.hpp>
#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::FileIterator;
using ::rusty_iterators::iterator::FIterType;
using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestIteratorIntegration, TestFilterMap)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 == 0; }).map([](auto x) {
        return x * x;
    });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{4, 16}));
}

TEST(TestIteratorIntegration, TestMapFilter)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec}.map([](auto x) { return x * x; }).filter([](auto x) {
        return x % 2 == 0;
    });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{4, 16}));
}

TEST(TestIteratorIntegration, TestMultipleMaps)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}
                  .map([](auto x) { return x * x; })
                  .map([](auto x) { return x + 3; })
                  .map([](auto x) { return std::format("Hello {}", x); });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{"Hello 4", "Hello 7", "Hello 12"}));
}

TEST(TestIteratorIntegration, TestFilterMapCycled)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.filter([](auto x) { return x % 2 != 0; }).cycle().map([](auto x) {
        return x + 3;
    });

    ASSERT_EQ(it.next(), 4);
    ASSERT_EQ(it.next(), 6);
    ASSERT_EQ(it.next(), 4);
}

TEST(TestIteratorIntegration, TestTakeOnCycle)
{
    auto vec = std::vector{1, 2};
    auto it  = LazyIterator{vec}.cycle().take(4);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 1, 2}));
}

TEST(TestIteratorIntegration, TestCheckIfIteratorIncremental)
{
    auto goodVec = std::vector{1, 2, 2, 4};
    auto r1      = LazyIterator{goodVec}.movingWindow(2).all([](auto x) { return x[0] <= x[1]; });

    ASSERT_TRUE(r1);

    auto badVec = std::vector{1, 2, 4, 2};
    auto r2     = LazyIterator{badVec}.movingWindow(2).all([](auto x) { return x[0] <= x[1]; });

    ASSERT_FALSE(r2);
}

TEST(TestIteratorIntegration, TestWindowsOverCycle)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec}.cycle().movingWindow(2);

    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{1, 2}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{2, 3}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{3, 1}));
    EXPECT_THAT(it.next().value(), ElementsAreArray(std::array{1, 2}));
}

TEST(TestIteratorIntegration, TestCycleAdvanceBy)
{
    auto vec = std::vector{1, 2};
    auto it  = LazyIterator{vec}.cycle().advanceBy(3);

    ASSERT_EQ(it.next().value(), 2);
    ASSERT_EQ(it.next().value(), 1);
}

TEST(TestIteratorIntegration, TestChainMultipleIterators)
{
    auto v1 = std::vector{1, 2};
    auto v2 = std::array{4, 5};
    auto v3 = std::vector{3, 6};

    auto it = LazyIterator{v1}
                  .chain(LazyIterator{v2})
                  .map([](auto x) { return x * 2; })
                  .chain(LazyIterator{v3}.cycle())
                  .take(10);

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{2, 4, 8, 10, 3, 6, 3, 6, 3, 6}));
}

TEST(TestIteratorIntegration, TestFindMaxDiffBetweenTwoPairsUsingZip)
{
    auto v1 = std::vector{1, 2, 3, 4, 5};
    auto v2 = std::vector{2, 7, 4, 8, 1};

    auto result = LazyIterator{v1}
                      .zip(LazyIterator{v2})
                      .map([](auto x) { return std::abs(std::get<0>(x) - std::get<1>(x)); })
                      .max();

    ASSERT_EQ(result.value(), 5);
}

TEST(TestIteratorIntegration, TestLoadFileDataAndParseIt)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Lazy>{testFileName}.map(
        [](auto x) { return std::atoi(x.c_str()); });

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
}
