#pragma once

#include "interface.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

enum class FIterType : uint8_t
{
    Lazy,
    Buffered
};

template <FIterType type>
class FileIterator : public IterInterface<std::string, FileIterator<type>>
{};

// TODO: Perf.
// With the buffered iterator a lot of iterator functions could be optimized.
template <>
class FileIterator<FIterType::Buffered>
    : public IterInterface<std::string, FileIterator<FIterType::Buffered>>
{
  public:
    explicit FileIterator(const std::string& filePath)
    {
        std::ifstream is{filePath};

        if (!is.is_open())
        {
            throw std::runtime_error{"Could not open the file."};
        }

        std::string nextLine;

        while (std::getline(is, nextLine))
        {
            fileLines.push_back(std::move(nextLine));
        }
        is.close();
    };

    auto next() -> std::optional<std::string>
    {
        [[unlikely]] if (ptr == fileLines.size())
        {
            return std::nullopt;
        }
        auto line = fileLines.at(ptr);
        ptr += 1;
        return std::make_optional(std::move(line));
    }

    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>
    {
        return std::make_optional(fileLines.size());
    }

  private:
    size_t ptr = 0;
    std::vector<std::string> fileLines{};
};

template <>
class FileIterator<FIterType::Lazy>
    : public IterInterface<std::string, FileIterator<FIterType::Lazy>>
{
  public:
    explicit FileIterator(const std::string& filePath) : is(filePath)
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
