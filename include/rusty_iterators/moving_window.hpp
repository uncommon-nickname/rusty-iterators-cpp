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
    using R = std::vector<T>;

  public:
    explicit MovingWindow(Other&& it, size_t size) : it(std::forward<Other>(it)), size(size)
    {
        if (size == 0)
        {
            throw std::length_error{"Moving window size must be greater that zero."};
        }
        cache.reserve(size);
    }

    friend auto operator<<(auto& os, MovingWindow<T, Other> const& m) -> std::ostream&
    {
        auto size    = m.sizeHint();
        auto sizeStr = size.has_value() ? std::to_string(size.value()) : "inf";

        return os << "MovingWindow{ windowSize=" << m.size << ", size=" << sizeStr
                  << ", it=" << m.it << " }";
    }

    auto next() -> std::optional<R>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    size_t size;
    uint8_t ptr      = 0;
    bool initialized = false;
    std::vector<T> cache{};

    auto initializeRingBuffer() -> bool;
    auto updateRingBuffer() -> bool;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::next() -> std::optional<R>
{
    if (initialized)
    {
        if (!updateRingBuffer())
        {
            return std::nullopt;
        }

        auto result = std::vector<T>{};
        result.reserve(size);

        for (uint8_t i = 0; i < size; i++)
        {
            ptr = ptr % size;
            result.push_back(cache.at(ptr));
            ptr += 1;
        }
        return std::make_optional(std::move(result));
    }
    if (!initializeRingBuffer())
    {
        return std::nullopt;
    }
    // The first element of the iterator is same as our cache.
    return std::make_optional(cache);
}

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto size = it.sizeHint();

    if (!size.has_value())
    {
        return std::nullopt;
    }
    return size.value() == 0 ? size : std::make_optional(size.value() - 1);
}

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::initializeRingBuffer() -> bool
{
    for (uint8_t i = 0; i < size; i++)
    {
        auto nextItem = it.next();

        if (!nextItem.has_value())
        {
            return false;
        }
        cache.push_back(std::move(nextItem.value()));
    }
    initialized = true;
    return true;
}

template <class T, class Other>
auto rusty_iterators::iterator::MovingWindow<T, Other>::updateRingBuffer() -> bool
{
    auto nextItem = it.next();

    if (!nextItem.has_value())
    {
        return false;
    }
    ptr           = ptr % size;
    cache.at(ptr) = std::move(nextItem.value());
    ptr += 1;
    return true;
}
