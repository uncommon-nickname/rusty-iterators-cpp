#pragma once

#include "interface.hpp"

#include <functional>
#include <optional>

namespace
{
template <class Container>
    requires std::ranges::range<Container>
using Item = std::reference_wrapper<const typename Container::value_type>;
}

namespace rusty_iterators::iterator
{
template <class Container>
    requires std::ranges::range<Container>
class RustyIter : public interface::IterInterface<Item<Container>, RustyIter<Container>>
{
    using T        = Item<Container>;
    using Iterator = typename Container::iterator;

  public:
    explicit RustyIter(Container& it) : head(it.begin()), tail(it.end() - 1), size(it.size()) {}

    auto nextBack() -> std::optional<T>;
    auto nextFront() -> std::optional<T>;
    [[nodiscard]] inline auto sizeHint() const -> std::optional<size_t>;

  private:
    Iterator head;
    Iterator tail;
    size_t size;
};
} // namespace rusty_iterators::iterator

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::RustyIter<Container>::nextBack() -> std::optional<T>
{
    [[unlikely]] if (tail == head - 1)
    {
        return std::nullopt;
    }

    auto& item = *tail;
    tail -= 1;

    return std::make_optional(std::cref(item));
}

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::RustyIter<Container>::nextFront() -> std::optional<T>
{
    [[unlikely]] if (head == tail + 1)
    {
        return std::nullopt;
    }

    auto& item = *head;
    head += 1;

    return std::make_optional(std::cref(item));
}

template <class Container>
    requires std::ranges::range<Container>
inline auto rusty_iterators::iterator::RustyIter<Container>::sizeHint() const
    -> std::optional<size_t>
{
    return std::make_optional(size);
}
