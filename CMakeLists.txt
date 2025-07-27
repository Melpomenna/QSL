cmake_minimum_required(VERSION 3.19...4.0)

enable_testing()

project(QSL VERSION 1.0.0 LANGUAGES CXX)

set(BENCHMARK_ENABLE_GTEST_TESTS  OFF)
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_INSTALL_DOCS OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(INSTALL_GTEST OFF)


if (MSVC)
    set(BENCHMARK_ENABLE_LTO ON)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}:${CMAKE_MODULE_PATH}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/WorkFolder)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/WorkFolder)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/WorkFolder)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/WorkFolder)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(QSL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

include(GlobalCompileOptions)
SetGlobalCompileOptions()

add_subdirectory(external)

add_definitions(-DQSL_API_EXPORT)

add_subdirectory(src)

if (${QSL_ENABLE_TESTS})
    message(STATUS "Tests enabled")
    add_subdirectory(tests)
endif()

if (${QSL_ENABLE_BENCHMARKS})
    message(STATUS "Benchmarks enabled")
    add_subdirectory(benchmarks)
endif()

add_custom_target(${CMAKE_PROJECT_NAME} ALL)

add_custom_target(
    run_benchmarks
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/WorkFolder/Benchmarks
    COMMENT "Executing benchmark script"
    OUTPUT Unused
)

file(GLOB_RECURSE QSL_HEADERS CONFIGURE_DEPENDS ${QSL_INCLUDE_DIR}/QSL/*.h ${QSL_INCLUDE_DIR}/QSL/*.hpp)
file(GLOB_RECURSE QSL_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

include(CppCheck)
set(CPPCHECK_DIR ${QSL_INCLUDE_DIR}/QSL/Config)
UseCppCheck(${CMAKE_PROJECT_NAME} QSL_SOURCES QSL_HEADERS)
include(ClangTidy)
UseClangTidy(${CMAKE_PROJECT_NAME})

include(QSLInstall)
QSLInstallProject()