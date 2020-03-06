//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The real number class
// —————————————————————
//

#include "../features.h"

#include <limits>   // std::numeric_limits
#include <vector>   // std::vector
#include <string>   // std::string
#include <stdint.h> // int64_t etc.

#if _MSC_VER
#   pragma push_macro("min")
#   pragma push_macro("max")
#   undef min
#   undef max
#endif

namespace lol
{

//
// The base class for reals. The only real reason for making this a template
// class is so we can have implicit constructors ("real x = 1" works) but
// avoid accidental implicit conversions ("int x = 1; sqrt(x)" will never
// call real::sqrt).
//
template<typename T>
class [[nodiscard]] real_t
{
public:
    typedef T bigit_t;
    typedef int64_t exponent_t;

    real_t() = default;

    real_t(float f);
    real_t(double f);
    real_t(long double f);
    real_t(int32_t i);
    real_t(uint32_t i);
    real_t(int64_t i);
    real_t(uint64_t i);

    real_t(char const *str);

    static int global_bigit_count(int n = 0)
    {
        static int count = 16;
        return n <= 0 ? count : (count = n);
    }

    [[nodiscard]] bool is_zero() const { return m_mantissa.size() == 0; }
    [[nodiscard]] bool is_negative() const { return m_sign; }
    [[nodiscard]] bool is_nan() const { return m_nan; }
    [[nodiscard]] bool is_inf() const { return m_inf; }

    // Mark all these operators explicit to avoid surprises
    [[nodiscard]] explicit operator float() const;
    [[nodiscard]] explicit operator double() const;
    [[nodiscard]] explicit operator long double() const;
    [[nodiscard]] explicit operator int32_t() const;
    [[nodiscard]] explicit operator uint32_t() const;
    [[nodiscard]] explicit operator int64_t() const;
    [[nodiscard]] explicit operator uint64_t() const;

    real_t<T> operator +() const;
    real_t<T> operator -() const;
    real_t<T> operator +(real_t<T> const &x) const;
    real_t<T> operator -(real_t<T> const &x) const;
    real_t<T> operator *(real_t<T> const &x) const;
    real_t<T> operator /(real_t<T> const &x) const;
    real_t<T> const &operator +=(real_t<T> const &x);
    real_t<T> const &operator -=(real_t<T> const &x);
    real_t<T> const &operator *=(real_t<T> const &x);
    real_t<T> const &operator /=(real_t<T> const &x);

    [[nodiscard]] bool operator ==(real_t<T> const &x) const;
    [[nodiscard]] bool operator !=(real_t<T> const &x) const;
    [[nodiscard]] bool operator <(real_t<T> const &x) const;
    [[nodiscard]] bool operator >(real_t<T> const &x) const;
    [[nodiscard]] bool operator <=(real_t<T> const &x) const;
    [[nodiscard]] bool operator >=(real_t<T> const &x) const;

    [[nodiscard]] bool operator !() const;
    [[nodiscard]] explicit operator bool() const;

    // Comparison functions
    template<typename U> friend real_t<U> min(real_t<U> const &a,
                                              real_t<U> const &b);
    template<typename U> friend real_t<U> max(real_t<U> const &a,
                                              real_t<U> const &b);
    template<typename U> friend real_t<U> clamp(real_t<U> const &x,
                                                real_t<U> const &a,
                                                real_t<U> const &b);

    // Trigonometric functions
    template<typename U> friend real_t<U> sin(real_t<U> const &x);
    template<typename U> friend real_t<U> cos(real_t<U> const &x);
    template<typename U> friend real_t<U> tan(real_t<U> const &x);
    template<typename U> friend real_t<U> asin(real_t<U> const &x);
    template<typename U> friend real_t<U> acos(real_t<U> const &x);
    template<typename U> friend real_t<U> atan(real_t<U> const &x);
    template<typename U> friend real_t<U> atan2(real_t<U> const &y,
                                                real_t<U> const &x);

    // Hyperbolic functions
    template<typename U> friend real_t<U> sinh(real_t<U> const &x);
    template<typename U> friend real_t<U> cosh(real_t<U> const &x);
    template<typename U> friend real_t<U> tanh(real_t<U> const &x);

    // Exponential and logarithmic functions
    template<typename U> friend real_t<U> exp(real_t<U> const &x);
    template<typename U> friend real_t<U> exp2(real_t<U> const &x);
    template<typename U> friend real_t<U> erf(real_t<U> const &x);
    template<typename U> friend real_t<U> log(real_t<U> const &x);
    template<typename U> friend real_t<U> log2(real_t<U> const &x);
    template<typename U> friend real_t<U> log10(real_t<U> const &x);

    // Floating-point functions
    template<typename U> friend real_t<U> frexp(real_t<U> const &x,
                                                typename real_t<U>::exponent_t *exp);
    template<typename U> friend real_t<U> ldexp(real_t<U> const &x,
                                                typename real_t<U>::exponent_t exp);
    template<typename U> friend real_t<U> modf(real_t<U> const &x,
                                               real_t<U> *iptr);
    template<typename U> friend real_t<U> nextafter(real_t<U> const &x,
                                                    real_t<U> const &y);

    // Power functions
    template<typename U> friend real_t<U> inverse(real_t<U> const &x);
    template<typename U> friend real_t<U> sqrt(real_t<U> const &x);
    template<typename U> friend real_t<U> cbrt(real_t<U> const &x);
    template<typename U> friend real_t<U> pow(real_t<U> const &x,
                                              real_t<U> const &y);
    template<typename U> friend real_t<U> gamma(real_t<U> const &x);

