if (${QSL_ENABLE_TESTS})
    add_subdirectory(gtest)
endif()

if (${QSL_ENABLE_BENCHMARKS})
    add_subdirectory(benchmarks)
endif()