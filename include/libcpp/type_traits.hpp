#pragma once

namespace libcpp
{

// Represents a value known at compile-time as a type.
template <typename T, T v>
struct integral_constant
{
    using type = T;

    static constexpr T value = v;
};

using true_type = integral_constant<bool, true>;

using false_type = integral_constant<bool, false>;

template <typename T> struct is_integral : false_type {};
template <> struct is_integral<bool> : true_type {};

template <typename T> struct is_floating_point : false_type {};
template <> struct is_floating_point<float> : true_type {};
template <> struct is_floating_point<double> : true_type {};
template <> struct is_floating_point<long double> : true_type {};

template <typename T>
struct is_arithmetic : integral_constant<bool,
                                         is_integral<T>::value ||
                                         is_floating_point<T>::value> {};

}