    // Rounding, absolute value, remainder etc.
    template<typename U> friend real_t<U> ceil(real_t<U> const &x);
    template<typename U> friend real_t<U> copysign(real_t<U> const &x,
                                                   real_t<U> const &y);
    template<typename U> friend real_t<U> floor(real_t<U> const &x);
    template<typename U> friend real_t<U> fabs(real_t<U> const &x);
    template<typename U> friend real_t<U> round(real_t<U> const &x);
    template<typename U> friend real_t<U> fmod(real_t<U> const &x,
                                               real_t<U> const &y);

    // Functions inherited from GLSL
    template<typename U> friend real_t<U> fract(real_t<U> const &x);
    template<typename U> friend real_t<U> degrees(real_t<U> const &x);
    template<typename U> friend real_t<U> radians(real_t<U> const &x);

    // Additional functions
    template<typename U> friend real_t<U> franke(real_t<U> const &x,
                                                 real_t<U> const &y);
    template<typename U> friend real_t<U> peaks(real_t<U> const &x,
                                                real_t<U> const &y);

    std::string str(int ndigits = 150) const;
    std::string xstr() const;

    // Additional operators using base C++ types
#define __LOL_REAL_OP_HELPER_GENERIC(op, type) \
    inline real_t<T> operator op(type x) const { return *this op (real_t<T>)x; } \
    inline real_t<T> const &operator op##=(type x) { return *this = (*this op x); }
#define __LOL_REAL_OP_HELPER_FASTMULDIV(op, type) \
    inline real_t<T> operator op(type x) const \
    { \
        real_t<T> tmp = *this; return tmp op##= x; \
    } \
    inline real_t<T> const &operator op##=(type x) \
    { \
        /* If multiplying or dividing by a power of two, take a shortcut */ \
        if (!is_zero() && x && !(x & (x - 1))) \
        { \
            while (x >>= 1) \
                m_exponent += 1 op 2 - 1; /* 1 if op is *, -1 if op is / */ \
        } \
        else \
            *this = *this op (real_t<T>)x; \
        return *this; \
    }
#define __LOL_REAL_OP_HELPER_INT(type) \
    __LOL_REAL_OP_HELPER_GENERIC(+, type) \
    __LOL_REAL_OP_HELPER_GENERIC(-, type) \
    __LOL_REAL_OP_HELPER_FASTMULDIV(*, type) \
    __LOL_REAL_OP_HELPER_FASTMULDIV(/, type)
#define __LOL_REAL_OP_HELPER_FLOAT(type) \
    __LOL_REAL_OP_HELPER_GENERIC(+, type) \
    __LOL_REAL_OP_HELPER_GENERIC(-, type) \
    __LOL_REAL_OP_HELPER_GENERIC(*, type) \
    __LOL_REAL_OP_HELPER_GENERIC(/, type)

    __LOL_REAL_OP_HELPER_INT(int)
    __LOL_REAL_OP_HELPER_INT(unsigned int)
    __LOL_REAL_OP_HELPER_INT(int64_t)
    __LOL_REAL_OP_HELPER_INT(uint64_t)
    __LOL_REAL_OP_HELPER_FLOAT(float)
    __LOL_REAL_OP_HELPER_FLOAT(double)
    __LOL_REAL_OP_HELPER_FLOAT(long double)

    // Constants
    static real_t<T> const  R_0();
    static real_t<T> const& R_1();
    static real_t<T> const& R_2();
    static real_t<T> const& R_3();
    static real_t<T> const& R_4();
    static real_t<T> const& R_10();

    static real_t<T> const& R_E();
    static real_t<T> const& R_LOG2E();
    static real_t<T> const& R_LOG10E();
    static real_t<T> const& R_LN2();
    static real_t<T> const& R_LN10();
    static real_t<T> const& R_PI();
    static real_t<T> const& R_PI_2();
    static real_t<T> const& R_PI_3();
    static real_t<T> const& R_PI_4();
    static real_t<T> const& R_TAU();
    static real_t<T> const& R_1_PI();
    static real_t<T> const& R_2_PI();
    static real_t<T> const& R_2_SQRTPI();
    static real_t<T> const& R_SQRT2();
    static real_t<T> const& R_SQRT3();
    static real_t<T> const& R_SQRT1_2();

    static real_t<T> const  R_INF();
    static real_t<T> const  R_NAN();

    static real_t<T> const& R_MIN();
    static real_t<T> const& R_MAX();

private:
    std::vector<T> m_mantissa;
    exponent_t m_exponent = 0;
    bool m_sign = false, m_nan = false, m_inf = false;

public:
    static int const DEFAULT_BIGIT_COUNT = 16;

    static inline int bigit_bits() { return 8 * (int)sizeof(bigit_t); }
    inline int bigit_count() const { return (int)m_mantissa.size(); }
    inline int total_bits() const { return bigit_count() * bigit_bits(); }
};

template<typename U>
std::ostream& operator <<(std::ostream &s, real_t<U> const &x);

typedef real_t<uint32_t> real;

} // namespace lol

#include "real.ipp"

#if _MSC_VER
#   pragma pop_macro("min")
#   pragma pop_macro("max")
#endif
