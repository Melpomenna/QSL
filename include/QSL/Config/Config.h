#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define OS_WIN 1
#define OS_LINUX 0
#define OS_MAC 0
#elif defined(__linux__)
#define OS_WIN 0
#define OS_LINUX 1
#define OS_MAC 0
#elif defined(__APPLE__)
#define OS_WIN 0
#define OS_LINUX 0
#define OS_MAC 1
#else
#error "Unsupported operating system"
#endif

#if defined(__GNUC__)
#define COMPILER_GCC 1
#define COMPILER_CLANG 0
#define COMPILER_MSVC 0
#elif defined(__clang__)
#define COMPILER_GCC 0
#define COMPILER_CLANG 1
#define COMPILER_MSVC 0
#elif defined(_MSC_VER)
#define COMPILER_GCC 0
#define COMPILER_CLANG 0
#define COMPILER_MSVC 1
#else
#error "Unsupported compiler"
#endif

#if defined(QSL_API_EXPORT)
#if OS_WIN
#define QSL_API __declspec(dllexport)
#elif OS_LINUX || OS_MAC
#define QSL_API __attribute__((__visibility__("default")))
#endif
#else
#if OS_WIN
#define QSL_API __declspec(dllimport)
#elif OS_LINUX || OS_MAC
#define QSL_API
#endif
#endif


#if OS_WIN && COMPILER_MSVC
#define QSL_ALLOCATOR __declspec(allocator)
#else
#define QSL_ALLOCATOR
#endif

#if OS_WIN && COMPILER_MSVC
#define QSL_NO_VTABLE __declspec(novtable)
#else
#define QSL_NO_VTABLE
#endif

#define QSL_CXX __cplusplus
#define QSL_CXX_VERS ((QSL_CXX / 100) % 100)

#if QSL_CXX_VERS == 26
#define QSL_CXX_26 1
#else
#define QSL_CXX_26 0
#endif

#if QSL_CXX_VERS >= 23
#define QSL_CXX_23 1
#else
#define QSL_CXX_23 0
#endif

#if QSL_CXX_VERS >= 20
#define QSL_CXX_20 1
#else
#define QSL_CXX_20 0
#endif

#if QSL_CXX_VERS >= 17
#define QSL_CXX_17 1
#else
#define QSL_CXX_17 0
#endif

#if QSL_CXX_VERS >= 14
#define QSL_CXX_14 1
#else
#define QSL_CXX_14 0
#error "Minimum required cxx standart is 14"
#endif


#if QSL_CXX_23
#if defined(__cpp_explicit_this_parameter)
#define QSL_DEDUCING_THIS 1
#else
#define QSL_DEDUCING_THIS 0
#endif
#else
#define QSL_DEDUCING_THIS 0
#endif

#define QSL_NOEXCEPT noexcept
#define QSL_CONSTEXPR constexpr

#if QSL_CXX_20
#define QSL_CONSTINIT constinit
#define QSL_CONSTEVAL consteval
#define QSL_LIKELY [[likely]]
#define QSL_UNLIKELY [[unlikely]]
#else
#define QSL_CONSTINIT
#define QSL_CONSTEVAL
#define QSL_LIKELY
#define QSL_UNLIKELY
#endif

#if QSL_CXX_17
#define QSL_NODISCARD [[nodiscard]]
#else
#define QSL_NODISCARD
#endif

#if QSL_CXX_26
#include <debugging>
#define QSL_DEBUG_BREAK() std::breakpoint();
#else
#if OS_WIN && COMPILER_MSVC
#define QSL_SAL_NULL __null
#define QSL_DEBUG_BREAK() __debugbreak()
#else
#define QSL_SAL_NULL
#define QSL_DEBUG_BREAK() __builtin_debugbreak()
#define QSL_ANALYZE_ASSUM_CONDITION(...)
#endif
#endif

#if !defined(NDEBUG)
#define QSL_VERIFY(condition) ((void)((!!(condition)) || (QSL_DEBUG_BREAK(), 0)))

#define QSL_ASSERT(condition)                                                                                          \
    while (!!(condition))                                                                                              \
        QSL_DEBUG_BREAK();
#else
#define QSL_VERIFY(...)
#define QSL_ASSERT(...)
#endif

#include <type_traits>
#include <limits>

namespace qsl
{
    namespace config
    {

        using TSizeType = unsigned long long;
        using TByte = char;

        using TrueType = std::true_type;
        using FalseType = std::false_type;

        QSL_CONSTEXPR inline TSizeType kBitsCount = CHAR_BIT;
    } // namespace config
} // namespace qsl