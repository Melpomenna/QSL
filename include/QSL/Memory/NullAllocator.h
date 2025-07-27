#pragma once

#include <QSL/Config/Config.h>

namespace qsl::memory
{
    class QSL_API NullAllocator
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using ValueType = char;
        using BufferType = void*;

        using RebindAllocator = NullAllocator;

        using PropogateOnCopy = qsl::config::TrueType;
        using PropogateOnMove = qsl::config::TrueType;
        using PropogateOnSwap = qsl::config::TrueType;

        NullAllocator() = default;

        QSL_CONSTEXPR NullAllocator(const NullAllocator&) noexcept = default;
        QSL_CONSTEXPR NullAllocator& operator=(const NullAllocator&) noexcept = default;

        QSL_CONSTEXPR NullAllocator(NullAllocator&&) noexcept = default;
        QSL_CONSTEXPR NullAllocator& operator=(NullAllocator&&) noexcept = default;

        virtual ~NullAllocator() = default;

        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(TSizeType nBytes) noexcept;
        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(TSizeType nBytes, TSizeType nAlign) noexcept;
        QSL_CONSTEXPR void Deallocate(QSL_SAL_NULL void* pMem, TSizeType nBytes) noexcept;

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept;

        template <class T, class... Args>
        QSL_NODISCARD QSL_CONSTEXPR T* Construct(QSL_SAL_NULL [[maybe_unused]] void* pMem,
                                                 [[maybe_unused]] Args&&... args)
        {
            QSL_VERIFY(pMem == nullptr);
            return nullptr;
        }

        template <class T>
        QSL_CONSTEXPR void Destroy(QSL_SAL_NULL [[maybe_unused]] T* pObject) noexcept
        {
            QSL_VERIFY(pObject == nullptr);
        }

        QSL_CONSTEXPR void Clear() noexcept;

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(void* pMem) const noexcept;
    };
} // namespace qsl::memory