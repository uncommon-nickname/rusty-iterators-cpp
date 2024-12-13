include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB TESTS "*.cpp")
add_executable(rusty_iterators_tests ${TESTS})
target_link_libraries(rusty_iterators_tests rusty_iterators gtest_main gmock_main)

gtest_discover_tests(rusty_iterators_tests)
