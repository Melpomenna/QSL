#include <benchmark/benchmark.h>
#include <QSL/Memory/NullAllocator.h>
#include <memory_resource>

static void Benchmark_StdNullAllocator(benchmark::State& state)
{
    for([[maybe_unused]] const auto& _ : state)
    {
        auto* pResource = std::pmr::null_memory_resource();

        for(int i = 0; i < 100; ++i) {
            benchmark::DoNotOptimize(pResource->allocate(0));
        }

        benchmark::DoNotOptimize(pResource);
    }
}

static void Benchmark_QSLNullAllocator(benchmark::State& state)
{
    for([[maybe_unused]] const auto& _ : state)
    {
        qsl::memory::NullAllocator allocator;

        for(int i = 0; i < 100; ++i) {
            benchmark::DoNotOptimize(allocator.Allocate(100));
        }

        benchmark::DoNotOptimize(allocator);
    }
}

BENCHMARK(Benchmark_StdNullAllocator);
BENCHMARK(Benchmark_QSLNullAllocator);