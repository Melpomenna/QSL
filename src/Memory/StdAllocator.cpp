#include <QSL/Memory/StdAllocator.h>
#include <cstdlib>

namespace qsl::memory
{
    static void foo();

    QSL_NODISCARD void* StdAllocator::Allocate(TSizeType nBytes) noexcept
    {
        return Allocate(nBytes, alignof(ValueType));
    }

    QSL_NODISCARD void* StdAllocator::Allocate(TSizeType nBytes, [[maybe_unused]] TSizeType nAlign) noexcept
    {
        return malloc(nBytes);
    }

    void StdAllocator::Deallocate(void* pMem, [[maybe_unused]] TSizeType nBytes) noexcept
    {
        free(pMem);
    }

    QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType StdAllocator::GetSize() const noexcept
    {
        return 0;
    }

    QSL_NODISCARD QSL_CONSTEXPR bool StdAllocator::Owns([[maybe_unused]] void* pMem) const noexcept
    {
        return true;
    }

    QSL_CONSTEXPR void StdAllocator::Clear() noexcept
    {
    }
} // namespace qsl::memory