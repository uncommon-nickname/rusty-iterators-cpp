#pragma once

#include <expected>
#include <optional>
#include <tuple>

namespace rusty_iterators::concepts
{
template <class T, class Functor>
concept AllFunctor = requires(Functor f, T&& t) {
    { f(t) } -> std::same_as<bool>;
};

template <class T, class Functor>
concept AnyFunctor = AllFunctor<T, Functor>;

template <class T>
concept Comparable = requires(T first, T second) {
    { first > second } -> std::same_as<bool>;
    { first < second } -> std::same_as<bool>;
};

template <class T, class Functor>
concept EqFunctor = requires(Functor f, std::tuple<T, T> t) {
    { f(t) } -> std::same_as<bool>;
};

template <class T, class Functor>
concept NeFunctor = EqFunctor<T, Functor>;

template <class T, class Functor>
concept FilterFunctor = AllFunctor<T&, Functor>;

template <class Tin, class Tout, class Functor>
concept FilterMapFunctor = requires(Functor f, Tin&& t) {
    { f(t) } -> std::same_as<std::optional<Tout>>;
};

template <class B, class T, class Functor>
concept FoldFunctor = requires(Functor f, B&& accumulator, T&& item) {
    { f(accumulator, item) } -> std::same_as<B>;
};

template <class T, class Functor>
concept ForEachFunctor = requires(Functor f, T t) {
    { f(t) } -> std::same_as<void>;
};

template <class T>
concept Indexable = requires(T t) {
    typename T::value_type;
    t.at(0);
};

template <class T, class Functor>
concept InspectFunctor = requires(Functor f, T& t) {
    { f(t) } -> std::same_as<void>;
};

template <class T, class Functor>
concept PositionFunctor = AllFunctor<T, Functor>;

template <class T>
concept Summable = requires(T first, T second) {
    { first + second } -> std::same_as<T>;
};

template <class B, class T, class Functor>
concept TryFoldFunctor = requires(Functor f, B&& accumulator, T&& item) {
    { f(accumulator, item) } -> std::same_as<std::expected<B, B>>;
};

template <class T>
concept TupleLike = requires(T t) {
    std::tuple_size<T>::value == 2;
    std::get<0>(t);
};
} // namespace rusty_iterators::concepts
