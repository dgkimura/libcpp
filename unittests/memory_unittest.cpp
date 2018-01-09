#include <gtest/gtest.h>

#include <memory.hpp>


TEST(MemoryTest, testSharedPtrGetAllocatedObject)
{
    libcpp::shared_ptr<int> p(new int(42));
    ASSERT_EQ(*p.get(), 42);
}

TEST(MemoryTest, testSharedPtrInitialStrongCount)
{
    libcpp::shared_ptr<int> p(nullptr);
    ASSERT_EQ(p.use_count(), 1);
}

TEST(MemoryTest, testSharedPtrDeletesAllocatedMemoryAfterStrongCountDropsToZero)
{
    struct Object
    {
        Object(bool& is_destroyed) : is_destroyed(is_destroyed) {}
        ~Object(){ is_destroyed = true; }
        bool& is_destroyed;
    };

    bool is_destroyed = false;
    {
        libcpp::shared_ptr<Object> p(new Object(is_destroyed));
    }
    ASSERT_TRUE(is_destroyed);
}
