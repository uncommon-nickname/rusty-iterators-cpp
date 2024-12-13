#include "iterator_interface.hpp"

#include <optional>

namespace rusty_iterators::iterator
{
template <template <class...> class Container, class Item>
    requires std::ranges::range<Container<Item>>
class Iterator : public interface::IterInterface<Item, Iterator<Container, Item>>
{
  public:
    explicit Iterator(Container<Item>& it) : head(it.begin()), tail(it.end() - 1), size(it.size())
    {}

    [[nodiscard]] auto nextFront() -> std::optional<Item>;
    [[nodiscard]] auto nextBack() -> std::optional<Item>;
    [[nodiscard]] auto sizeHint() const -> size_t;

  private:
    Container<Item>::iterator head;
    Container<Item>::iterator tail;
    size_t size;
};
} // namespace rusty_iterators::iterator

template <template <class...> class Container, class Item>
    requires std::ranges::range<Container<Item>>
auto rusty_iterators::iterator::Iterator<Container, Item>::nextFront() -> std::optional<Item>
{
    if (head == tail + 1)
    {
        return std::nullopt;
    }

    auto item = *head;
    head += 1;

    return std::make_optional(item);
}

template <template <class...> class Container, class Item>
    requires std::ranges::range<Container<Item>>
auto rusty_iterators::iterator::Iterator<Container, Item>::nextBack() -> std::optional<Item>
{
    if (tail == head - 1)
    {
        return std::nullopt;
    }

    auto item = *tail;
    tail -= 1;

    return std::make_optional(item);
}

template <template <class...> class Container, class Item>
    requires std::ranges::range<Container<Item>>
auto rusty_iterators::iterator::Iterator<Container, Item>::sizeHint() const -> size_t
{
    return size;
}
