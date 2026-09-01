cmake_minimum_required(VERSION 3.14)
project(RabinKarpBenchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Always build in Release mode for accurate benchmarks
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Fetch Google Benchmark automatically
include(FetchContent)
FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
FetchContent_MakeAvailable(google_benchmark)

include_directories(include)

# Core Library
add_library(rabin_karp_lib src/RabinKarp.cpp)

# Dataset Tools
add_executable(prepare_dataset tools/prepare_dataset.cpp)
target_link_libraries(prepare_dataset rabin_karp_lib)

add_executable(prepare_malicious tools/prepare_malicious_dataset.cpp)
target_link_libraries(prepare_malicious rabin_karp_lib)

add_executable(generate_text tools/generate_text.cpp)
target_link_libraries(generate_text rabin_karp_lib)

# The New Google Benchmark Runner
add_executable(benchmark_runner src/benchmark.cpp)
target_link_libraries(benchmark_runner 
    rabin_karp_lib 
    benchmark::benchmark 
)