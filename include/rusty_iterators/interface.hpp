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

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto max() -> std::optional<R>;

    template <class R = T>
        requires Comparable<R>
    [[nodiscard]] auto min() -> std::optional<R>;

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
template <class R>
    requires rusty_iterators::concepts::Comparable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::max() -> std::optional<R>
{
    sizeHintChecked();

    auto maxItem = self().next();

    if (!maxItem.has_value())
    {
        return std::nullopt;
    }
    while (true)
    {
        auto nextItem = self().next();

        if (!nextItem.has_value())
        {
            break;
        }
        if (nextItem.value() > maxItem.value())
        {
            maxItem = nextItem;
        }
    }
    return maxItem;
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Comparable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::min() -> std::optional<R>
{
    sizeHintChecked();

    auto minItem = self().next();

    if (!minItem.has_value())
    {
        return std::nullopt;
    }
    while (true)
    {
        auto nextItem = self().next();

        if (!nextItem.has_value())
        {
            break;
        }
        if (nextItem.value() < minItem.value())
        {
            minItem = nextItem;
        }
    }
    return minItem;
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Summable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::sum() -> R
{
    sizeHintChecked();

    auto sum = R{};

    while (true)
    {
        auto nextItem = self().next();

        if (!nextItem.has_value())
        {
            break;
        }
        sum += nextItem.value();
    }
    return std::move(sum);
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
