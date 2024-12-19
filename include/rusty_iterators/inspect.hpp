#pragma once

#include "concepts.hpp"
#include "interface.fwd.hpp"

#include <optional>
#include <string>

namespace rusty_iterators::iterator
{
using concepts::InspectFunctor;
using interface::IterInterface;

template <class T, class Functor, class Other>
    requires InspectFunctor<T, Functor>
class Inspect : public IterInterface<T, Inspect<T, Functor, Other>>
{
  public:
    Inspect(Other&& it, Functor&& f)
        : it(std::forward<Other>(it)), func(std::forward<Functor>(f)) {};

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

    friend auto operator<<(auto& os, Inspect<T, Functor, Other> const& m) -> std::ostream&
    {
        auto size    = m.sizeHint();
        auto sizeStr = size.has_value() ? std::to_string(size.value()) : "inf";

        return os << "Inspect{ size=" << sizeStr << ", it=" << m.it << " }";
    }

  private:
    Other it;
    Functor func;
};
} // namespace rusty_iterators::iterator

template <class T, class Functor, class Other>
    requires rusty_iterators::concepts::InspectFunctor<T, Functor>
auto rusty_iterators::iterator::Inspect<T, Functor, Other>::next() -> std::optional<T>
{
    auto nextItem = it.next();

    if (nextItem.has_value())
    {
        func(nextItem.value());
        return std::move(nextItem);
    }
    return std::nullopt;
}

template <class T, class Functor, class Other>
    requires rusty_iterators::concepts::InspectFunctor<T, Functor>
auto rusty_iterators::iterator::Inspect<T, Functor, Other>::sizeHint() const
    -> std::optional<size_t>
{
    // Inspect does not change the size of the underlying iterator.
    return it.sizeHint();
}
