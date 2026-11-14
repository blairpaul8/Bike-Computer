cmake_minimum_required(VERSION 3.22)
project(bike_logic_host_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

# Compile the same sources the firmware uses, but with the host compiler
set(LOGIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../components/bike_logic)
add_library(bike_logic STATIC ${LOGIC_DIR}/src/metrics.cpp)
target_include_directories(bike_logic PUBLIC ${LOGIC_DIR}/include)

enable_testing()
add_executable(unit_tests metrics_test.cpp)
target_link_libraries(unit_tests PRIVATE bike_logic GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)