#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <stdexcept>
#include <vector>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class MovingWindow : public IterInterface<std::vector<T>, MovingWindow<T, Other>>
{
  public:
    explicit MovingWindow(Other&& it, size_t size)
        : it(std::forward<Other>(it)), orig(this->it), size(size)
    {
        if (size == 0)
        {
            throw std::length_error{"Moving window size must be greater that zero."};
        }
    }

    auto next() -> std::optional<std::vector<T>>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

    friend auto operator<<(auto& os, MovingWindow<T, Other> const& m) -> std::ostream&
    {
        auto size    = m.sizeHint();
        auto sizeStr = size.has_value() ? std::to_string(size.value()) : "inf";

        return os << "MovingWindow{ windowSize=" << m.size << ", size=" << sizeStr
                  << ", it=" << m.it << ", orig=" << m.orig << " }";
    }

  private:
    Other it;
    Other orig;
    size_t size;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::next() -> std::optional<std::vector<T>>
{
    auto result = std::vector<T>{};
    result.reserve(size);

    for (size_t i = 0; i < size; i++)
    {
        auto nextItem = it.next();

        [[unlikely]] if (!nextItem.has_value())
        {
            return std::nullopt;
        }
        result.push_back(std::move(nextItem.value()));
    }

    orig.next();
    it = orig;

    return std::make_optional(std::move(result));
}

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::sizeHint() const -> std::optional<size_t>
{
    // Moving windows are always one element shorter than the original
    // iterator (if it is not empty or infinite of course).
    auto size = it.sizeHint();

    if (!size.has_value())
    {
        return std::nullopt;
    }
    return size.value() == 0 ? size : std::make_optional(size.value() - 1);
}
