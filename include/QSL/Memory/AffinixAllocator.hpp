#pragma once

#include <QSL/Config/Config.h>

namespace qsl::memory
{
    template <class Allocator, class Prefix, class Suffix = void>
    class AffixAllocator
    {
    public:
        using TSizeType = typename Allocator::TSizeType;
        using ValueType = typename Allocator::ValueType;
        using BufferType = typename Allocator::BufferType;

        template <class QAllocator, class QPrefix, class QSuffix = void>
        using RebindAllocator = AffixAllocator<QAllocator, QPrefix, QSuffix>;

        using PropogateOnCopy = typename Allocator::PropogateOnCopy;
        using PropogateOnMove = typename Allocator::PropogateOnMove;
        using PropogateOnSwap = typename Allocator::PropogateOnSwap;


        AffixAllocator() = delete;

        AffixAllocator(const Allocator& alloc) : m_pAllocator(std::addressof(alloc))
        {
        }

        AffixAllocator(const AffixAllocator& other) = default;

        AffixAllocator& operator=(const AffixAllocator& other) = default;

        AfiixAllocator(AffixAllocator&& other) noexcept = default;

        AffixAlllocator& operator=(AffixAllocator&& other) noexcept = default;

        virtual ~AffixAllocator() = default;

        QSL_NODISCARD QSL_ALLOCATOR BufferType Allocate(TSizeType nBytes, Prefix* pPrefix = nullptr,
                                                        Suffix* pSuffix = nullptr)
        {
            TSizeType nOldBytes = nBytes;
            nBytes += (pPrefix != nullptr) * sizeof(Prefix);
            nBytes += (pSuffix != nullptr) * sizeof(Suffix);
            BufferType pMem = m_pAllocator->Allocate(nBytes);
            if (!pMem)
                return nullptr;
            if (pPrefix)
                memcpy(pMem, pPrefix, sizeof(Prefix));
            if (pSuffix)
                memcpy(static_cast<char*>(pMem) + sizeof(Prefix) + nOldBytes, pSuffix, sizeof(Suffix));
            return static_cast<char*>(pMem) + sizeof(Prefix);
        }

        QSL_NODISCARD void Deallocate(BufferType pMem, qsl::config::TSizeType nBytes, Prefix** pPrefix = nullptr,
                                      Suffix** pSuffix = nullptr)
        {
            if (!pMem)
                return;
            if (pPrefix)
                *pPrefix = reinterpret_cast<Prefix*>(static_cast<char*>(pMem) - sizeof(Prefix));
            if (pSuffix)
                *pSuffix = reinterpret_cast<Suffix*>(static_cast<char*>(pMem) + nBytes);
            m_pAllocator->Deallocate(pMem);
        }

        template <class... Args>
        QSL_NODISCARD QSL_CONSTEXPR BufferType Construct(BufferType pMem, Args&&... args)
        {
            return m_pAllocator->Construct(pMem, std::forward<Args>(args)...);
        }

        QSL_CONSTEXPR void Destroy(BufferType pObject) noexcept
        {
            m_pAllocator->Destroy(pObject);
        }

    private:
        Allocator* m_pAllocator{nullptr};
    };
} // namespace qsl::memory