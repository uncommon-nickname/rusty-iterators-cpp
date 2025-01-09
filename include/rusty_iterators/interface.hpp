#pragma once

#include "chain.hpp"
#include "concepts.hpp"
#include "cycle.hpp"
#include "filter.hpp"
#include "filter_map.hpp"
#include "inspect.hpp"
#include "map.hpp"
#include "moving_window.hpp"
#include "take.hpp"
#include "zip.hpp"

#include <stdexcept>
#include <type_traits>
#include <vector>

namespace rusty_iterators::interface
{
using concepts::AllFunctor;
using concepts::AnyFunctor;
using concepts::Comparable;
using concepts::EqFunctor;
using concepts::FilterFunctor;
using concepts::FilterMapFunctor;
using concepts::FoldFunctor;
using concepts::ForEachFunctor;
using concepts::Indexable;
using concepts::InspectFunctor;
using concepts::PositionFunctor;
using concepts::Summable;
using concepts::TryFoldFunctor;
using concepts::TupleLike;

using iterator::CacheCycle;
using iterator::Chain;
using iterator::CopyCycle;
using iterator::CycleType;
using iterator::Filter;
using iterator::FilterMap;
using iterator::Inspect;
using iterator::Map;
using iterator::MovingWindow;
using iterator::Take;
using iterator::Zip;

template <class T, class Derived>
class IterInterface
{
  public:
    using Type = T;

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

    template <CycleType type = CycleType::Copy>
        requires(type == CycleType::Copy)
    [[nodiscard]] auto cycle() -> CopyCycle<T, Derived>;

    template <CycleType type>
        requires(type == CycleType::Cache)
    [[nodiscard]] auto cycle() -> CacheCycle<T, Derived>;

    template <class Second>
    [[nodiscard]] auto chain(Second&& it) -> Chain<T, Derived, Second>;

    template <class Other>
    [[nodiscard]] auto eq(Other&& it) -> bool;

    template <class Other, class Functor>
        requires EqFunctor<T, Functor>
    [[nodiscard]] auto eqBy(Other&& it, Functor&& f) -> bool;

    template <class Functor>
        requires FilterFunctor<T, Functor>
    [[nodiscard]] auto filter(Functor&& f) -> Filter<T, Functor, Derived>;

    template <class Functor>
        requires FilterMapFunctor<T, typename std::invoke_result_t<Functor, T>::value_type, Functor>
    [[nodiscard]] auto filterMap(Functor&& f)
        -> FilterMap<T, typename std::invoke_result_t<Functor, T>::value_type, Functor, Derived>;

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
        requires PositionFunctor<T, Functor>
    [[nodiscard]] auto position(Functor&& f) -> std::optional<size_t>;

    template <class Functor>
        requires FoldFunctor<T, T, Functor>
    [[nodiscard]] auto reduce(Functor&& f) -> std::optional<T>;

    template <class R = T>
        requires Summable<R>
    [[nodiscard]] auto sum() -> R;

    [[nodiscard]] auto take(size_t amount) -> Take<T, Derived>;

    template <class B, class Functor>
        requires TryFoldFunctor<B, T, Functor>
    [[nodiscard]] auto tryFold(B&& init, Functor&& f) -> B;

    template <class R = T>
        requires TupleLike<R>
    [[nodiscard]] auto unzip() -> std::tuple<std::vector<typename std::tuple_element<0, R>::type>,
                                             std::vector<typename std::tuple_element<1, R>::type>>;

    template <class R = T>
        requires Indexable<R>
    [[nodiscard]] auto unzip()
        -> std::tuple<std::vector<typename R::value_type>, std::vector<typename R::value_type>>;

