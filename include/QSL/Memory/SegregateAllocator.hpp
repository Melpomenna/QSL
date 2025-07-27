#pragma once

#include <QSL/Config/Config.h>

namespace qsl::memory
{

    template <qsl::config::TSizeType nThreadShold, class LowMemoryAllocator, class HighMemoryAllocator>
    class SegregateAllocator
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using BufferType = void*;
        using ValueType = char;

        template <qsl::config::TSizeType nThreadShold, class LowMemoryAllocator, class HighMemoryAllocator>
        using RebindAllocator = SegregateAllocator<nThreadShold, LowMemoryAllocator, HighMemoryAllocator>;

        using PropogateOnCopy =
            std::conditional_t<std::is_same_v<typename LowMemoryAllocator::PropogateOnCopy, qsl::config::TrueType> &&
                                   std::is_same_v<typename HighMemoryAllocator::PropogateOnCopy, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;
        using PropogateOnMove =
            std::conditional_t<std::is_same_v<typename LowMemoryAllocator::PropogateOnMove, qsl::config::TrueType> &&
                                   std::is_same_v<typename HighMemoryAllocator::PropogateOnMove, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;
        using PropogateOnSwap =
            std::conditional_t<std::is_same_v<typename LowMemoryAllocator::PropogateOnSwap, qsl::config::TrueType> &&
                                   std::is_same_v<typename HighMemoryAllocator::PropogateOnSwap, qsl::config::TrueType>,
                               qsl::config::TrueType, qsl::config::FalseType>;

        SegregateAllocator() = delete;

        explicit SegregateAllocator(const PrimaryAllocator& primaryAllocator,
                                    const SegregateAllocator& SegregateAllocator) :
            m_pPrimaryAllocator(std::addressof(primaryAllocator)),
            m_pSegregateAllocator(std::addressof(SegregateAllocator))
        {
        }

        SegregateAllocator(const SegregateAllocator&) = default;
        SegregateAllocator& operator=(const SegregateAllocator&) = default;

        SegregateAllocator(SegregateAllocator&&) = default;
        SegregateAllocator& operator=(SegregateAllocator&&) = default;

        virtual ~SegregateAllocator() = default;

        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(qsl::config::TSizeType nBytes) noexcept
        {
            if (nBytes < nThreadShold)
            {
                return m_pLowMemAllocator->Allocate(nBytes);
            }
            return m_pHighMemAllocator->Allocate(nBytes);
        }

        QSL_NODISCARD QSL_CONSTEXPR void* Allocate(qsl::config::TSizeType nBytes, qsl::config::TSizeType nAlign) noexcept
        {
            if (nBytes < nThreadShold)
            {
                return m_pLowMemAllocator->Allocate(nBytes, nAlign);
            }
            return m_pHighMemAllocator->Allocate(nBytes, nAlign);
        }

        QSL_CONSTEXPR void Deallocate(void* pMem, qsl::config::TSizeType nBytes) noexcept
        {
            if (m_pLowMemAllocator->Owns(pMem))
            {
                m_pLowMemAllocator->Deallocate(pMem, nBytes);
            }
            else if (m_pHighMemAllocator->Owns(pMem))
            {
                m_pHighMemAllocator->Deallocate(pMem, nBytes);
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
            if (m_pLowMemAllocator->Owns(pMem))
            {
                return m_pLowMemAllocator->Construct<T>(pMem, std::forward<Args>(args)...);
            }
            else if (m_pHighMemAllocator->Owns(pMem))
            {
                return m_pHighMemAllocator->Construct<T>(pMem, std::forward<Args>(args)...);
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
            if (m_pLowMemAllocator->Owns(pObject))
            {
                m_pLowMemAllocator->Destroy(pObject);
            }
            else if (m_pHighMemAllocator->Owns(pObject))
            {
                m_pHighMemAllocator->Destroy(pObject);
            }
            else
            {
                QSL_VERIFY(false);
            }
        }

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(void* pMem) const noexcept
        {
            return m_pLowMemAllocator->Owns(pMem) || m_pHighMemAllocator->Owns(pMem);
        }

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept
        {
            return m_pLowMemAllocator->GetSize() + m_pHighMemAllocator->GetSize();
        }

    private:
        LowMemoryAllocator* m_pLowMemAllocator{nullptr};
        HighMemoryAllocator* m_pHighMemAllocator{nullptr};
    };

} // namespace qsl::memory