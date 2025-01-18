#pragma once

#include "interface.fwd.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Peekable : public IterInterface<T, Peekable<T, Other>>
{
  public:
    explicit Peekable(Other&& it) : it(std::forward<Other>(it)) {}

    auto next() -> std::optional<T>;
    auto peek() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    std::optional<std::optional<T>> peeked = std::nullopt;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Peekable<T, Other>::next() -> std::optional<T>
{
    if (peeked.has_value())
    {
        auto item = peeked.value();
        peeked    = std::nullopt;
        return std::move(item);
    }
    return std::move(it.next());
}

template <class T, class Other>
auto rusty_iterators::iterator::Peekable<T, Other>::peek() -> std::optional<T>
{
    if (peeked.has_value())
        return peeked.value();

    peeked = std::make_optional(it.next());
    return peeked.value();
}

template <class T, class Other>
auto rusty_iterators::iterator::Peekable<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto underlyingSize = it.sizeHint();

    if (!underlyingSize.has_value())
        return std::move(underlyingSize);

    auto fullSize = underlyingSize.value();

    if (peeked.has_value() && peeked.value().has_value())
        fullSize += 1;

    return fullSize;
}
