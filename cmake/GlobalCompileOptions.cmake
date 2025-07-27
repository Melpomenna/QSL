function(__SetGlobalCompileOptionsMSVC)
    add_compile_options(/EHs)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_link_options(/INCREMENTAL:NO)
        add_compile_options(/MTd
                            /fsanitize=address 
                            /std:c++${CMAKE_CXX_STANDARD}
                            /Zc:__cplusplus
                            )
        add_definitions(-D_DEBUG)
    else()
        add_link_options(/LTCG:INCREMENTAL)
        add_compile_options(/MT 
                            /GL
                            /std:c++${CMAKE_CXX_STANDARD}
                            /Zc:__cplusplus
                            )
    endif()
endfunction()

function(__SetGlobalCompileOptionsMacOS)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(-g
                            -std=c++${CMAKE_CXX_STANDART}
                            )
    else()
        add_compile_options(-march=native 
                            -flto
                            -std=c++${CMAKE_CXX_STANDART}
                            )
    endif()
endfunction()

function(__SetGlobalCompileOptionsLinux)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(-g                            
                            -std=c++${CMAKE_CXX_STANDART}
                            )
    else()
        add_compile_options(-march=native 
                            -flto                            
                            -std=c++${CMAKE_CXX_STANDART}
                            )
    endif()
endfunction()

function(SetGlobalCompileOptions)
    if (MSVC)
        __SetGlobalCompileOptionsMSVC()
    elseif(APPLE)
        __SetGlobalCompileOptionsMacOS()
    elseif(LINUX)
        __SetGlobalCompileOptionsLinux()
    endif()
endfunction()