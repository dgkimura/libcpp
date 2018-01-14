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

TEST(MemoryTest, testSharedPtrCopyConstructorIncrementsCount)
{
    libcpp::shared_ptr<int> p1(nullptr);
    libcpp::shared_ptr<int> p2 = p1;

    ASSERT_EQ(p1.use_count(), 2);
    ASSERT_EQ(p2.use_count(), 2);
}

TEST(MemoryTest, testSharedPtrAssignmentOperatorIncrementsCount)
{
    libcpp::shared_ptr<int> p1(nullptr);
    libcpp::shared_ptr<int> p2(nullptr);
    p2 = p1;

    ASSERT_EQ(p1.use_count(), 2);
    ASSERT_EQ(p2.use_count(), 2);
}

TEST(MemoryTest, testSharedPtrReferenceDecrementsWhenInstanceIsOutOfScope)
{
    libcpp::shared_ptr<int> p1(nullptr);
    {
        libcpp::shared_ptr<int> p2(nullptr);
        p2 = p1;
    }

    ASSERT_EQ(p1.use_count(), 1);
}

TEST(MemoryTest, testUniquePtrDeletesAllocatedMemoryAfterGoingOutOfScope)
{
    struct Object
    {
        Object(bool& is_destroyed) : is_destroyed(is_destroyed) {}
        ~Object(){ is_destroyed = true; }
        bool& is_destroyed;
    };

    bool is_destroyed = false;
    {
        libcpp::unique_ptr<Object> p(new Object(is_destroyed));
    }
    ASSERT_TRUE(is_destroyed);
}

TEST(MemoryTest, testUniquePtrCanSwitchOwnershipToAnotherUniquePtr)
{
    struct Object
    {
        Object(bool& is_destroyed) : is_destroyed(is_destroyed) {}
        ~Object(){ is_destroyed = true; }
        bool& is_destroyed;
    };

    bool is_destroyed = false;
    {
        libcpp::unique_ptr<Object> p1;
        {
            libcpp::unique_ptr<Object> p2(new Object(is_destroyed));
            p1 = std::move(p2);

            // Object should not be destoyed yet because it is now owned by p1
            // which is still in scope.
            ASSERT_FALSE(is_destroyed);
        }
    }

    // Obect should now be destroyed because p1 is now out of scope.
    ASSERT_TRUE(is_destroyed);
}
