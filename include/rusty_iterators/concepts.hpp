#pragma once

#include <concepts>

namespace rusty_iterators::concepts
{
template <class T, class Functor>
concept FilterFunctor = requires(Functor f, T t) {
    { f(t) } -> std::same_as<bool>;
};

template <class B, class T, class Functor>
concept FoldFunctor = requires(Functor f, B first, T second) {
    { f(first, second) } -> std::same_as<B>;
};

template <class T, class Functor>
concept ForEachFunctor = requires(Functor f, T t) {
    { f(t) } -> std::same_as<void>;
};

template <class T>
concept Summable = requires(T first, T second) {
    { first + second } -> std::same_as<T>;
};

template <class T>
concept Comparable = requires(T first, T second) {
    { first > second } -> std::same_as<bool>;
    { first < second } -> std::same_as<bool>;
};
} // namespace rusty_iterators::concepts
