#pragma once

#include "concepts.hpp"
#include "interface.hpp"

#include <functional>
#include <optional>

namespace
{
template <class Container>
    requires std::ranges::range<Container>
using Item = std::reference_wrapper<const typename Container::value_type>;
} // namespace

namespace rusty_iterators::iterator
{
using concepts::FoldFunctor;
using concepts::Multiplyable;
using concepts::Summable;

template <class Container>
    requires std::ranges::range<Container>
class LazyIterator : public interface::IterInterface<Item<Container>, LazyIterator<Container>>
{
    using RawT     = typename Container::value_type;
    using T        = Item<Container>;
    using Iterator = typename Container::iterator;

  public:
    explicit LazyIterator(Container& it) : ptr(it.begin()), end(it.end()) {}

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

    template <class R = RawT>
        requires Summable<R>
    [[nodiscard]] auto sum() -> R;

  private:
    Iterator ptr;
    Iterator end;
};
} // namespace rusty_iterators::iterator

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::LazyIterator<Container>::next() -> std::optional<T>
{
    [[unlikely]] if (ptr == end)
        return std::nullopt;

    auto& item = *ptr;
    ptr += 1;

    return std::cref(item);
}

template <class Container>
    requires std::ranges::range<Container>
auto rusty_iterators::iterator::LazyIterator<Container>::sizeHint() const -> std::optional<size_t>
{
    return end - ptr;
}

template <class Container>
    requires std::ranges::range<Container>
template <class R>
    requires rusty_iterators::concepts::Summable<R>
auto rusty_iterators::iterator::LazyIterator<Container>::sum() -> R
{
    return this->fold(RawT{}, [](auto acc, auto x) { return acc + x; });
}
