# Rusty iterators

This is a small proof of concept showing a port of Rust iterator interface to modern C++. Is it better than C++ stdlib iterators? Probably not, but it is cool.

## Coding code of conduct

Any contributions are welcome, but please adhere to our code of conduct.

### Conventional commits

We utilize [conventional commits](https://gist.github.com/qoomon/5dfcdf8eec66a051ecd85625518cfd13). We don't maintain a `changelog` manually. Squashed commits, keeping the correct convention should be more than enough to create a release summary.

### Linting and formatting

We use both `clang-tidy` and `clang-format`. The former is integrated with CMake and will be run during build. The latter can be run using provided formatting script.

```bash
./tools/format.sh
```

## Building the project

We provide a small utility script to build the project. We use both CMake and Ninja, so make sure you have those installed.

```bash
# Clean build:
./tools/build.sh --no-cache --compile-tests

# Build with specified compiler:
./tools/build.sh --compiler g++-14 --compile-tests --no-cache

# Cached build:
./tools/build.sh --compile-tests

# Build in release mode
./tools/build.sh --compile-tests --release
```

## Examples

Some actual real-life inspired examples can be found in the `examples/` directory. You can build them using provided build script:

```bash
./tools/build.sh --compile-examples
```

Here we will list some of the simple examples, to show you the power of lazy iteration!

### Count all even numbers in the iterator

```c++
auto data = std::vector{1, 2, 3, 4, 5, 6};
auto result = LazyIterator{data}
                .filter([](auto x) { return x % 2 == 0; })
                .count();
```

### Get `n` items from cycle iterator

```c++
auto data = std::vector{1, 2, 3};
auto result = LazyIterator{data}.cycle().take(10).collect();
```

### Run a callable on every element of the iterator

```c++
auto data = std::vector{1, 2, 3, 4};
auto f = [](auto x){ ... } // Do some business logic.

LazyIterator{data}.forEach(std::move(f));
```

### Find the length of the shortest string in the iterator

```c++
auto data = std::vector<std::string>{"a", "abc", "defg"};
auto result = LazyIterator{data}
                .map([](auto x) { return x.get().size(); })
                .min();
```

### Filter out all even numbers and square the odd numbers

```c++
auto data = std::vector{1, 2, 3, 4};
auto result = LazyIterator{data}
                .filter([](auto x) { return x % 2 != 0; })
                .map([](auto x) { return x * x; })
                .collect();
```

### Chain two iterators

```c++
auto v1 = std::vector{1, 2, 3};
auto v2 = std::vector{4, 5, 6};

auto result = LazyIterator{v1}
                .chain(LazyIterator{v2}
                .map([](auto x) { return x * 2; }))
                .collect();
```

### Find biggest difference between elements of two iterators

```c++
auto v1 = std::vector{1, 2, 3, 4, 5};
auto v2 = std::vector{2, 7, 4, 8, 1};

auto result = LazyIterator{v1}
                    .zip(LazyIterator{v2})
                    .map([](auto x) { return std::abs(std::get<0>(x) - std::get<1>(x)); })
                    .max();
```

### Parse file containing numbers in lazy manner

```c++
auto fileName = std::string{"test.txt"};
auto numbers = FileIterator<FIterType::Lazy>{fileName}
                    .filter([](auto x) { return std::all_of(x.begin(), x.end(), std::isdigit); })
                    .map([](auto x) { return std::atoi(x.c_str()); })
                    .collect();
```

## Benchmarks

We provide a small set of benchmarks located in the `benchmarks/` directory. You can build them using provided build script.

```bash
./tools/build.sh --compile-benchmarks
```

All of the benchmarks measure the performance in release mode and run similar scenario using C++20 `std::ranges` (if it is possible to recreate tested feature without writing a lot of code) to compare how fast, or how slow we are.

### Current measurements

All of the measurements were taken on MacBook M3 Pro 18GB.

#### Filter and map

Apply filter and map on 1'000'000 integers

|          **Benchmark**         | **Time [ns]** | **CPU [ns]** |
|:------------------------------:|:-------------:|:------------:|
| benchmarkRustyIterFilterAndMap |     559108    |    559065    |
|   benchmarkRustyIterFilterMap  |     556122    |    556120    |
| benchmarkRangesFilterTransform |     539868    |    539786    |

Apply filter and map on 10'000'000 integers

|          **Benchmark**         | **Time [ns]** | **CPU [ns]** |
|:------------------------------:|:-------------:|:------------:|
| benchmarkRustyIterFilterAndMap |    5644164    |    5643854   |
|   benchmarkRustyIterFilterMap  |    5599177    |    5599185   |
| benchmarkRangesFilterTransform |    5988982    |    5988701   |

## Authors

- [Wiktor Nowak](@uncommon-nickname)
