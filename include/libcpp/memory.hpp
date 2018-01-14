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
    shared_ptr()
        : _ptr(nullptr),
          _count(new control_block())
    {
    }

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

    // Allow weak_ptr to share private member variable _count.
    template <typename _T> friend class weak_ptr;
};

template <typename T>
class weak_ptr
{
public:
    weak_ptr()
        : _ptr(nullptr),
          _count(new control_block())
    {
    }

    weak_ptr(T* ptr)
        : _ptr(ptr),
          _count(new control_block())
    {
    }

    weak_ptr(shared_ptr<T> ptr)
        : _ptr(ptr.get()),
          _count(ptr._count)
    {
    }

    weak_ptr(const weak_ptr<T>& rhs)
    {
        _count->weak_count -= 1;
        rhs._count->weak_count += 1;
        _count = rhs._count;
    }

    weak_ptr<T>& operator=(const weak_ptr<T>& rhs)
    {
        _count->weak_count -= 1;
        rhs._count->weak_count += 1;
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

    bool expired()
    {
        return use_count() == 0;
    }

    shared_ptr<T> lock()
    {
        return expired() ? shared_ptr<T>() : shared_ptr<T>(_ptr);
    }

    ~weak_ptr()
    {
    }

private:
    T* _ptr;
    control_block* _count;
};

}
