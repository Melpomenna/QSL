function(UseClangTidy target)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

    if (NOT CLANG_TIDY_EXE)
        message(WARNING "no command clang-tidy")
    else ()
        message(STATUS "Set clang-tidy for ${target}")
        if (${CMAKE_GENERATOR} MATCHES "Visual Studio")
        set_target_properties(${target} PROPERTIES
            VS_GLOBAL_RunCodeAnalysis true
            VS_GLOBAL_EnableMicrosoftCodeAnalysis true
            VS_GLOBAL_EnableClangTidyCodeAnalysis true
            VS_GLOBAL_ClangTidyChecks "-* \"\"--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy --header-filter=gtest" 
        )
        else()
            set_target_properties(${target} PROPERTIES CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
        endif()
    endif ()
endfunction()