#pragma once

namespace libcpp
{

class control_block
{
private:
    long _strong_count;
    long _weak_count;
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

private:
    T* _ptr;
    control_block* _count;
};

}
