add_executable(aoc main.cpp)
target_compile_features(aoc PRIVATE cxx_std_23)
target_link_libraries(aoc PRIVATE rusty_iterators)
