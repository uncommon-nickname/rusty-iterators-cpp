#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <vector>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

enum class CycleType : uint8_t
{
    Copy,
    Cache,
};

template <class T, class Other>
class CopyCycle : public IterInterface<T, CopyCycle<T, Other>>
{
  public:
    explicit CopyCycle(Other&& it) : it(std::forward<Other>(it)), original(this->it) {};

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    Other original;
};

template <class T, class Other>
class CacheCycle : public IterInterface<T, CacheCycle<T, Other>>
{
  public:
    explicit CacheCycle(Other&& it) : it(std::forward<Other>(it)) {};

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    std::vector<T> cache{};
    bool useCache = false;
    size_t ptr    = 0;
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::CopyCycle<T, Other>::next() -> std::optional<T>
{
    auto item = it.next();

    if (!item.has_value())
    {
        it = original;
        return it.next();
    }
    return item;
}

template <class T, class Other>
auto rusty_iterators::iterator::CopyCycle<T, Other>::sizeHint() const -> std::optional<size_t>
{
    // Cycle iterator is either infinite or empty.
    auto size = it.sizeHint();

    if (size.has_value() && size.value() > 0)
    {
        return std::nullopt;
    }
    return 0;
}

template <class T, class Other>
auto rusty_iterators::iterator::CacheCycle<T, Other>::next() -> std::optional<T>
{
    if (useCache)
    {
        ptr %= cache.size();
        auto nextItem = cache.at(ptr);
        ptr += 1;
        return std::make_optional(nextItem);
    }

    auto nextItem = this->it.next();

    if (!nextItem.has_value())
    {
        if (cache.size() == 0)
        {
            // If cycle is empty, we don't want to go into cached logic.
            return std::nullopt;
        }
        useCache = true;
        return next();
    }

    cache.push_back(nextItem.value());
    return nextItem;
}

template <class T, class Other>
auto rusty_iterators::iterator::CacheCycle<T, Other>::sizeHint() const -> std::optional<size_t>
{
    // Cycle iterator is either infinite or empty.
    auto size = it.sizeHint();

    if (size.has_value() && size.value() > 0)
    {
        return std::nullopt;
    }
    return 0;
}
