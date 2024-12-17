#pragma once

#include "concepts.hpp"
#include "filter.hpp"
#include "map.hpp"

#include <stdexcept>
#include <vector>

namespace rusty_iterators::interface
{
using concepts::Comparable;
using concepts::FilterFunctor;
using concepts::FoldFunctor;
using concepts::Summable;

using iterator::Filter;
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
    [[nodiscard]] auto count() -> size_t;

    template <class B, class Functor>
        requires FoldFunctor<B, T, Functor>
    [[nodiscard]] auto fold(B&& init, Functor&& f) -> B;

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

    template <class Functor>
        requires FilterFunctor<T, Functor>
    [[nodiscard]] auto filter(Functor&& f) -> Filter<T, Functor, Derived>;

    template <class Functor>
        requires std::invocable<Functor, T>
    [[nodiscard]] auto map(Functor&& f) -> Map<T, Functor, Derived>;

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
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        collection.push_back(std::move(nextItem.value()));
        nextItem = self().next();
    }
    return std::move(collection);
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::count() -> size_t
{
    sizeHintChecked();

    size_t amount = 0;
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        amount += 1;
        nextItem = self().next();
    }
    return amount;
}

template <class T, class Derived>
template <class B, class Functor>
    requires rusty_iterators::concepts::FoldFunctor<B, T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::fold(B&& init, Functor&& f) -> B
{
    sizeHintChecked();

    auto func  = std::forward<Functor>(f);
    auto accum = std::forward<B>(init);

    while (true)
    {
        auto nextItem = self().next();

        if (!nextItem.has_value())
        {
            break;
        }

        accum = func(std::move(accum), std::move(nextItem.value()));
    }
    return std::move(accum);
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

    if (!first.has_value())
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
template <class Functor>
    requires rusty_iterators::iterator::FilterFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::filter(Functor&& f)
    -> Filter<T, Functor, Derived>
{
    return Filter<T, Functor, Derived>{std::forward<Derived>(self()), std::forward<Functor>(f)};
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
auto rusty_iterators::interface::IterInterface<T, Derived>::sizeHintChecked() -> size_t
{
    auto size = self().sizeHint();

    if (!size.has_value())
    {
        throw std::length_error{
            "Trying to collect an infinite iterator will result in an infinite loop."};
    }
    return size.value();
}
