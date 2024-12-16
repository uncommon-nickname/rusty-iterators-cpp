#pragma once

#include "interface.fwd.hpp"

#include <optional>

namespace
{
using rusty_iterators::interface::IterInterface;
}

namespace rusty_iterators::iterator
{
template <class T, class Functor>
concept IsFilterFunctor = requires(Functor f, T t) {
    { f(t) } -> std::same_as<bool>;
};

template <class T, class Functor, class Other>
    requires IsFilterFunctor<T, Functor>
class Filter : public IterInterface<T, Filter<T, Functor, Other>>
{
  public:
    explicit Filter(Other&& it, Functor&& f)
        : it(std::forward<Other>(it)), func(std::forward<Functor>(f)) {};

    auto next() -> std::optional<T>;
    [[nodiscard]] inline auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    Functor func;
};
} // namespace rusty_iterators::iterator

template <class T, class Functor, class Other>
    requires rusty_iterators::iterator::IsFilterFunctor<T, Functor>
auto rusty_iterators::iterator::Filter<T, Functor, Other>::next() -> std::optional<T>
{
    auto nextItem = it.next();

    while (nextItem.has_value())
    {
        if (func(nextItem.value()))
        {
            return nextItem;
        }
        nextItem = it.next();
    }
    return std::nullopt;
}

template <class T, class Functor, class Other>
    requires rusty_iterators::iterator::IsFilterFunctor<T, Functor>
auto rusty_iterators::iterator::Filter<T, Functor, Other>::sizeHint() const -> std::optional<size_t>
{
    // Filter might reduce the size, but without consumption we can't be
    // sure how much. It is better to overallocate so we will return the
    // original size.
    return it.sizeHint();
}
