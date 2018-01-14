#pragma once

namespace libcpp
{

template <typename T>
class unique_ptr
{
public:
    unique_ptr()
        : _ptr(nullptr)
    {
    }

    unique_ptr(T* ptr)
        : _ptr(ptr)
    {
    }

    unique_ptr(const unique_ptr<T>& rhs) = delete;

    unique_ptr<T>& operator=(const unique_ptr<T>& rhs) = delete;

    unique_ptr<T>& operator=(unique_ptr<T>&& rhs)
    {
        swap(rhs);
        return *this;
    }

    T* get()
    {
        return _ptr;
    }

    void release()
    {
        _ptr = nullptr;
    }

    void swap(unique_ptr& other)
    {
        T* t = other._ptr;
        other._ptr = _ptr;
        _ptr = t;
    }

    ~unique_ptr()
    {
        delete _ptr;
    }

private:
    T* _ptr;
};

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
