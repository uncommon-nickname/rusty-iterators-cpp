#pragma once

#include "interface.fwd.hpp"

#include <algorithm>
#include <optional>
#include <stdexcept>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Take : public IterInterface<T, Take<T, Other>>
{
  public:
    Take(Other&& it, size_t size) : it(std::forward<Other>(it)), size(size)
    {
        if (size == 0)
            throw std::length_error{"You have to take at least one item."};
    }

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
        return std::nullopt;

    taken += 1;
    return std::move(it.next());
}

template <class T, class Other>
auto rusty_iterators::iterator::Take<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto itSize   = it.sizeHint();
    auto currSize = size - taken;

    if (!itSize.has_value())
        return currSize;

    return std::min(currSize, itSize.value());
}
