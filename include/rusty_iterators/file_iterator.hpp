#pragma once

#include "interface.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

class LazyFileIter : public IterInterface<std::string, LazyFileIter>
{
  public:
    explicit LazyFileIter(const std::string& fileName) : is(fileName)
    {
        if (!is.is_open())
        {
            throw std::runtime_error{"Could not open the file."};
        }
    };

    auto next() -> std::optional<std::string>
    {
        std::string nextLine;

        [[unlikely]] if (!std::getline(is, nextLine))
        {
            return std::nullopt;
        }
        return std::make_optional(std::move(nextLine));
    }

    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>
    {
        // We don't know how many lines the file has. No optimizations here.
        return std::make_optional(0);
    }

  private:
    std::ifstream is;
};
} // namespace rusty_iterators::iterator
