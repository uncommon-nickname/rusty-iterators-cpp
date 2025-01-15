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

template <>
class FileIterator<FIterType::Buffered>
    : public IterInterface<std::string, FileIterator<FIterType::Buffered>>
{
  public:
    explicit FileIterator(const std::string& filePath, char delimiter = '\n')
    {
        std::ifstream is{filePath};

        if (!is.is_open())
            throw std::runtime_error{"Could not open the file."};

        std::string nextLine;

        while (std::getline(is, nextLine, delimiter))
            fileLines.push_back(std::move(nextLine));

        is.close();
    };

    auto next() -> std::optional<std::string>
    {
        [[unlikely]] if (ptr == fileLines.size())
            return std::nullopt;

        auto line = fileLines.at(ptr);
        ptr += 1;
        return std::move(line);
    }

    [[nodiscard]] auto sizeHint() const -> std::optional<size_t> { return fileLines.size() - ptr; }

  private:
    std::vector<std::string> fileLines{};
    size_t ptr = 0;
};

template <>
class FileIterator<FIterType::Lazy>
    : public IterInterface<std::string, FileIterator<FIterType::Lazy>>
{
  public:
    explicit FileIterator(const std::string& filePath, char delimiter = '\n')
        : is(filePath), delimiter(delimiter)
    {
        if (!is.is_open())
            throw std::runtime_error{"Could not open the file."};
    };

    auto next() -> std::optional<std::string>
    {
        std::string nextLine;

        [[unlikely]] if (!std::getline(is, nextLine, delimiter))
            return std::nullopt;

        return std::move(nextLine);
    }

    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>
    {
        // We don't know how many lines the file has. No optimizations here.
        return 0;
    }

  private:
    std::ifstream is;
    char delimiter;
};
} // namespace rusty_iterators::iterator