    template <class Second>
    [[nodiscard]] auto zip(Second&& it) -> Zip<T, typename Second::Type, Derived, Second>;

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
            break;
    }
    return std::move(self());
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::AnyFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::any(Functor&& f) -> bool // NOLINT
{
    auto anyf = [f = std::forward<Functor>(f)](bool acc, T x) {
        return f(x) ? std::expected<bool, bool>{true} : std::unexpected{false};
    };
    return self().tryFold(false, std::move(anyf));
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::AllFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::all(Functor&& f) -> bool // NOLINT
{
    auto allf = [f = std::forward<Functor>(f)](bool acc, T x) {
        return !f(x) ? std::expected<bool, bool>{false} : std::unexpected{true};
    };
    return self().tryFold(true, std::move(allf));
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
    size_t count = 0;

    forEach([&count](auto&& _) { count += 1; });

    return count;
}

template <class T, class Derived>
template <rusty_iterators::iterator::CycleType type>
    requires(type == rusty_iterators::iterator::CycleType::Copy)
auto rusty_iterators::interface::IterInterface<T, Derived>::cycle() -> CopyCycle<T, Derived>
{
    return CopyCycle<T, Derived>{std::forward<Derived>(self())};
}

template <class T, class Derived>
template <rusty_iterators::iterator::CycleType type>
    requires(type == rusty_iterators::iterator::CycleType::Cache)
auto rusty_iterators::interface::IterInterface<T, Derived>::cycle() -> CacheCycle<T, Derived>
{
    return CacheCycle<T, Derived>{std::forward<Derived>(self())};
}

template <class T, class Derived>
template <class Second>
auto rusty_iterators::interface::IterInterface<T, Derived>::chain(Second&& it)
    -> Chain<T, Derived, Second>
{
    return Chain<T, Derived, Second>{std::forward<Derived>(self()), std::forward<Second>(it)};
}

template <class T, class Derived>
template <class Other>
auto rusty_iterators::interface::IterInterface<T, Derived>::eq(Other&& it) -> bool
{
    return self().eqBy(std::forward<Other>(it),
                       [](auto x) { return std::get<0>(x) == std::get<1>(x); });
}

template <class T, class Derived>
template <class Other, class Functor>
    requires rusty_iterators::concepts::EqFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::eqBy(Other&& it, Functor&& f) -> bool
{
    return self().zip(std::forward<Other>(it)).all(std::forward<Functor>(f));
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
    requires rusty_iterators::concepts::FilterMapFunctor<
        T, typename std::invoke_result_t<Functor, T>::value_type, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::filterMap(Functor&& f)
    -> FilterMap<T, typename std::invoke_result_t<Functor, T>::value_type, Functor, Derived>
{
    return FilterMap<T, typename std::invoke_result_t<Functor, T>::value_type, Functor, Derived>{
        std::forward<Derived>(self()), std::forward<Functor>(f)};
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

    [[likely]] while (nextItem.has_value())
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
    return self().reduce([](auto x, auto y) { return std::max(x, y); });
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Comparable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::min() -> std::optional<R>
{
    return self().reduce([](auto x, auto y) { return std::min(x, y); });
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
    return self().advanceBy(element).next();
}

template <class T, class Derived>
template <class Functor>
    requires rusty_iterators::concepts::PositionFunctor<T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::position(Functor&& f) // NOLINT
    -> std::optional<size_t>
{
    sizeHintChecked();

    auto func       = std::forward<Functor>(f);
    auto nextItem   = self().next();
    size_t position = 0;

    [[likely]] while (nextItem.has_value())
    {
        if (func(nextItem.value()))
        {
            return std::make_optional(position);
        }
        position += 1;
        nextItem = self().next();
    }
    return std::nullopt;
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
    return self().fold(R{}, [](auto acc, auto x) { return acc + x; });
}

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::take(size_t amount) -> Take<T, Derived>
{
    return Take<T, Derived>{std::forward<Derived>(self()), amount};
}

template <class T, class Derived>
template <class B, class Functor>
    requires rusty_iterators::concepts::TryFoldFunctor<B, T, Functor>
auto rusty_iterators::interface::IterInterface<T, Derived>::tryFold(B&& init, Functor&& f) -> B
{
    sizeHintChecked();

    auto func     = std::forward<Functor>(f);
    auto accum    = std::forward<B>(init);
    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        auto potentialAccum = func(std::move(accum), std::move(nextItem.value()));

        if (potentialAccum.has_value())
        {
            return potentialAccum.value();
        }
        accum    = potentialAccum.error();
        nextItem = self().next();
    }
    return std::move(accum);
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::TupleLike<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::unzip()
    -> std::tuple<std::vector<typename std::tuple_element<0, R>::type>,
                  std::vector<typename std::tuple_element<1, R>::type>>
{
    auto size = sizeHintChecked();

    auto first  = std::vector<typename std::tuple_element<0, R>::type>{};
    auto second = std::vector<typename std::tuple_element<1, R>::type>{};

    first.reserve(size);
    second.reserve(size);

    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        auto [f, s] = nextItem.value();
        first.push_back(std::move(f));
        second.push_back(std::move(s));
        nextItem = self().next();
    }
    return std::move(std::tuple{std::move(first), std::move(second)});
}

template <class T, class Derived>
template <class R>
    requires rusty_iterators::concepts::Indexable<R>
auto rusty_iterators::interface::IterInterface<T, Derived>::unzip()
    -> std::tuple<std::vector<typename R::value_type>, std::vector<typename R::value_type>>
{
    auto size = sizeHintChecked();

    auto first  = std::vector<typename R::value_type>{};
    auto second = std::vector<typename R::value_type>{};

    first.reserve(size);
    second.reserve(size);

    auto nextItem = self().next();

    [[likely]] while (nextItem.has_value())
    {
        auto value = nextItem.value();
        first.push_back(std::move(value.at(0)));
        second.push_back(std::move(value.at(1)));
        nextItem = self().next();
    }

    return std::move(std::tuple{std::move(first), std::move(second)});
}

template <class T, class Derived>
template <class Second>
auto rusty_iterators::interface::IterInterface<T, Derived>::zip(Second&& it)
    -> Zip<T, typename Second::Type, Derived, Second>
{
    return Zip<T, typename Second::Type, Derived, Second>{std::forward<Derived>(self()),
                                                          std::forward<Second>(it)};
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
