#pragma once

#include "chain.hpp"
#include "concepts.hpp"
#include "cycle.hpp"
#include "filter.hpp"
#include "inspect.hpp"
#include "map.hpp"
#include "moving_window.hpp"
#include "take.hpp"
#include "zip.hpp"

#include <stdexcept>
#include <vector>

namespace rusty_iterators::interface
{
using concepts::AllFunctor;
using concepts::AnyFunctor;
using concepts::Comparable;
using concepts::FilterFunctor;
using concepts::FoldFunctor;
using concepts::ForEachFunctor;
using concepts::InspectFunctor;
using concepts::Summable;

using iterator::Chain;
using iterator::Cycle;
using iterator::Filter;
using iterator::Inspect;
using iterator::Map;
using iterator::MovingWindow;
using iterator::Take;
using iterator::Zip;

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

    [[nodiscard]] auto advanceBy(size_t amount) -> Derived;

    template <class Functor>
        requires AnyFunctor<T, Functor>
    [[nodiscard]] auto any(Functor&& f) -> bool;

    template <class Functor>
        requires AllFunctor<T, Functor>
    [[nodiscard]] auto all(Functor&& f) -> bool;

    [[nodiscard]] auto collect() -> std::vector<T>;
    [[nodiscard]] auto count() -> size_t;
    [[nodiscard]] auto cycle() -> Cycle<T, Derived>;

    template <class Second>
    [[nodiscard]] auto chain(Second&& it) -> Chain<T, Derived, Second>;

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
        requires InspectFunctor<T, Functor>
    auto inspect(Functor&& f) -> Inspect<T, Functor, Derived>;

    template <class Functor>
        requires std::invocable<Functor, T>
    [[nodiscard]] auto map(Functor&& f) -> Map<T, Functor, Derived>;

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto max() -> std::optional<R>;

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto min() -> std::optional<R>;

    [[nodiscard]] auto movingWindow(size_t size) -> MovingWindow<T, Derived>;
    [[nodiscard]] auto nth(size_t element) -> std::optional<T>;

    template <class Functor>
        requires FoldFunctor<T, T, Functor>
    [[nodiscard]] auto reduce(Functor&& f) -> std::optional<T>;

    template <class R = T>
        requires Summable<R>
    [[nodiscard]] auto sum() -> R;

    [[nodiscard]] auto take(size_t amount) -> Take<T, Derived>;

    template <class Second>
    [[nodiscard]] auto zip(Second&& it) -> Zip<T, Derived, Second>;

  private:
    [[nodiscard]] inline auto self() -> Derived& { return static_cast<Derived&>(*this); }
    auto sizeHintChecked() -> size_t;
};
} // namespace rusty_iterators::interface

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::advanceBy(size_t amount) -> Derived
{
    for (size_t i = 0; i < amount; i++)
    {
        [[unlikely]] if (!self().next().has_value())
        {
            break;
        }
    }
    return std::move(self());
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::AnyFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::any(Functor&& f) -> bool
{
    // TODO: move this to use `tryFold` when implemented.
    sizeHintChecked();

    auto func     = std::forward<Functor>(f);
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        if (func(nextItem.value()))
        {
            return true;
        }
        nextItem = self().next();
    }
    return false;
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::AllFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::all(Functor&& f) -> bool
{
    // TODO: move this to use `tryFold` when implemented.
    sizeHintChecked();

    auto func     = std::forward<Functor>(f);
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        if (!func(nextItem.value()))
        {
            return false;
        }
        nextItem = self().next();
    }
    return true;
}

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
template <class Second>
auto rusty_iterators::interface::IterInterface<T, Derived>::chain(Second&& it)
    -> Chain<T, Derived, Second>
{
    return Chain<T, Derived, Second>{std::forward<Derived>(self()), std::forward<Second>(it)};
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
    requires rusty_iterators::concepts::InspectFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::inspect(Functor&& f)
    -> Inspect<T, Functor, Derived>
{
    return Inspect<T, Functor, Derived>{std::forward<Derived>(self()), std::forward<Functor>(f)};
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
auto rusty_iterators::interface::IterInterface<T, Derived>::movingWindow(size_t size)
    -> MovingWindow<T, Derived>
{
    return MovingWindow<T, Derived>{std::forward<Derived>(self()), size};
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::nth(size_t element) -> std::optional<T>
{
    return advanceBy(element).next();
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
template <class Second>
auto rusty_iterators::interface::IterInterface<T, Derived>::zip(Second&& it)
    -> Zip<T, Derived, Second>
{
    return Zip<T, Derived, Second>{std::forward<Derived>(self()), std::forward<Second>(it)};
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
