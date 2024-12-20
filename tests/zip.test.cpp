#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::RustyIter;
using ::testing::FieldsAre;

TEST(TestZipIterator, TestCollectedTuples)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4, 5, 6};

    auto result = RustyIter{v1}.zip(RustyIter{v2}).collect();

    ASSERT_EQ(result.size(), 3);
    EXPECT_THAT(result[0], FieldsAre(1, 4));
    EXPECT_THAT(result[1], FieldsAre(2, 5));
    EXPECT_THAT(result[2], FieldsAre(3, 6));
}

TEST(TestZipIterator, TestShorterIteratorDefinesTheEnd)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{4};

    auto it = RustyIter{v1}.zip(RustyIter{v2});

    EXPECT_THAT(it.next().value(), FieldsAre(1, 4));
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestZipIterator, TestSizeHintWhenOneIsInf)
{
    auto v1 = std::vector{1, 2};
    auto v2 = std::vector{3, 4};

    auto it = RustyIter{v1}.zip(RustyIter{v2}.cycle());

    ASSERT_EQ(it.sizeHint().value(), 2);
}

TEST(TestZipIterator, TestSizeHintWhenOneShorter)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector{3, 4};

    auto it = RustyIter{v1}.zip(RustyIter{v2});

    ASSERT_EQ(it.sizeHint().value(), 2);
}

TEST(TestZipIterator, TestSizeHintWhenBothInf)
{
    auto v1 = std::vector{1, 2};
    auto v2 = std::vector{3, 4};

    auto it = RustyIter{v1}.cycle().zip(RustyIter{v2}.cycle());

    ASSERT_EQ(it.sizeHint(), std::nullopt);
}

TEST(TestZipIterator, TestDifferentTypesOfZippedIterators)
{
    auto v1 = std::vector{1, 2, 3};
    auto v2 = std::vector<std::string>{"a", "b", "c"};

    auto item = RustyIter{v1}.zip(RustyIter{v2}).next().value();

    ASSERT_EQ(std::get<0>(item), 1);
    ASSERT_EQ(std::get<1>(item).get(), "a");
}
