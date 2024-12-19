# Rusty iterators

This is a small proof of concept showing a port of Rust iterator interface to modern C++. Is it better than C++ stdlib iterators? Probably not, but it is cool.

## Coding code of conduct

Any contributions are welcome, but please adhere to our code of conduct.

### Conventional commits

We utilize [conventional commits](https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13). We don't maintain a `changelog` manually. Squashed commits, keeping the correct convention should be more than enough to create a release summary.

### Linting and formatting

We use both `clang-tidy` and `clang-format`. The former is integrated with CMake and will be run during build. The latter can be run using provided formatting script.

```bash
./format.sh
```

## Building the project

We provide a small utility script to build the project. We use both CMake and Ninja, so make sure you have those installed.

```bash
# Clean build using Clang:
./build.sh --clang --no-cache --compile-tests

# Clean build using GCC:
./build.sh --no-cache --compile-tests

# Cached build:
./build.sh --clang --compile-tests
```

## Examples

Some simple examples of usage.

### Count all even numbers in the iterator

```c++
auto data = std::vector{1, 2, 3, 4, 5, 6};
auto result = RustyIter{data}.filter([](auto x) { return x % 2 == 0; }).count();
```

### Get `n` items from cycle iterator

```c++
auto data = std::vector{1, 2, 3};
auto result = RustyIter{data}.cycle().take(10).collect();
```

### Run a callable on every element of the iterator

```c++
auto data = std::vector{1, 2, 3, 4};
auto f = [](auto x){ ... } // Do some business logic.

RustyIter{data}.forEach(std::move(f));
```

### Find the length of the shortest string in the iterator

```c++
auto data = std::vector<std::string>{"a", "abc", "defg"};
auto result = RustyIter{data}.map([](auto x) { return x.get().size(); }).min();
```

### Filter out all even numbers and square the odd numbers

```c++
auto data = std::vector{1, 2, 3, 4};
auto result = RustyIter{data}.filter([](auto x) { return x % 2 != 0; }).map([](auto x) { return x * x; }).collect();
```

## Benchmarks

We provide a small set of benchmarks located in the `benchmarks/` directory. You can build them using provided build script.

```bash
./build.sh --clang --compile-benchmarks
```

All of the benchmarks measure the performance in release mode and run similar scenario using C++20 `std::ranges` to compare how fast, or how slow we are.

### Current measurements

All of the measurements were taken on MacBook M3 Pro 18GB.

#### Apply filter and map on 10 000 000 ints

|          **Benchmark**         | **Time [ns]** | **CPU [ns]** |
|:------------------------------:|:-------------:|:------------:|
|   benchmarkRustyIterFilterMap  |    5565001    |    5564975   |
| benchmarkRangesFilterTransform |    5837398    |    5837397   |

## Authors

- [Wiktor Nowak](@uncommon-nickname)
