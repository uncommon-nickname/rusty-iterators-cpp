#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <tuple>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Enumerate : public IterInterface<std::tuple<size_t, T>, Enumerate<T, Other>>
{
  public:
    explicit Enumerate(Other&& it) : it(std::forward<Other>(it)) {}

    [[nodiscard]] auto count() -> size_t;
    auto next() -> std::optional<std::tuple<size_t, T>>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    size_t idx = 0;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Enumerate<T, Other>::count() -> size_t
{
    return it.count();
}

template <class T, class Other>
auto rusty_iterators::iterator::Enumerate<T, Other>::next() -> std::optional<std::tuple<size_t, T>>
{
    auto nextItem = it.next();

    if (!nextItem.has_value())
    {
        return std::nullopt;
    }
    return std::tuple{idx++, std::move(nextItem.value())};
}

template <class T, class Other>
auto rusty_iterators::iterator::Enumerate<T, Other>::sizeHint() const -> std::optional<size_t>
{
    return it.sizeHint();
}
