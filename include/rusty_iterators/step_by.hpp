#pragma once

#include "interface.fwd.hpp"

#include <optional>
#include <stdexcept>

namespace rusty_iterators::iterator
{
using interface::IterInterface;

template <class T, class Other>
class StepBy : public IterInterface<T, StepBy<T, Other>>
{
  public:
    explicit StepBy(Other&& it, size_t step) : it(std::forward<Other>(it))
    {
        if (step == 0)
            throw std::length_error{"Step has to be greater than zero."};

        stepMinusOne = step - 1;
    }

    auto next() -> std::optional<T>;
    [[nodiscard]] auto sizeHint() const -> std::optional<size_t>;

  private:
    Other it;
    size_t stepMinusOne;
    bool firstTake = true;

    [[nodiscard]] inline auto originalStep() const -> size_t { return stepMinusOne + 1; }

    [[nodiscard]] inline auto firstSize(size_t underlyingSize) const -> size_t
    {
        auto step = originalStep();
        return underlyingSize == 0 ? 0 : 1 + (underlyingSize - 1) / step;
    }

    [[nodiscard]] inline auto otherSize(size_t underlyingSize) const -> size_t
    {
        auto step = originalStep();
        return underlyingSize / step;
    }
};
} // namespace rusty_iterators::iterator

template <class T, class Other>
auto rusty_iterators::iterator::StepBy<T, Other>::next() -> std::optional<T>
{
    auto stepSize = firstTake ? 0 : stepMinusOne;
    firstTake     = false;

    return std::move(it.nth(stepSize));
}

template <class T, class Other>
auto rusty_iterators::iterator::StepBy<T, Other>::sizeHint() const -> std::optional<size_t>
{
    auto underlyingSize = it.sizeHint();

    if (!underlyingSize.has_value())
        return std::nullopt;

    if (firstTake)
        return firstSize(underlyingSize.value());

    return otherSize(underlyingSize.value());
}
