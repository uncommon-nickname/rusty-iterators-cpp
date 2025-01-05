#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/file_iterator.hpp>

using ::rusty_iterators::iterator::LazyFileIter;
using ::testing::ElementsAreArray;

constexpr std::string testFileName = "./tests/test_data.txt";

TEST(TestFileIterator, TestLoadFileData)
{
    auto it = LazyFileIter{testFileName};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{"1", "2", "3", "4"}));
}

TEST(TestFileIterator, TestCountLinesInFile)
{
    auto it = LazyFileIter{testFileName};

    ASSERT_EQ(it.count(), 4);
}

TEST(TestFileIterator, TestSumIterator)
{
    auto it = LazyFileIter{testFileName};

    ASSERT_EQ(it.sum(), std::string{"1234"});
}
