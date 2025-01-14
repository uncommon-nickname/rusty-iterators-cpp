#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <type_traits>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin&&>
class Map : public IterInterface<std::invoke_result_t<Functor, Tin>, Map<Tin, Functor, Other>>
{
    using Tout = std::invoke_result_t<Functor, Tin>;

  public:
    explicit Map(Other&& it, Functor&& f)
        : it(std::forward<Other>(it)), func(std::forward<Functor>(f)) {};

    [[nodiscard]] auto count() -> size_t;
    auto next() -> std::optional<Tout>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    Functor func;
};
} // namespace rusty_iterators::iterator

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin&&>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::count() -> size_t
{
    // We do not have to apply the mapping callable just to count elements.
    return it.count();
}

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin&&>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::next() -> std::optional<Tout>
{
    auto item = it.next();

    [[likely]] if (item.has_value())
        return func(std::move(item.value()));

    return std::nullopt;
}

template <class Tin, class Functor, class Other>
    requires std::invocable<Functor, Tin&&>
auto rusty_iterators::iterator::Map<Tin, Functor, Other>::sizeHint() const -> std::optional<size_t>
{
    // Map does not change the size of the underlying iterator.
    return it.sizeHint();
}
