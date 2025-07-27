#include <QSL/Memory/NullAllocator.h>

namespace qsl::memory
{

    QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType NullAllocator::GetSize() const noexcept
    {
        return 0;
    }

    QSL_NODISCARD QSL_CONSTEXPR void* NullAllocator::Allocate([[maybe_unused]] TSizeType nBytes) noexcept
    {
        return nullptr;
    }

    QSL_NODISCARD QSL_CONSTEXPR void* NullAllocator::Allocate([[maybe_unused]] TSizeType nBytes,
                                                              [[maybe_unused]] TSizeType nAlign) noexcept
    {
        return nullptr;
    }

    QSL_CONSTEXPR void NullAllocator::Deallocate(QSL_SAL_NULL [[maybe_unused]] void* pMem,
                                                 [[maybe_unused]] TSizeType nBytes) noexcept
    {
        QSL_VERIFY(pMem == nullptr);
    }

    QSL_NODISCARD QSL_CONSTEXPR bool NullAllocator::Owns(void* pMem) const noexcept
    {
        QSL_VERIFY(pMem == nullptr);
        return pMem == nullptr;
    }

    QSL_CONSTEXPR void NullAllocator::Clear() noexcept
    {
    }
} // namespace qsl::memory