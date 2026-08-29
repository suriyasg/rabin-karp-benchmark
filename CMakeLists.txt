cmake_minimum_required(VERSION 3.10)
project(RabinKarpBenchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include)

# Compile the core algorithms into a library
add_library(rabin_karp_lib src/RabinKarp.cpp)

# Build the dataset preparation tool
add_executable(prepare_dataset tools/prepare_dataset.cpp)
target_link_libraries(prepare_dataset rabin_karp_lib)

# Build the malicious dataset preparation tool
add_executable(prepare_malicious_dataset tools/prepare_malicious_dataset.cpp)
target_link_libraries(prepare_malicious_dataset rabin_karp_lib)

# Build the benchmark suite
add_executable(benchmark_deterministic src/benchmark_deterministic.cpp)
add_executable(benchmark_random_base src/benchmark_random_base.cpp)
add_executable(benchmark_random_modulus src/benchmark_random_modulus.cpp)
target_link_libraries(benchmark_deterministic rabin_karp_lib)
target_link_libraries(benchmark_random_base rabin_karp_lib)
target_link_libraries(benchmark_random_modulus rabin_karp_lib)