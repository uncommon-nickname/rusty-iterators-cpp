cmake_minimum_required(VERSION 3.15...3.30)

project(
    rusty-iterators
    VERSION 0.0.1
    DESCRIPTION "Light, lazy and fast iterators, inspired by Rust."
    LANGUAGES CXX
)

option(COMPILE_TESTS "Boolean indicating if tests should be compiled")
option(COMPILE_BENCHMARKS "Boolean indicating if benchmarks should be compiled")
option(COMPILE_EXAMPLES "Boolean indicating if examples should be compiled")

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${rusty-iterators_SOURCE_DIR}/.clang-tidy")

add_library(rusty_iterators INTERFACE)
target_include_directories(rusty_iterators INTERFACE "${rusty-iterators_SOURCE_DIR}/include/")
target_compile_features(rusty_iterators INTERFACE cxx_std_23)
set_target_properties(rusty_iterators PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
set_target_properties(rusty_iterators PROPERTIES LINKER_LANGUAGE CXX)

if(${COMPILE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()

if(${COMPILE_BENCHMARKS})
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    set(CMAKE_BUILD_TYPE "Release")
    add_subdirectory(benchmarks)
endif()

if (${COMPILE_EXAMPLES})
    add_subdirectory(examples)
endif()
