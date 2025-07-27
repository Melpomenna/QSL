#pragma once

#include <QSL/Config/Config.h>

namespace qsl::memory
{
    template <class T = qsl::config::TByte, qsl::config::TSizeType Size>
    class StackAllocator
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using BufferType = T[Size];
        using ValueType = T;

        template <class Q, qsl::config::TSizeType N>
        using RebindAllocator = StackAllocator<Q, N>;

        using PropogateOnCopy = qsl::config::TrueType;
        using PropogateOnMove = qsl::config::TrueType;
        using PropogateOnSwap = qsl::config::TrueType;

        StackAllocator() = default;

        StackAllocator(const StackAllocator& other) noexcept(std::is_nothrow_copy_constructible_v<T>) :
            m_nOffset(other.m_nOffset)
        {
            for (qsl::config::TSizeType i = 0; i < Size; ++i)
            {
                m_arrBuffer[i] = other.m_arrBuffer[i];
            }
        }

        StackAllocator& operator=(const StackAllocator& other) noexcept(std::is_nothrow_copy_assignable_v<T>)
        {
            if (this == &other)
                return *this;

            m_nOffset = other.m_nOffset;

            for (qsl::config::TSizeType i = 0; i < Size; ++i)
            {
                m_arrBuffer[i] = other.m_arrBuffer[i];
            }

            return *this;
        }

        StackAllocator(StackAllocator&& other) noexcept(std::is_nothrow_move_constructible_v<T>) :
            m_nOffset(other.m_nOffset)
        {
            for (qsl::config::TSizeType i = 0; i < Size; ++i)
            {
                m_arrBuffer[i] = std::move(other.m_arrBuffer[i]);
            }
            other.m_nOffset = 0;
        }

        StackAllocator& operator=(StackAllocator&& other) noexcept(std::is_nothrow_move_assignable_v<T>)
        {
            if (this == &other)
                return *this;

            m_nOffset = other.m_nOffset;
            for (qsl::config::TSizeType i = 0; i < Size; ++i)
            {
                m_arrBuffer[i] = std::move(other.m_arrBuffer[i]);
            }
            other.m_nOffset = 0;
            return *this;
        }

        virtual ~StackAllocator() = default;

        QSL_NODISCARD QSL_CONSTEXPR BufferType Allocate(TSizeType nCount) noexcept
        {
            return Allocate(nCount, 0);
        }

        QSL_NODISCARD QSL_CONSTEXPR BufferType Allocate(TSizeType nCount, TSizeType nAlign) noexcept
        {
            nCount = GetAlignedSize(nCount, nAlign);
            if (m_nOffset + nCount > Size)
            {
                QSL_VERIFY(false);
                return nullptr;
            }

            void* pMem = &m_arrBuffer[m_nOffset];
            m_nOffset += nCount;
            return pMem;
        }

        QSL_CONSTEXPR void Deallocate([[maybe_unused]] BufferType pMem, [[maybe_unused]] TSizeType nCount) noexcept
        {
            if (!Owns(pMem))
            {
                QSL_VERIFY(false);
                return;
            }

            m_nOffset = 0;
        }

        template <class... Args>
        QSL_NODISCARD QSL_CONSTEXPR BufferType
        Construct(BufferType pMem, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            if (!Owns(pMem))
            {
                QSL_VERIFY(false);
                return nullptr;
            }
            pMem->ValueType(std::forward<Args>(args)...);
            return pObject;
        }

        QSL_CONSTEXPR void Destroy(BufferType pObject) noexcept
        {
            if (!Owns(pObject))
            {
                QSL_VERIFY(false);
                return;
            }

            pObject->~ValueType();
        }

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(void* pMem) const noexcept
        {
            return &m_arrBuffer[0] <= pMem && pMem < &m_arrBuffer[Size - 1];
        }

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept
        {
            return Size - m_nOffset;
        }

        QSL_CONSTEXPR void Clear() noexcept
        {
            m_nOffset = 0;
        }

    private:
        QSL_CONSTEXPR TSizeType GetAlignedSize(TSizeType nBytes, TSizeType nAlign) const noexcept
        {
            return (nBytes + nAlign - 1) & ~(nAlign - 1);
        }

        ValueType m_arrBuffer[Size]{};
        TSizeType m_nOffset{0};
    };


    template <class T>
    class StackAllocator<T, 0>
    {
    public:
        using TSizeType = qsl::config::TSizeType;
        using BufferType = T*;
        using ValueType = T;

        template <class Q, qsl::config::TSizeType N>
        using RebindAllocator = StackAllocator<Q, N>;

        using PropogateOnCopy = qsl::config::TrueType;
        using PropogateOnMove = qsl::config::TrueType;
        using PropogateOnSwap = qsl::config::TrueType;

        QSL_CONSTEXPR StackAllocator() = default;

        QSL_CONSTEXPR StackAllocator(const StackAllocator& other) noexcept = default;

        QSL_NODISCARD QSL_CONSTEXPR StackAllocator& operator=(const StackAllocator& other) noexcept = default;

        QSL_CONSTEXPR StackAllocator(StackAllocator&& other) noexcept = default;

        QSL_NODISCARD QSL_CONSTEXPR StackAllocator& operator=(StackAllocator&& other) noexcept = default;

        virtual ~StackAllocator() = default;

        QSL_NODISCARD QSL_CONSTEXPR BufferType Allocate([[maybe_unused]] TSizeType nBytes) noexcept
        {
            return nullptr;
        }

        QSL_CONSTEXPR void Deallocate(BufferType pMem, [[maybe_unused]] TSizeType nBytes) noexcept
        {
            QSL_VERIFY(pMem == nullptr);
        }

        template <class... Args>
        QSL_NODISCARD QSL_CONSTEXPR BufferType Construct([[maybe_unused]] BufferType pMem,
                                                         [[maybe_unused]] Args&&... args)
        {
        }

        template <class T>
        QSL_CONSTEXPR void Destroy([[maybe_unused]] BufferType pObject) noexcept
        {
        }

        QSL_NODISCARD QSL_CONSTEXPR bool Owns(BufferType pMem) const noexcept
        {
            return pMem == nullptr;
        }

        QSL_NODISCARD QSL_CONSTEXPR qsl::config::TSizeType GetSize() const noexcept
        {
            return 0;
        }

        QSL_CONSTEXPR void Clear() noexcept
        {
        }
    };
} // namespace qsl::memory