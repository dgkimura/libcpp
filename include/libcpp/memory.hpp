#pragma once

namespace libcpp
{

struct control_block
{
    long strong_count;
    long weak_count;

    long increment()
    {
        strong_count += 1;
        return strong_count;
    }

    long decrement()
    {
        strong_count -= 1;
        return strong_count;
    }
};

template <typename T>
class shared_ptr
{
public:
    shared_ptr(T* ptr)
        : _ptr(ptr),
          _count(new control_block())
    {
        _count->increment();
    }

    shared_ptr(const shared_ptr<T>& rhs)
    {
        _decrement_count();
        rhs._count->increment();
        _count = rhs._count;
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& rhs)
    {
        _decrement_count();
        rhs._count->increment();
        _count = rhs._count;

        return *this;
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
        _decrement_count();
    }

private:
    T* _ptr;
    control_block* _count;

    void _decrement_count()
    {
        if (_count->decrement() <= 0)
        {
            delete _count;
            delete _ptr;
        }
    }
};

}
