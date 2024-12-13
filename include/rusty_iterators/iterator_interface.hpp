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
};
} // namespace rusty_iterators::interface

template <class T, class Derived>
auto rusty_iterators::interface::IterInterface<T, Derived>::collect() -> std::vector<T>
{
    auto collection = std::vector<T>{};
    auto size       = static_cast<Derived*>(this)->sizeHint();

    collection.reserve(size);

    while (true)
    {
        auto nextItem = static_cast<Derived*>(this)->nextFront();

        if (nextItem.has_value())
        {
            collection.push_back(nextItem.value());
            continue;
        }
        break;
    }

    return std::move(collection);
}
