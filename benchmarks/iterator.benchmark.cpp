#include <benchmark/benchmark.h>
#include <rusty_iterators/iterator.hpp>

#include <numeric>
#include <ranges>

using rusty_iterators::iterator::RustyIter;

constexpr size_t test_elements_amount = 10'000'000;

auto initializeIncrementalVector() -> std::vector<int>
{
    auto data = std::vector<int>(test_elements_amount);
    std::iota(data.begin(), data.end(), 0);

    return std::move(data);
}

auto benchmarkRustyIterFilterMap(benchmark::State& state) -> void
{
    auto data = initializeIncrementalVector();

    for (auto _ : state)
    {
        auto result = RustyIter{data}
                          .filter([](auto x) { return x % 2 == 0; })
                          .map([](auto x) { return x * 2; })
                          .collect();
    }
}

auto benchmarkRangesFilterTransform(benchmark::State& state) -> void
{
    auto data = initializeIncrementalVector();

    for (auto _ : state)
    {
        auto it = data | std::views::filter([](auto x) { return x % 2 == 0; }) |
                  std::views::transform([](auto x) { return x * 2; });
        auto result = std::vector<int>{it.begin(), it.end()};
    }
}

auto benchmarkRustyIterCycle(benchmark::State& state) -> void
{
    auto data = std::vector{1, 2, 3};

    for (auto _ : state)
    {
        auto result = RustyIter{data}.cycle().take(test_elements_amount).collect();
    }
}

auto benchmarkRustyIterMovingWindow(benchmark::State& state) -> void
{
    auto data = initializeIncrementalVector();

    for (auto _ : state)
    {
        auto result = RustyIter{data}.movingWindow(3).collect();
    }
}

BENCHMARK(benchmarkRustyIterFilterMap);
BENCHMARK(benchmarkRangesFilterTransform);
BENCHMARK(benchmarkRustyIterCycle);
BENCHMARK(benchmarkRustyIterMovingWindow);
