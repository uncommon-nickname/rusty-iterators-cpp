#pragma once

#include <vector>

namespace rusty_iterators::interface
{
template <class T, class Derived>
class IterInterface
{
  public:
    IterInterface()          = default;
    virtual ~IterInterface() = default;

    IterInterface(IterInterface&)                  = default;
    IterInterface& operator=(IterInterface const&) = default;
    IterInterface(IterInterface&&)                 = default;
    IterInterface& operator=(IterInterface&&)      = default;

    [[nodiscard]] auto collect() -> std::vector<T>;

  private:
    [[nodiscard]] inline auto child() -> Derived& { return static_cast<Derived&>(*this); }
};
} // namespace rusty_iterators::interface

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::collect() -> std::vector<T>
{
    auto collection = std::vector<T>{};
    auto size       = child().sizeHint();

    collection.reserve(size);

    auto nextItem = child().nextFront();

    [[likely]] while (nextItem.has_value())
    {
        collection.push_back(nextItem.value());
        nextItem = child().nextFront();
    }

    return std::move(collection);
}
