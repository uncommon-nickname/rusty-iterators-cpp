#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <type_traits>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin>
class Map : public IterInterface<std::invoke_result_t<Functor, Tin>, Map<Tin, Functor, Other>>
{
    using Tout = std::invoke_result_t<Functor, Tin>;

  public:
    explicit Map(Other&& it, Functor&& f)
        : it(std::forward<Other>(it)), f(std::forward<Functor>(f)) {};

    auto nextFront() -> std::optional<Tout>;
    auto nextBack() -> std::optional<Tout>;
    auto sizeHint() -> size_t;

  private:
    Other it;
    Functor f;
};
} // namespace rusty_iterators::iterator

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::nextFront() -> std::optional<Tout>
{
    auto item = it.nextFront();
    return item.has_value() ? std::make_optional(f(item.value())) : std::nullopt;
}

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::nextBack() -> std::optional<Tout>
{
    auto item = it.nextBack();
    return item.has_value() ? std::make_optional(f(item.value())) : std::nullopt;
}

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::sizeHint() -> size_t
{
    return it.sizeHint();
}
