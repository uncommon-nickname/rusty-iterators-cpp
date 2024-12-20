#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <string>
#include <tuple>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class First, class Second>
class Zip : public IterInterface<std::tuple<T, T>, Zip<T, First, Second>>
{
    using R = std::tuple<T, T>;

  public:
    explicit Zip(First&& f, Second&& s)
        : first(std::forward<First>(f)), second(std::forward<Second>(s))
    {}

    auto next() -> std::optional<R>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

    friend auto operator<<(auto& os, Zip<T, First, Second> const& m) -> std::ostream&
    {
        auto size    = m.sizeHint();
        auto sizeStr = size.has_value() ? std::to_string(size.value()) : "inf";

        return os << "Zip{ size=" << sizeStr << ", first=" << m.first << ", second=" << m.second
                  << " }";
    }

  private:
    First first;
    Second second;
};
} // namespace rusty_iterators::iterator

template <class T, class First, class Second>
auto rusty_iterators::iterator::Zip<T, First, Second>::next() -> std::optional<R>
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
    return std::make_optional(std::tuple<T, T>{firstItem.value(), secondItem.value()});
}

template <class T, class First, class Second>
auto rusty_iterators::iterator::Zip<T, First, Second>::sizeHint() const -> std::optional<size_t>
{
    auto firstSize  = first.sizeHint();
    auto secondSize = second.sizeHint();

    if (firstSize.has_value())
    {
        if (secondSize.has_value())
        {
            return std::make_optional(std::min(firstSize.value(), secondSize.value()));
        }
        return firstSize;
    }
    return secondSize;
}
