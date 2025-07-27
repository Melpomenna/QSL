#pragma once

#include <QSL/Config/Config.h>

namespace qsl::memory
{
    class QSL_API StdAllocator
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using ValueType = char;

        using RebindAllocator = StdAllocator;

        using PropogateOnCopy = qsl::config::TrueType;
        using PropogateOnMove = qsl::config::TrueType;
        using PropogateOnSwap = qsl::config::TrueType;

        static void Invoke();

        StdAllocator() = default;

        QSL_CONSTEXPR StdAllocator(const StdAllocator&) noexcept = default;
        QSL_CONSTEXPR StdAllocator& operator=(const StdAllocator&) noexcept = default;

        QSL_CONSTEXPR StdAllocator(StdAllocator&&) noexcept = default;
        QSL_CONSTEXPR StdAllocator& operator=(StdAllocator&&) noexcept = default;

        virtual ~StdAllocator() = default;

        QSL_NODISCARD QSL_ALLOCATOR void* Allocate(TSizeType nBytes) noexcept;

        QSL_NODISCARD QSL_ALLOCATOR void* Allocate(TSizeType nBytes, TSizeType nAlign) noexcept;

        void Deallocate(void* pMem, TSizeType nBytes) noexcept;

        template <class T, class... Args>
        QSL_NODISCARD QSL_CONSTEXPR T* Construct(void* pMem,
                                                 Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            QSL_VERIFY(pMem != nullptr);
            return new (pMem) T(std::forward<Args>(args)...);
        }

        template <class T>
        QSL_CONSTEXPR void Destroy(T* pObject) noexcept
        {
            QSL_VERIFY(pObject != nullptr);
            pObject->~T();
        }

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(void* pMem) const noexcept;

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept;

        QSL_CONSTEXPR void Clear() noexcept;
    };
} // namespace qsl::memory