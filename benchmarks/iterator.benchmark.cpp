#include <benchmark/benchmark.h>
#include <rusty_iterators/iterator.hpp>

#include <numeric>
#include <ranges>

using ::rusty_iterators::iterator::CycleType;
using ::rusty_iterators::iterator::LazyIterator;

constexpr size_t test_elements_amount = 10'000'000;

auto initializeIncrementalVector() -> std::vector<int>
{
    auto data = std::vector<int>(test_elements_amount);
    std::iota(data.begin(), data.end(), 0);

    return std::move(data);
}

auto benchmarkRustyIterFilterAndMap(benchmark::State& state) -> void
{
    auto data = initializeIncrementalVector();

    for (auto _ : state)
    {
        auto result = LazyIterator{data}
                          .filter([](auto x) { return x % 2 == 0; })
                          .map([](auto x) { return x * 2; })
                          .collect();
    }
}

auto benchmarkRustyIterFilterMap(benchmark::State& state) -> void
{
    auto data = initializeIncrementalVector();

    for (auto _ : state)
    {
        auto f = [](int x) -> std::optional<int> {
            if (x % 2 == 0)
                return std::make_optional(x * 2);
            return std::nullopt;
        };
        auto result = LazyIterator{data}.filterMap(std::move(f)).collect();
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

auto benchmarkRustyIterCopyCycle(benchmark::State& state) -> void
{
    auto data = std::vector{1, 2, 3};

    for (auto _ : state)
    {
        auto result =
            LazyIterator{data}.cycle<CycleType::Copy>().take(test_elements_amount).collect();
    }
}

auto benchmarkRustyIterCacheCycle(benchmark::State& state) -> void
{
    auto data = std::vector{1, 2, 3};

    for (auto _ : state)
    {
        auto result =
            LazyIterator{data}.cycle<CycleType::Cache>().take(test_elements_amount).collect();
    }
}

BENCHMARK(benchmarkRustyIterFilterAndMap);
BENCHMARK(benchmarkRustyIterFilterMap);
BENCHMARK(benchmarkRangesFilterTransform);
BENCHMARK(benchmarkRustyIterCopyCycle);
BENCHMARK(benchmarkRustyIterCacheCycle);
