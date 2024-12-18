#pragma once

#include "concepts.hpp"
#include "cycle.hpp"
#include "filter.hpp"
#include "map.hpp"
#include "take.hpp"

#include <stdexcept>
#include <vector>

namespace rusty_iterators::interface
{
using concepts::Comparable;
using concepts::FilterFunctor;
using concepts::FoldFunctor;
using concepts::ForEachFunctor;
using concepts::Summable;

using iterator::Cycle;
using iterator::Filter;
using iterator::Map;
using iterator::Take;

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
    [[nodiscard]] auto count() -> size_t;
    [[nodiscard]] auto cycle() -> Cycle<T, Derived>;

    template <class Functor>
        requires FilterFunctor<T, Functor>
    [[nodiscard]] auto filter(Functor&& f) -> Filter<T, Functor, Derived>;

    template <class B, class Functor>
        requires FoldFunctor<B, T, Functor>
    [[nodiscard]] auto fold(B&& init, Functor&& f) -> B;

    template <class Functor>
        requires ForEachFunctor<T, Functor>
    auto forEach(Functor&& f) -> void;

    template <class Functor>
        requires std::invocable<Functor, T>
    [[nodiscard]] auto map(Functor&& f) -> Map<T, Functor, Derived>;

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto max() -> std::optional<R>;

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto min() -> std::optional<R>;

    template <class Functor>
        requires FoldFunctor<T, T, Functor>
    [[nodiscard]] auto reduce(Functor&& f) -> std::optional<T>;

    template <class R = T>
        requires Summable<R>
    [[nodiscard]] auto sum() -> R;

    [[nodiscard]] auto take(size_t amount) -> Take<T, Derived>;

  private:
    [[nodiscard]] inline auto self() -> Derived& { return static_cast<Derived&>(*this); }
    auto sizeHintChecked() -> size_t;
};
} // namespace rusty_iterators::interface

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::collect() -> std::vector<T>
{
    auto collection = std::vector<T>{};
    auto size       = sizeHintChecked();

    collection.reserve(size);

    forEach([&collection](auto&& x) { collection.push_back(std::move(x)); });

    return std::move(collection);
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::count() -> size_t
{
    return fold(0, [](auto count, auto _) { return count + 1; });
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::cycle() -> Cycle<T, Derived>
{
    return Cycle<T, Derived>{std::forward<Derived>(self())};
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::iterator::FilterFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::filter(Functor&& f)
    -> Filter<T, Functor, Derived>
{
    return Filter<T, Functor, Derived>{std::forward<Derived>(self()), std::forward<Functor>(f)};
}

template <class T, class Derived>
template <class B, class Functor>
    requires rusty_iterators::concepts::FoldFunctor<B, T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::fold(B&& init, Functor&& f) -> B
{
    sizeHintChecked();

    auto func     = std::forward<Functor>(f);
    auto accum    = std::forward<B>(init);
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        accum    = func(std::move(accum), std::move(nextItem.value()));
        nextItem = self().next();
    }

    return std::move(accum);
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::ForEachFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::forEach(Functor&& f) -> void
{
    sizeHintChecked();

    auto func     = std::forward<Functor>(f);
    auto nextItem = self().next();

    while (nextItem.has_value())
    {
        func(std::move(nextItem.value()));
        nextItem = self().next();
    }
}

template <class T, class Derived>
template <class Functor>
    requires std::invocable<Functor, T>
auto rusty_iterators::interface::IterInterface<T, Derived>::map(Functor&& f)
    -> Map<T, Functor, Derived>
{
    return Map<T, Functor, Derived>{std::forward<Derived>(self()), std::forward<Functor>(f)};
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Comparable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::max() -> std::optional<R>
{
    return reduce([](auto x, auto y) { return std::max(x, y); });
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Comparable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::min() -> std::optional<R>
{
    return reduce([](auto x, auto y) { return std::min(x, y); });
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::FoldFunctor<T, T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::reduce(Functor&& f) -> std::optional<T>
{
    auto first = self().next();

    [[unlikely]] if (!first.has_value())
    {
        return std::nullopt;
    }

    return std::make_optional(fold(std::move(first.value()), std::forward<Functor>(f)));
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Summable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::sum() -> R
{
    return fold(R{}, [](auto acc, auto x) { return acc + x; });
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::take(size_t amount) -> Take<T, Derived>
{
    return Take<T, Derived>{std::forward<Derived>(self()), amount};
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::sizeHintChecked() -> size_t
{
    auto size = self().sizeHint();

    [[likely]] if (size.has_value())
    {
        return size.value();
    }
    throw std::length_error{
        "Trying to collect an infinite iterator will result in an infinite loop."};
}
