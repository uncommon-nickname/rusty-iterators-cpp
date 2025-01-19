#pragma once

#include "interface.fwd.hpp"
#include "peekable.hpp"

#include <algorithm>
#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Interperse : public IterInterface<T, Interperse<T, Other>>
{
  public:
    explicit Interperse(Other&& it, T&& item)
        : it(std::forward<Peekable<T, Other>>(it.peekable())), item(std::forward<T>(item))
    {}

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Peekable<T, Other> it;
    T item;
    bool returnInterperseValue = false;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Interperse<T, Other>::next() -> std::optional<T>
{
    /// NOTE: 18.01.2025 <@uncommon-nickname>
    /// We need to check if iterator is finished. By using a peekable
    /// iterator, we can do that without any significant cost, because
    /// we will advance the iterator anyway later.
    auto peeked = it.peek();

    [[unlikely]] if (!peeked.has_value())
        return std::nullopt;

    if (returnInterperseValue)
    {
        returnInterperseValue = false;
        return item;
    }
    returnInterperseValue = true;
    return std::move(it.next());
}

template <class T, class Other>
auto rusty_iterators::iterator::Interperse<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto underlyingSize = it.sizeHint();

    if (!underlyingSize.has_value())
        return std::nullopt;

    int32_t potentialSize = (underlyingSize.value() * 2) - 1;

    return std::max(0, potentialSize);
}
