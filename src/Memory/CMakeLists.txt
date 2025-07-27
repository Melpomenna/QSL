set(target Memory)

set(SOURCES 
    NullAllocator.cpp
    StdAllocator.cpp
)

add_library(${target} 
    SHARED
    ${SOURCES}
)


include(CompileOptions)
SetCompilerOptions(${target})
