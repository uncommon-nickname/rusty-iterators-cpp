#include <rusty_iterators/file_iterator.hpp>
#include <rusty_iterators/iterator.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

using ::rusty_iterators::iterator::FileIterator;
using ::rusty_iterators::iterator::FIterType;
using ::rusty_iterators::iterator::LazyIterator;

auto splitByWhitespaceAndCastToInt(std::string str) -> std::vector<int>
{
    std::istringstream iss{str};
    std::vector<int> tokens{};
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(std::atoi(token.c_str()));
    }
    return std::move(tokens);
}

auto isReportSafe(std::vector<int> vec) -> bool
{
    auto signum = (vec.at(1) - vec.at(0)) > 0 ? 1 : -1;

    return LazyIterator{vec}.movingWindow(2).all([&signum](auto x) {
        auto result     = x.at(1) - x.at(0);
        auto currSignum = result > 0 ? 1 : -1;
        auto absResult  = std::abs(result);

        return absResult >= 1 && absResult <= 3 && currSignum == signum;
    });
}

/*
 * An example solution to Advent of Code 2024: day 2, part 1.
 * https://adventofcode.com/2024/day/2
 */
auto main() -> int
{
    auto result = FileIterator<FIterType::Lazy>{std::string{"./examples/aoc/input.txt"}}
                      .map(splitByWhitespaceAndCastToInt)
                      .filter(isReportSafe)
                      .count();

    std::cout << "Expected amount of safe reports: 2" << std::endl;
    std::cout << "Calculated amount of safe reports: " << result << std::endl;

    assert(result == 2);
}
