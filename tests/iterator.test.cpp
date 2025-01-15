#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;
using ::testing::ElementsAreArray;

TEST(TestIterator, TestCopyHasItsOwnPointers)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = LazyIterator{vec};
    auto cp = it;

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
    EXPECT_THAT(cp.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
}

TEST(TestIterator, TestCopySavesIteratorState)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = LazyIterator{vec};

    it.next();
    auto cp = it;
    it.next();

    auto x = it.collect();
    auto y = cp.collect();

    EXPECT_THAT(x, ElementsAreArray(std::array{3, 4}));
    EXPECT_THAT(y, ElementsAreArray(std::array{2, 3, 4}));
}

TEST(TestIterator, NextReturnsItemsFromTheHead)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 2);
}

TEST(TestIterator, NextReturnsNoneWhenDepleted)
{
    auto vec = std::vector{1};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestIterator, CollectingAlreadyCollected)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3}));
    EXPECT_THAT(it.collect(), ElementsAreArray(std::array<int, 0>{}));
}

TEST(TestIterator, CollectedItemsAreReferences)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    auto result = it.collect();

    ASSERT_EQ(&result[0].get(), &vec[0]);
}

TEST(TestIterator, SizeHintReturnsSizeOfTheContainer)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestIterator, SizeHintOnPartiallyCollectedIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    it.next();

    ASSERT_EQ(it.sizeHint(), 2);
}

TEST(TestIterator, SizeHintOnCollectedIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    auto _ = it.collect();

    ASSERT_EQ(it.sizeHint(), 0);
}

TEST(TestIterator, CountReturnsRealAmountOfElements)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.count(), 4);
}

TEST(TestIterator, MaxReturnsTheBiggestNumber)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.max(), 4);
}

TEST(TestIterator, MaxOnEmptyVec)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.max(), std::nullopt);
}

TEST(TestIterator, MinReturnsTheSmallestNumber)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.min(), 1);
}

TEST(TestIterator, MinOnEmptyVec)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.min(), std::nullopt);
}

TEST(TestIterator, FoldUsedToSum)
{
    auto vec    = std::vector{1, 2, 3, 4};
    auto result = LazyIterator{vec}.fold(0, [](auto acc, auto x) { return acc + x; });

    ASSERT_EQ(result, 10);
}

TEST(TestIterator, FoldUsedToConcatenate)
{
    auto vec  = std::vector{1, 2, 3, 4};
    auto zero = std::string{"0"};

    auto result = LazyIterator{vec}.fold(
        std::move(zero), [](auto acc, auto x) { return std::format("({} + {})", acc, x.get()); });

    ASSERT_EQ(result, "((((0 + 1) + 2) + 3) + 4)");
}

TEST(TestIterator, SumAllOfTheElements)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.sum(), 10);
}

TEST(TestIterator, ForEachElementIncrementRef)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto i   = 0;

    LazyIterator{vec}.forEach([&i](auto x) { i += 1; });

    ASSERT_EQ(i, 4);
}

TEST(TestIterator, AnyReturnsTrueIfOneFits)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.any([](auto x) { return x == 2; });

    ASSERT_TRUE(result);
}

TEST(TestIterator, AnyReturnsFalseIfNothingFits)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.any([](auto x) { return x == 4; });

    ASSERT_FALSE(result);
}

TEST(TestIterator, AllReturnsTrueIfAllFit)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.all([](auto x) { return x > 0; });

    ASSERT_TRUE(result);
}

TEST(TestIterator, AllReturnsFalseIfOneDoesntFit)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.all([](auto x) { return x < 3; });

    ASSERT_FALSE(result);
}

TEST(TestIterator, AdvanceByMovesIteratorPtr)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = LazyIterator{vec};

    it.advanceBy(3);

    ASSERT_EQ(it.next().value(), 4);
}

TEST(TestIterator, AdvanceByBiggerThanSize)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    it.advanceBy(4);

    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestIterator, TestNthGetFirstElement)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.nth(0).value(), 1);
}

