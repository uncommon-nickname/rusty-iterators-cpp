#include <gtest/gtest.h>
#include <rusty_iterators/iterator.hpp>

using rusty_iterators::iterator::Iterator;

TEST(Abc, Abc)
{
    auto vec = std::vector{1, 2, 3, 4};

    auto it = Iterator{vec};
    auto cp = it;

    auto x = it.collect();
    auto y = cp.collect();

    ASSERT_EQ(x, y);
}
