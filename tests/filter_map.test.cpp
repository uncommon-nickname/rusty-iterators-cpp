#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rusty_iterators/iterator.hpp>

using ::rusty_iterators::iterator::LazyIterator;

TEST(TestFilterMapIterator, NextChecksIfValid)
{
    auto vec = std::vector{1, 2, 3};
    auto f   = [](auto x) -> std::optional<float> {
        if (x % 2 == 0)
            return std::make_optional(static_cast<float>(x));
        return std::nullopt;
    };
    auto it = LazyIterator{vec}.filterMap(std::move(f));

    ASSERT_EQ(it.next(), 2.0);
    ASSERT_EQ(it.next(), std::nullopt);
}

TEST(TestFilterMapIterator, SizeHintReturnsValueOfUnderlyingIterator)
{
    auto vec = std::vector{1, 2, 3};
    auto f   = [](auto x) -> std::optional<float> {
        if (x % 2 == 0)
            return std::make_optional(static_cast<float>(x));
        return std::nullopt;
    };
    auto it = LazyIterator{vec}.filterMap(std::move(f));

    ASSERT_EQ(it.sizeHint(), 3.0);
}