TEST(TestIterator, TestNthGetElement)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.nth(2).value(), 3);
}

TEST(TestIterator, TestNthElementOutOfIteratorRange)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.nth(4), std::nullopt);
}

TEST(TestIterator, TestUnzipIterator)
{
    auto v1 = std::vector{1, 2, 3, 4};
    auto v2 = std::vector{2.3, 1.5, 4.3};

    auto [f, s] = LazyIterator{v1}.zip(LazyIterator{v2}).unzip();

    EXPECT_THAT(f, ElementsAreArray(std::array{1, 2, 3}));
    EXPECT_THAT(s, ElementsAreArray(std::array{2.3, 1.5, 4.3}));
}

TEST(TestIterator, TestUnzipVectorElement)
{
    auto vec    = std::vector{1, 2, 3, 4};
    auto [f, s] = LazyIterator{vec}.movingWindow(2).unzip();

    EXPECT_THAT(f, ElementsAreArray(std::array{1, 2, 3}));
    EXPECT_THAT(s, ElementsAreArray(std::array{2, 3, 4}));
}

TEST(TestIterator, TestPositionOfEmptyIterator)
{
    auto vec    = std::vector<int>{};
    auto result = LazyIterator{vec}.position([](auto x) { return x == 1; });

    ASSERT_EQ(result, std::nullopt);
}

TEST(TestIterator, TestPositionIndexedFromZero)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.position([](auto x) { return x == 1; });

    ASSERT_EQ(result.value(), 0);
}

TEST(TestIterator, TestPositionNoValueMatched)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = LazyIterator{vec}.position([](auto x) { return x > 5; });

    ASSERT_EQ(result, std::nullopt);
}

TEST(TestIterator, TestEqWhenSame)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{1, 2, 3};

    ASSERT_TRUE(LazyIterator{v1}.eq(LazyIterator{v2}));
}

TEST(TestIterator, TestEqWhenDifferent)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{1, 2, 4};

    ASSERT_FALSE(LazyIterator{v1}.eq(LazyIterator{v2}));
}

TEST(TestIterator, TestEqBy)
{
    auto v1 = std::vector<std::string>{"a", "bc", "dea"};
    auto v2 = std::vector<std::string>{"j", "hg", "dfg"};

    // Compare the vectors not by exact content, but by length of strings.
    auto result = LazyIterator{v1}.eqBy(LazyIterator{v2}, [](auto x) {
        return std::get<0>(x).get().size() == std::get<1>(x).get().size();
    });

    ASSERT_TRUE(result);
}

TEST(TestIterator, TestNeWhenSame)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{1, 2, 3};

    ASSERT_FALSE(LazyIterator{v1}.ne(LazyIterator{v2}));
}

TEST(TestIterator, TestNeWhenDifferent)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{1, 2, 4};

    ASSERT_TRUE(LazyIterator{v1}.ne(LazyIterator{v2}));
}

TEST(TestIterator, TestNeBy)
{
    auto v1 = std::vector<std::string>{"a", "bc", "def"};
    auto v2 = std::vector<std::string>{"d", "jk", "cgde"};

    auto result = LazyIterator{v1}.neBy(LazyIterator{v2}, [](auto x) {
        return std::get<0>(x).get().size() != std::get<1>(x).get().size();
    });

    ASSERT_TRUE(result);
}

TEST(TestIterator, TestTryFoldEarlyExit)
{
    auto vec = std::vector{1, 2, 3, 4, 5};
    auto f   = [](auto acc, auto x) {
        return acc > 5 ? std::expected<int, int>{acc} : std::unexpected{x + acc};
    };
    auto result = LazyIterator{vec}.tryFold(0, std::move(f));

    ASSERT_EQ(result, 6);
}

TEST(TestIterator, TestLastOnEmptyIterator)
{
    auto vec = std::vector<int>{};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.last(), std::nullopt);
}

TEST(TestIterator, TestLast)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = LazyIterator{vec};

    ASSERT_EQ(it.last(), 3);
}
