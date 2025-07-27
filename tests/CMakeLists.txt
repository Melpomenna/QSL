set(target Tests)

remove_definitions(-DQSL_API_EXPORT)

add_executable(${target} 
    memory.tests.cpp
)

target_link_libraries(${target} 
    PRIVATE
        Memory
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(CompileOptions)
SetCompilerOptions(${target})
include(GoogleTest)
gtest_discover_tests(${target})