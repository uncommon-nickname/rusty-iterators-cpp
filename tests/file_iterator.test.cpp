#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/file_iterator.hpp>
#include <stdexcept>

using ::rusty_iterators::iterator::FileIterator;
using ::rusty_iterators::iterator::FIterType;
using ::testing::ElementsAreArray;

TEST(TestLazyFileIterator, TestFileDoesNotExist)
{
    auto testFileName = std::string{"./tests/abc.jpg"};

    EXPECT_THROW(FileIterator<FIterType::Lazy>{testFileName}, std::runtime_error);
}

TEST(TestLazyFileIterator, TestLoadFileData)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Lazy>{testFileName};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{"1", "2", "3", "4"}));
}

TEST(TestLazyFileIterator, TestSizeHintReturnsZero)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Lazy>{testFileName};

    ASSERT_EQ(it.sizeHint(), 0);
}

TEST(TestLazyFileIterator, TestCountLinesInFile)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Lazy>{testFileName};

    ASSERT_EQ(it.count(), 4);
}

TEST(TestLazyFileIterator, TestSumIterator)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Lazy>{testFileName};

    ASSERT_EQ(it.sum(), std::string{"1234"});
}

TEST(TestBufferedFileIterator, TestFileDoesNotExist)
{
    auto testFileName = std::string{"./tests/abc.jpg"};

    EXPECT_THROW(FileIterator<FIterType::Buffered>{testFileName}, std::runtime_error);
}

TEST(TestBufferedFileIterator, TestLoadFileData)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Buffered>{testFileName};

    EXPECT_THAT(it.collect(), ElementsAreArray(std::array{"1", "2", "3", "4"}));
}

TEST(TestBufferedFileIterator, TestSizeHint)
{
    auto testFileName = std::string{"./tests/test_data.txt"};
    auto it           = FileIterator<FIterType::Buffered>{testFileName};

    ASSERT_EQ(it.sizeHint(), 4);
}
