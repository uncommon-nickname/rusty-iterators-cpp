#pragma once

#include "interface.fwd.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Interperse : public IterInterface<T, Interperse<T, Other>>
{
  public:
    explicit Interperse(Other&& it, T&& item)
        : it(std::forward<Other>(it)), item(std::forward<T>(item))
    {}

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    T item;
    bool interperse = false;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Interperse<T, Other>::next() -> std::optional<T>
{
    if (interperse)
    {
        interperse = false;
        return item;
    }

    auto nextItem = it.next();

    if (!nextItem.has_value())
        return std::move(nextItem);

    interperse = true;
    return std::move(nextItem);
}

template <class T, class Other>
auto rusty_iterators::iterator::Interperse<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto underlyingSize = it.sizeHint();

    if (!underlyingSize.has_value())
        return std::nullopt;

    return underlyingSize.value() * 2;
}
