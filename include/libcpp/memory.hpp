#pragma once

namespace libcpp
{

struct control_block
{
    long strong_count;
    long weak_count;
};

template <typename T>
class shared_ptr
{
public:
    shared_ptr(T* ptr)
        : _ptr(ptr),
          _count(new control_block())
    {
    }

    T* get()
    {
        return _ptr;
    }

    long use_count()
    {
        return _count->strong_count;
    }

private:
    T* _ptr;
    control_block* _count;
};

}
