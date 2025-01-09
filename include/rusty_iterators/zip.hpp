#pragma once

#include "interface.fwd.hpp"

#include <algorithm>
#include <optional>
#include <tuple>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class R, class First, class Second>
class Zip : public IterInterface<std::tuple<T, R>, Zip<T, R, First, Second>>
{
  public:
    explicit Zip(First&& f, Second&& s)
        : first(std::forward<First>(f)), second(std::forward<Second>(s))
    {}

    [[nodiscard]] auto advanceBy(size_t amount) -> Zip<T, R, First, Second>;
    auto next() -> std::optional<std::tuple<T, R>>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    First first;
    Second second;
};
} // namespace rusty_iterators::iterator

template <class T, class R, class First, class Second>
auto rusty_iterators::iterator::Zip<T, R, First, Second>::advanceBy(size_t amount)
    -> Zip<T, R, First, Second>
{
    first  = first.advanceBy(amount);
    second = second.advanceBy(amount);

    return std::move(static_cast<Zip<T, R, First, Second>&>(*this));
}

template <class T, class R, class First, class Second>
auto rusty_iterators::iterator::Zip<T, R, First, Second>::next() -> std::optional<std::tuple<T, R>>
{
    auto firstItem = first.next();

    if (!firstItem.has_value())
    {
        return std::nullopt;
    }
    auto secondItem = second.next();

    if (!secondItem.has_value())
    {
        return std::nullopt;
    }
    return std::tuple<T, R>{firstItem.value(), secondItem.value()};
}

template <class T, class R, class First, class Second>
auto rusty_iterators::iterator::Zip<T, R, First, Second>::sizeHint() const -> std::optional<size_t>
{
    auto firstSize  = first.sizeHint();
    auto secondSize = second.sizeHint();

    if (firstSize.has_value())
    {
        if (secondSize.has_value())
        {
            return std::min(firstSize.value(), secondSize.value());
        }
        return firstSize;
    }
    return secondSize;
}
