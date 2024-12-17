#pragma once

#include "interface.fwd.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Cycle : public IterInterface<T, Cycle<T, Other>>
{
  public:
    explicit Cycle(Other&& it) : it(std::forward<Other>(it)), original(this->it) {};

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    Other original;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Cycle<T, Other>::next() -> std::optional<T>
{
    auto item = it.next();

    if (!item.has_value())
    {
        it = original;
        return it.next();
    }
    return item;
}

template <class T, class Other>
auto rusty_iterators::iterator::Cycle<T, Other>::sizeHint() const -> std::optional<size_t>
{
    // Cycle iterator is either infinite or empty.
    auto size = original.sizeHint();

    if (size.has_value() && size.value() > 0)
    {
        return std::nullopt;
    }
    return 0;
}
