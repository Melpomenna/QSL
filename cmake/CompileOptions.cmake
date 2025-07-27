function(__SetCompileOptionsMSVC target)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Debug")
            target_compile_options(${target} 
                                    PRIVATE 
                                    /nologo
                                    /W4
                                    /WX 
                                    /Z7
                                    /RTC1
                                    /sdl
                                    /fsanitize=address 
                                    /analyze
                                    /external:W0
                                    /analyze:external-
                                    /Od
                                    /MP
                                )
        elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Release")
            target_compile_options(${target} 
                                    PRIVATE
                                    /nologo
                                    /W4
                                    /WX
                                    /sdl
                                    /O2 
                                    /analyze
                                    /external:W0
                                    /analyze:external-
                                    /MP
                                )
        endif()
endfunction()


function(__SetCompileOptionsMacOS target)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Debug")
            target_compile_options(${target} 
                                    PRIVATE
                                    -Wall
                                    -Werror
                                    -Wextra
                                    -g
                                    -O0
                                    -fno-rtti
                                    -fstack-protector-all
                                )
        elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Release")
            target_compile_options(${target} 
                                    PRIVATE
                                    -Wall
                                    -Werror
                                    -Wextra
                                    -O2
                                    -fno-rtti
                                    -fstack-protector-all
                                )
        endif()
endfunction()


function(__SetCompileOptionsLinux target)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Debug")
            target_compile_options(${target} 
                                    PRIVATE
                                    -W4
                                    -Werror
                                    -Wextra
                                    -g
                                    -O0
                                    -fsanitize=adress,thread
                                    -fno-rtti
                                    -fstack-protector-all
                                )
        elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
            message(STATUS "Use compile arguments for module name: ${target}, preset:Release")
            target_compile_options(${target} 
                                    PRIVATE
                                    -Wall
                                    -Werror
                                    -Wextra
                                    -O2
                                    -fno-rtti
                                    -fstack-protector-all
                                )
        endif()
endfunction()

function(SetCompilerOptions target)
    message(STATUS "Use compiler options")

    if(MSVC)
        __SetCompileOptionsMSVC(${target})
    elseif(APPLE)
        __SetCompileOptionsMacOS(${target})
    elseif(LINUX)
        __SetCompileOptionsLinux(${target})
    endif()


    get_property(${target}_COMPILE_OPTIONS TARGET ${target} PROPERTY COMPILE_OPTIONS)

    set(TARGET_COMPILE_OPTIONS ${${target}_COMPILE_OPTIONS})

    message(STATUS "Compiler options for ${target} ${TARGET_COMPILE_OPTIONS}")
    set_target_properties(
            ${target}
            PROPERTIES
            CXX_STANDARD ${CMAKE_CXX_STANDARD}
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS ON
            COMPILE_OPTIONS "${TARGET_COMPILE_OPTIONS}"
    )
endfunction()