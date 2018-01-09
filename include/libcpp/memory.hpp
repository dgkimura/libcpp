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
        _count->strong_count += 1;
    }

    T* get()
    {
        return _ptr;
    }

    long use_count()
    {
        return _count->strong_count;
    }

    ~shared_ptr()
    {
        _count->strong_count -= 1;
        if (_count->strong_count <= 0)
        {
            delete _count;
            delete _ptr;
        }
    }

private:
    T* _ptr;
    control_block* _count;
};

}
