set(target Benchmarks)

add_executable(${target} memory.benchmarks.cpp)

target_link_libraries(${target} PRIVATE Memory benchmark::benchmark benchmark::benchmark_main)

include(CompileOptions)
SetCompilerOptions(${target})