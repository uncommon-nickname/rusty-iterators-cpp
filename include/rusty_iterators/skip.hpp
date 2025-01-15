#pragma once

#include "interface.fwd.hpp"

#include <algorithm>
#include <optional>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class Skip : public IterInterface<T, Skip<T, Other>>
{
  public:
    explicit Skip(Other&& it, size_t n) : it(std::forward<Other>(it)), n(n) {}

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    size_t n;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::Skip<T, Other>::next() -> std::optional<T>
{
    [[unlikely]] if (n > 0)
    {
        auto item = it.nth(n);
        n         = 0;
        return std::move(item);
    }
    return std::move(it.next());
}

template <class T, class Other>
auto rusty_iterators::iterator::Skip<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto underlyingSize = it.sizeHint();

    if (!underlyingSize.has_value())
    {
        return std::nullopt;
    }
    int32_t estimatedSize = underlyingSize.value() - n;
    return std::max(0, estimatedSize);
}
