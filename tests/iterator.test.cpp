#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::ElementsAreArray;

TEST(TestIterator, TestCopyHasItsOwnPointers)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = RustyIter{vec};
    auto cp = it;

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
    EXPECT_THAT(cp.collect(), ElementsAreArray(std::array{1, 2, 3, 4}));
}

TEST(TestIterator, TestCopySavesIteratorState)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = RustyIter{vec};
    it.next();
    auto cp = it;

    auto x = it.collect();
    auto y = cp.collect();

    EXPECT_THAT(x, ElementsAreArray(std::array{2, 3, 4}));
    EXPECT_THAT(y, ElementsAreArray(std::array{2, 3, 4}));
}

TEST(TestIterator, NextReturnsItemsFromTheHead)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), 2);
}

TEST(TestIterator, NextReturnsNoneWhenDepleted)
{
    auto vec = std::vector{1};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.next(), 1);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestIterator, CollectingAlreadyCollected)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{1, 2, 3}));
    EXPECT_THAT(it.collect(), ElementsAreArray(std::array<int, 0>{}));
}

TEST(TestIterator, CollectedItemsAreReferences)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    auto result = it.collect();

    ASSERT_EQ(&result[0].get(), &vec[0]);
}

TEST(TestIterator, SizeHintReturnsSizeOfTheContainer)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.sizeHint(), 3);
}

TEST(TestIterator, SizeHintOnPartiallyCollectedIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    it.next();

    ASSERT_EQ(it.sizeHint(), 2);
}

TEST(TestIterator, SizeHintOnCollectedIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto it  = RustyIter{vec};

    auto _ = it.collect();

    ASSERT_EQ(it.sizeHint(), 0);
}

TEST(TestIterator, CountReturnsRealAmountOfElements)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.count(), 4);
}

TEST(TestIterator, MaxReturnsTheBiggestNumber)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.max(), 4);
}

TEST(TestIterator, MaxOnEmptyVec)
{
    auto vec = std::vector<int>{};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.max(), std::nullopt);
}

TEST(TestIterator, MinReturnsTheSmallestNumber)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.min(), 1);
}

TEST(TestIterator, MinOnEmptyVec)
{
    auto vec = std::vector<int>{};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.min(), std::nullopt);
}

TEST(TestIterator, FoldUsedToSum)
{
    auto vec    = std::vector{1, 2, 3, 4};
    auto result = RustyIter{vec}.fold(0, [](auto acc, auto x) { return acc + x; });

    ASSERT_EQ(result, 10);
}

TEST(TestIterator, FoldUsedToConcatenate)
{
    auto vec  = std::vector{1, 2, 3, 4};
    auto zero = std::string{"0"};

    auto result = RustyIter{vec}.fold(
        std::move(zero), [](auto acc, auto x) { return std::format("({} + {})", acc, x.get()); });

    ASSERT_EQ(result, "((((0 + 1) + 2) + 3) + 4)");
}

TEST(TestIterator, SumAllOfTheElements)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto it  = RustyIter{vec};

    ASSERT_EQ(it.sum(), 10);
}

TEST(TestIterator, ForEachElementIncrementRef)
{
    auto vec = std::vector{1, 2, 3, 4};
    auto i   = 0;

    RustyIter{vec}.forEach([&i](auto x) { i += 1; });

    ASSERT_EQ(i, 4);
}

TEST(TestIterator, AnyReturnsTrueIfOneFits)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = RustyIter{vec}.any([](auto x) { return x == 2; });

    ASSERT_TRUE(result);
}

TEST(TestIterator, AnyReturnsFalseIfNothingFits)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = RustyIter{vec}.any([](auto x) { return x == 4; });

    ASSERT_FALSE(result);
}

TEST(TestIterator, AllReturnsTrueIfAllFit)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = RustyIter{vec}.all([](auto x) { return x > 0; });

    ASSERT_TRUE(result);
}

TEST(TestIterator, AllReturnsFalseIfOneDoesntFit)
{
    auto vec    = std::vector{1, 2, 3};
    auto result = RustyIter{vec}.all([](auto x) { return x < 3; });

    ASSERT_FALSE(result);
}
