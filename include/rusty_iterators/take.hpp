#pragma once

#include "interface.fwd.hpp"

#include <algorithm>
#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Take : public IterInterface<T, Take<T, Other>>
{
  public:
    Take(Other&& it, size_t size) : it(std::forward<Other>(it)), size(size) {}

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    size_t size;
    size_t taken = 0;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Take<T, Other>::next() -> std::optional<T>
{
    if (taken == size)
    {
        return std::nullopt;
    }

    auto nextItem = it.next();

    if (!nextItem.has_value())
    {
        return std::nullopt;
    }

    taken += 1;

    return std::move(nextItem);
}

template <class T, class Other>
auto rusty_iterators::iterator::Take<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto itSize   = it.sizeHint();
    auto currSize = size - taken;

    if (!itSize.has_value())
    {
        return std::make_optional(currSize);
    }

    return std::make_optional(std::min(currSize, itSize.value()));
}