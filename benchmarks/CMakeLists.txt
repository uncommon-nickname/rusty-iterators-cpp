include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.zip
)
FetchContent_MakeAvailable(googlebenchmark)

file(GLOB BENCHMARKS "*.cpp")
add_executable(rusty_iterators_benchmarks ${BENCHMARKS})

target_link_libraries(
    rusty_iterators_benchmarks
    rusty_iterators
    benchmark::benchmark
    benchmark::benchmark_main
)
