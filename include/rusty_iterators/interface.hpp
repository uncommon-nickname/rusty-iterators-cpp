#pragma once

#include "map.hpp"

#include <vector>

namespace rusty_iterators::interface
{
using iterator::Map;

template <class T, class Derived>
class IterInterface
{
  public:
    IterInterface()          = default;
    virtual ~IterInterface() = default;

    IterInterface(IterInterface&)                  = default;
    IterInterface& operator=(IterInterface const&) = default;
    IterInterface(IterInterface&&)                 = default;
    IterInterface& operator=(IterInterface&&)      = default;

    [[nodiscard]] auto collect() -> std::vector<T>;

    template <class Functor>
    [[nodiscard]] auto map(Functor&& f) -> iterator::Map<T, Functor, Derived>;

  private:
    [[nodiscard]] inline auto self() -> Derived& { return static_cast<Derived&>(*this); }
};
} // namespace rusty_iterators::interface

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::collect() -> std::vector<T>
{
    auto collection = std::vector<T>{};
    auto size       = self().sizeHint();

    collection.reserve(size);

    auto nextItem = self().nextFront();

    [[likely]] while (nextItem.has_value())
    {
        collection.push_back(nextItem.value());
        nextItem = self().nextFront();
    }

    return std::move(collection);
}

template <class T, class Derived>
template <class Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::map(Functor&& f)
    -> Map<T, Functor, Derived>
{
    return Map<T, Functor, Derived>{std::forward<Derived>(self()), std::forward<Functor>(f)};
}
