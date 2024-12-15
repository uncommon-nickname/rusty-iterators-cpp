#pragma once

#include "interface.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
template <class Container>
    requires std::ranges::range<Container>
class RustyIter
    : public interface::IterInterface<typename Container::value_type, RustyIter<Container>>
{
    using Item     = typename Container::value_type;
    using Iterator = typename Container::iterator;

  public:
    explicit RustyIter(Container& it) : head(it.begin()), tail(it.end() - 1), size(it.size()) {}

    auto nextBack() -> std::optional<Item>;
    auto nextFront() -> std::optional<Item>;
    [[nodiscard]] inline auto sizeHint() const -> size_t;

  private:
    Iterator head;
    Iterator tail;
    size_t size;
};
} // namespace rusty_iterators::iterator

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::RustyIter<Container>::nextBack() -> std::optional<Item>
{
    [[unlikely]] if (tail == head - 1)
    {
        return std::nullopt;
    }

    auto item = *tail;
    tail -= 1;

    return std::make_optional(std::forward<Item>(item));
}

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::RustyIter<Container>::nextFront() -> std::optional<Item>
{
    [[unlikely]] if (head == tail + 1)
    {
        return std::nullopt;
    }

    auto item = *head;
    head += 1;

    return std::make_optional(std::forward<Item>(item));
}

template <class Container>
    requires std::ranges::range<Container>
inline auto rusty_iterators::iterator::RustyIter<Container>::sizeHint() const -> size_t
{
    return size;
}
