#include <gtest/gtest.h>
#include <QSL/Memory/NullAllocator.h>
#include <QSL/Memory/StdAllocator.h>

#if COMPILER_MSVC
#pragma warning(disable:6326)
#endif

class NullAllocatorTest : public ::testing::Test
{
protected:
    qsl::memory::NullAllocator m_allocator;
};

class StdAllocatorTest : public ::testing::Test
{
protected:
    qsl::memory::StdAllocator m_allocator;
};

TEST_F(NullAllocatorTest, AllocateReturnsNull)
{
    void* ptr = m_allocator.Allocate(100);
    EXPECT_EQ(ptr, nullptr);
}

TEST_F(StdAllocatorTest, AllocationSizeofInBy10ObjectsRetursnSuccess)
{
    int* ptr = static_cast<int*>(m_allocator.Allocate(sizeof(int) * 10));
    EXPECT_NE(ptr, nullptr);
    for (int i = 0; i < 10; ++i)
    {
        (void)m_allocator.Construct<int>(ptr+i, 10);
        EXPECT_EQ(*(ptr+i), 10);
    }

    m_allocator.Deallocate(ptr,sizeof(int)*10);
}
