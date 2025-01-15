#pragma once

#include "concepts.hpp"
#include "interface.fwd.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
using concepts::FilterMapFunctor;
using interface::IterInterface;

template <class Tin, class Tout, class Functor, class Other>
    requires FilterMapFunctor<Tin, Tout, Functor>
class FilterMap : public IterInterface<Tout, FilterMap<Tin, Tout, Functor, Other>>
{
  public:
    explicit FilterMap(Other&& it, Functor&& f)
        : it(std::forward<Other>(it)), func(std::forward<Functor>(f))
    {}

    auto next() -> std::optional<Tout>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    Functor func;
};
} // namespace rusty_iterators::iterator

template <class Tin, class Tout, class Functor, class Other>
    requires rusty_iterators::concepts::FilterMapFunctor<Tin, Tout, Functor>
auto rusty_iterators::iterator::FilterMap<Tin, Tout, Functor, Other>::next() -> std::optional<Tout>
{
    auto nextItem = it.next();

    while (nextItem.has_value())
    {
        auto result = func(std::move(nextItem.value()));

        if (result.has_value())
            return std::move(result.value());

        nextItem = it.next();
    }
    return std::nullopt;
}

template <class Tin, class Tout, class Functor, class Other>
    requires rusty_iterators::concepts::FilterMapFunctor<Tin, Tout, Functor>
auto rusty_iterators::iterator::FilterMap<Tin, Tout, Functor, Other>::sizeHint() const
    -> std::optional<size_t>
{
    return it.sizeHint();
}
