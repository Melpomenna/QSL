#pragma once

#include <QSL/Config/config.h>

namespace qsl::memory
{
    template <class PrimaryAllocator, class FallbackAllocator>
    class FallBackAllocator
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using BufferType = void*;
        using ValueType = char;

        template <class PrimaryAllocator, class FallbackAllocator>
        using RebindAllocator = FallBackAllocator<PrimaryAllocator, FallbackAllocator>;

        using PropogateOnCopy =
            std::conditional_t<std::is_same_v<typename PrimaryAllocator::PropogateOnCopy, qsl::config::TrueType> &&
                                   std::is_same_v<typename FallbackAllocator::PropogateOnCopy, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;
        using PropogateOnMove =
            std::conditional_t<std::is_same_v<typename PrimaryAllocator::PropogateOnMove, qsl::config::TrueType> &&
                                   std::is_same_v<typename FallbackAllocator::PropogateOnMove, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;
        using PropogateOnSwap =
            std::conditional_t<std::is_same_v<typename PrimaryAllocator::PropogateOnSwap, qsl::config::TrueType> &&
                                   std::is_same_v<typename FallbackAllocator::PropogateOnSwap, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;

        FallBackAllocator() = delete;

        explicit FallBackAllocator(const PrimaryAllocator& primaryAllocator,
                                   const FallbackAllocator& fallbackAllocator) :
            m_pPrimaryAllocator(std::addressof(primaryAllocator)),
            m_pFallbackAllocator(std::addressof(fallbackAllocator))
        {
        }

        FallBackAllocator(const FallBackAllocator&) = default;
        FallBackAllocator& operator=(const FallBackAllocator&) = default;

        FallBackAllocator(FallBackAllocator&&) = default;
        FallBackAllocator& operator=(FallBackAllocator&&) = default;

        virtual ~FallBackAllocator() = default;

        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(qsl::config::TSizeType nBytes) noexcept
        {
            if (void* pMem = m_pPrimaryAllocator->Allocate(nBytes))
            {
                return pMem;
            }
            return m_pFallbackAllocator->Allocate(nBytes);
        }

        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(qsl::config::TSizeType nBytes, qsl::config::TSizeType nAlign) noexcept
        {
            if (void* pMem = m_pPrimaryAllocator->Allocate(nBytes, nAlign))
            {
                return pMem;
            }
            return m_pFallbackAllocator->Allocate(nBytes, nAlign);
        }

        QSL_CONSTEXPR void Deallocate(void* pMem, qsl::config::TSizeType nBytes) noexcept
        {
            if (m_pPrimaryAllocator->Owns(pMem))
            {
                m_pPrimaryAllocator->Deallocate(pMem, nBytes);
            }
            else if (m_pFallbackAllocator->Owns(pMem))
            {
                m_pFallbackAllocator->Deallocate(pMem, nBytes);
            }
            else
            {
                QSL_VERIFY(false);
            }
        }

        template <class T, class... Args>
        QSL_NODISCARD QSL_CONSTEXPR T* Construct(void* pMem,
                                                 Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            if (m_pPrimaryAllocator->Owns(pMem))
            {
                return m_pPrimaryAllocator->Construct<T>(pMem, std::forward<Args>(args)...);
            }
            else if (m_pFallbackAllocator->Owns(pMem))
            {
                return m_pFallbackAllocator->Construct<T>(pMem, std::forward<Args>(args)...);
            }
            else
            {
                QSL_VERIFY(false);
                return nullptr;
            }
        }

        template <class T>
        QSL_CONSTEXPR void Destroy(T* pObject) noexcept
        {
            if (m_pPrimaryAllocator->Owns(pObject))
            {
                m_pPrimaryAllocator->Destroy(pObject);
            }
            else if (m_pFallbackAllocator->Owns(pObject))
            {
                m_pFallbackAllocator->Destroy(pObject);
            }
            else
            {
                QSL_VERIFY(false);
            }
        }

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(void* pMem) const noexcept
        {
            return m_pPrimaryAllocator->Owns(pMem) || m_pFallbackAllocator->Owns(pMem);
        }

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept
        {
            return m_pPrimaryAllocator->GetSize() + m_pFallbackAllocator->GetSize();
        }

    private:
        PrimaryAllocator* m_pPrimaryAllocator{nullptr};
        FallbackAllocator* m_pFallbackAllocator{nullptr};
    };
} // namespace qsl::memory