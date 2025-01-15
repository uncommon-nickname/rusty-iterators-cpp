#pragma once

#include "interface.fwd.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class First, class Second>
class Chain : public IterInterface<T, Chain<T, First, Second>>
{
  public:
    explicit Chain(First&& f, Second&& s)
        : first(std::forward<First>(f)), second(std::forward<Second>(s))
    {}

    [[nodiscard]] auto count() -> size_t;
    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    First first;
    Second second;
    bool useSecond = false;
};
} // namespace rusty_iterators::iterator

template <class T, class First, class Second>
auto rusty_iterators::iterator::Chain<T, First, Second>::count() -> size_t
{
    return first.count() + second.count();
}

template <class T, class First, class Second>
auto rusty_iterators::iterator::Chain<T, First, Second>::next() -> std::optional<T>
{
    if (useSecond)
        return second.next();

    auto nextItem = first.next();

    if (nextItem.has_value())
        return std::move(nextItem);

    useSecond = true;
    return next();
}

template <class T, class First, class Second>
auto rusty_iterators::iterator::Chain<T, First, Second>::sizeHint() const -> std::optional<size_t>
{
    // If one of the iterators is infinite, the full size is also infinite.
    auto sizeFirst  = first.sizeHint();
    auto sizeSecond = second.sizeHint();

    if (!sizeFirst.has_value() || !sizeSecond.has_value())
        return std::nullopt;

    return sizeFirst.value() + sizeSecond.value();
}
