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
    explicit RustyIter(Container& it) : ptr(it.begin()), end(it.end()) {}

    auto next() -> std::optional<T>;
    [[nodiscard]] inline auto sizeHint() const -> std::optional<size_t>;

  private:
    Iterator ptr;
    Iterator end;
};
} // namespace rusty_iterators::iterator

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::RustyIter<Container>::next() -> std::optional<T>
{
    [[unlikely]] if (ptr == end)
    {
        return std::nullopt;
    }

    auto& item = *ptr;
    ptr += 1;

    return std::make_optional(std::cref(item));
}

template <class Container>
    requires std::ranges::range<Container>
inline auto rusty_iterators::iterator::RustyIter<Container>::sizeHint() const
    -> std::optional<size_t>
{
    return std::make_optional(end - ptr);
}
