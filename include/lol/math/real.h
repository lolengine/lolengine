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
// The Real class
// ——————————————
//

#include <lol/base/features.h>
#include <lol/base/core.h>

#include <vector>
#include <string>
#include <cstdint>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers mess with it. */
#undef min
#undef max

template<typename T> class Real;
typedef Real<uint32_t> real;

/*
 * The base class for reals. The only real reason for making this a template
 * class is so we can have implicit constructors ("real x = 1" works) but
 * avoid accidental implicit conversions ("int x = 1; sqrt(x)" will never
 * call real::sqrt).
 */
template<typename T>
class LOL_ATTR_NODISCARD Real
{
public:
    typedef T bigit_t;
    typedef int64_t exponent_t;

    Real() = default;

    Real(float f);
    Real(double f);
    Real(long double f);
    Real(int32_t i);
    Real(uint32_t i);
    Real(int64_t i);
    Real(uint64_t i);

    Real(char const *str);

    static int global_bigit_count(int n = 0)
    {
        static int count = 16;
        return n <= 0 ? count : (count = n);
    }

    LOL_ATTR_NODISCARD bool is_zero() const { return m_mantissa.size() == 0; }
    LOL_ATTR_NODISCARD bool is_negative() const { return m_sign; }
    LOL_ATTR_NODISCARD bool is_nan() const { return m_nan; }
    LOL_ATTR_NODISCARD bool is_inf() const { return m_inf; }

    LOL_ATTR_NODISCARD operator float() const;
    LOL_ATTR_NODISCARD operator double() const;
    LOL_ATTR_NODISCARD operator long double() const;
    LOL_ATTR_NODISCARD operator int32_t() const;
    LOL_ATTR_NODISCARD operator uint32_t() const;
    LOL_ATTR_NODISCARD operator int64_t() const;
    LOL_ATTR_NODISCARD operator uint64_t() const;

    Real<T> operator +() const;
    Real<T> operator -() const;
    Real<T> operator +(Real<T> const &x) const;
    Real<T> operator -(Real<T> const &x) const;
    Real<T> operator *(Real<T> const &x) const;
    Real<T> operator /(Real<T> const &x) const;
    Real<T> const &operator +=(Real<T> const &x);
    Real<T> const &operator -=(Real<T> const &x);
    Real<T> const &operator *=(Real<T> const &x);
    Real<T> const &operator /=(Real<T> const &x);

    LOL_ATTR_NODISCARD bool operator ==(Real<T> const &x) const;
    LOL_ATTR_NODISCARD bool operator !=(Real<T> const &x) const;
    LOL_ATTR_NODISCARD bool operator <(Real<T> const &x) const;
    LOL_ATTR_NODISCARD bool operator >(Real<T> const &x) const;
    LOL_ATTR_NODISCARD bool operator <=(Real<T> const &x) const;
    LOL_ATTR_NODISCARD bool operator >=(Real<T> const &x) const;

    LOL_ATTR_NODISCARD bool operator !() const;
    LOL_ATTR_NODISCARD operator bool() const;

    /* Comparison functions */
    template<typename U> friend Real<U> min(Real<U> const &a, Real<U> const &b);
    template<typename U> friend Real<U> max(Real<U> const &a, Real<U> const &b);
    template<typename U> friend Real<U> clamp(Real<U> const &x,
                                              Real<U> const &a, Real<U> const &b);

    /* Trigonometric functions */
    template<typename U> friend Real<U> sin(Real<U> const &x);
    template<typename U> friend Real<U> cos(Real<U> const &x);
    template<typename U> friend Real<U> tan(Real<U> const &x);
    template<typename U> friend Real<U> asin(Real<U> const &x);
    template<typename U> friend Real<U> acos(Real<U> const &x);
    template<typename U> friend Real<U> atan(Real<U> const &x);
    template<typename U> friend Real<U> atan2(Real<U> const &y, Real<U> const &x);

    /* Hyperbolic functions */
    template<typename U> friend Real<U> sinh(Real<U> const &x);
    template<typename U> friend Real<U> cosh(Real<U> const &x);
    template<typename U> friend Real<U> tanh(Real<U> const &x);

    /* Exponential and logarithmic functions */
    template<typename U> friend Real<U> exp(Real<U> const &x);
    template<typename U> friend Real<U> exp2(Real<U> const &x);
    template<typename U> friend Real<U> erf(Real<U> const &x);
    template<typename U> friend Real<U> log(Real<U> const &x);
    template<typename U> friend Real<U> log2(Real<U> const &x);
    template<typename U> friend Real<U> log10(Real<U> const &x);

    /* Floating-point functions */
    template<typename U> friend Real<U> frexp(Real<U> const &x, typename Real<U>::exponent_t *exp);
    template<typename U> friend Real<U> ldexp(Real<U> const &x, typename Real<U>::exponent_t exp);
    template<typename U> friend Real<U> modf(Real<U> const &x, Real<U> *iptr);
    template<typename U> friend Real<U> nextafter(Real<U> const &x, Real<U> const &y);

    /* Power functions */
    template<typename U> friend Real<U> inverse(Real<U> const &x);
    template<typename U> friend Real<U> sqrt(Real<U> const &x);
    template<typename U> friend Real<U> cbrt(Real<U> const &x);
    template<typename U> friend Real<U> pow(Real<U> const &x, Real<U> const &y);
    template<typename U> friend Real<U> gamma(Real<U> const &x);

    /* Rounding, absolute value, remainder etc. */
    template<typename U> friend Real<U> ceil(Real<U> const &x);
    template<typename U> friend Real<U> copysign(Real<U> const &x, Real<U> const &y);
    template<typename U> friend Real<U> floor(Real<U> const &x);
    template<typename U> friend Real<U> fabs(Real<U> const &x);
    template<typename U> friend Real<U> round(Real<U> const &x);
    template<typename U> friend Real<U> fmod(Real<U> const &x, Real<U> const &y);

    /* Functions inherited from GLSL */
    template<typename U> friend Real<U> abs(Real<U> const &x);
    template<typename U> friend Real<U> fract(Real<U> const &x);
    template<typename U> friend Real<U> degrees(Real<U> const &x);
    template<typename U> friend Real<U> radians(Real<U> const &x);

    /* Additional functions */
    template<typename U> friend Real<U> franke(Real<U> const &x, Real<U> const &y);
    template<typename U> friend Real<U> peaks(Real<U> const &x, Real<U> const &y);

    std::string str(int ndigits = 150) const;
    std::string xstr() const;

    /* Additional operators using base C++ types */
#define __LOL_REAL_OP_HELPER_GENERIC(op, type) \
    inline Real<T> operator op(type x) const { return *this op (Real<T>)x; } \
    inline Real<T> const &operator op##=(type x) { return *this = (*this op x); }
#define __LOL_REAL_OP_HELPER_FASTMULDIV(op, type) \
    inline Real<T> operator op(type x) const \
    { \
        Real<T> tmp = *this; return tmp op##= x; \
    } \
    inline Real<T> const &operator op##=(type x) \
    { \
        /* If multiplying or dividing by a power of two, take a shortcut */ \
        if (!is_zero() && x && !(x & (x - 1))) \
        { \
            while (x >>= 1) \
                m_exponent += 1 op 2 - 1; /* 1 if op is *, -1 if op is / */ \
        } \
        else \
            *this = *this op (Real<T>)x; \
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

    /* Constants */
    static Real<T> const  R_0();
    static Real<T> const& R_1();
    static Real<T> const& R_2();
    static Real<T> const& R_3();
    static Real<T> const& R_4();
    static Real<T> const& R_10();

    static Real<T> const& R_E();
    static Real<T> const& R_LOG2E();
    static Real<T> const& R_LOG10E();
    static Real<T> const& R_LN2();
    static Real<T> const& R_LN10();
    static Real<T> const& R_PI();
    static Real<T> const& R_PI_2();
    static Real<T> const& R_PI_3();
    static Real<T> const& R_PI_4();
    static Real<T> const& R_TAU();
    static Real<T> const& R_1_PI();
    static Real<T> const& R_2_PI();
    static Real<T> const& R_2_SQRTPI();
    static Real<T> const& R_SQRT2();
    static Real<T> const& R_SQRT3();
    static Real<T> const& R_SQRT1_2();

    static Real<T> const  R_INF();
    static Real<T> const  R_NAN();

    static Real<T> const& R_MIN();
    static Real<T> const& R_MAX();

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
std::ostream& operator <<(std::ostream &s, Real<U> const &x);

/*
 * Mandatory forward declarations of template specialisations
 */
template<> real::Real(float f);
template<> real::Real(double f);
template<> real::Real(long double f);
template<> real::Real(int32_t i);
template<> real::Real(uint32_t i);
template<> real::Real(int64_t i);
template<> real::Real(uint64_t i);
template<> real::Real(char const *str);

template<> LOL_ATTR_NODISCARD real::operator float() const;
template<> LOL_ATTR_NODISCARD real::operator double() const;
template<> LOL_ATTR_NODISCARD real::operator long double() const;
template<> LOL_ATTR_NODISCARD real::operator int32_t() const;
template<> LOL_ATTR_NODISCARD real::operator uint32_t() const;
template<> LOL_ATTR_NODISCARD real::operator int64_t() const;
template<> LOL_ATTR_NODISCARD real::operator uint64_t() const;
template<> real real::operator +() const;
template<> real real::operator -() const;
template<> real real::operator +(real const &x) const;
template<> real real::operator -(real const &x) const;
template<> real real::operator *(real const &x) const;
template<> real real::operator /(real const &x) const;
template<> real const &real::operator +=(real const &x);
template<> real const &real::operator -=(real const &x);
template<> real const &real::operator *=(real const &x);
template<> real const &real::operator /=(real const &x);
template<> LOL_ATTR_NODISCARD bool real::operator ==(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator !=(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator <(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator >(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator <=(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator >=(real const &x) const;
template<> LOL_ATTR_NODISCARD bool real::operator !() const;
template<> LOL_ATTR_NODISCARD real::operator bool() const;

template<typename U> Real<U> min(Real<U> const &a, Real<U> const &b);
template<typename U> Real<U> max(Real<U> const &a, Real<U> const &b);
template<typename U> Real<U> clamp(Real<U> const &x,
                                   Real<U> const &a, Real<U> const &b);

template<typename U> Real<U> sin(Real<U> const &x);
template<typename U> Real<U> cos(Real<U> const &x);
template<typename U> Real<U> tan(Real<U> const &x);
template<typename U> Real<U> asin(Real<U> const &x);
template<typename U> Real<U> acos(Real<U> const &x);
template<typename U> Real<U> atan(Real<U> const &x);
template<typename U> Real<U> atan2(Real<U> const &y, Real<U> const &x);
template<typename U> Real<U> sinh(Real<U> const &x);
template<typename U> Real<U> cosh(Real<U> const &x);
template<typename U> Real<U> tanh(Real<U> const &x);
template<typename U> Real<U> exp(Real<U> const &x);
template<typename U> Real<U> exp2(Real<U> const &x);
template<typename U> Real<U> erf(Real<U> const &x);
template<typename U> Real<U> log(Real<U> const &x);
template<typename U> Real<U> log2(Real<U> const &x);
template<typename U> Real<U> log10(Real<U> const &x);
template<typename U> Real<U> frexp(Real<U> const &x, typename Real<U>::exponent_t *exp);
template<typename U> Real<U> ldexp(Real<U> const &x, typename Real<U>::exponent_t exp);
template<typename U> Real<U> modf(Real<U> const &x, Real<U> *iptr);
template<typename U> Real<U> nextafter(Real<U> const &x, Real<U> const &y);
template<typename U> Real<U> inverse(Real<U> const &x);
template<typename U> Real<U> sqrt(Real<U> const &x);
template<typename U> Real<U> cbrt(Real<U> const &x);
template<typename U> Real<U> pow(Real<U> const &x, Real<U> const &y);
template<typename U> Real<U> gamma(Real<U> const &x);
template<typename U> Real<U> ceil(Real<U> const &x);
template<typename U> Real<U> copysign(Real<U> const &x, Real<U> const &y);
template<typename U> Real<U> floor(Real<U> const &x);
template<typename U> Real<U> fabs(Real<U> const &x);
template<typename U> Real<U> round(Real<U> const &x);
template<typename U> Real<U> fmod(Real<U> const &x, Real<U> const &y);
template<typename U> Real<U> abs(Real<U> const &x);
template<typename U> Real<U> fract(Real<U> const &x);
template<typename U> Real<U> degrees(Real<U> const &x);
template<typename U> Real<U> radians(Real<U> const &x);
template<typename U> Real<U> franke(Real<U> const &x, Real<U> const &y);
template<typename U> Real<U> peaks(Real<U> const &x, Real<U> const &y);

template<> real min(real const &a, real const &b);
template<> real max(real const &a, real const &b);
template<> real clamp(real const &x, real const &a, real const &b);

template<> real sin(real const &x);
template<> real cos(real const &x);
template<> real tan(real const &x);
template<> real asin(real const &x);
template<> real acos(real const &x);
template<> real atan(real const &x);
template<> real atan2(real const &y, real const &x);
template<> real sinh(real const &x);
template<> real cosh(real const &x);
template<> real tanh(real const &x);
template<> real exp(real const &x);
template<> real exp2(real const &x);
template<> real erf(real const &x);
template<> real log(real const &x);
template<> real log2(real const &x);
template<> real log10(real const &x);
template<> real frexp(real const &x, real::exponent_t *exp);
template<> real ldexp(real const &x, real::exponent_t exp);
template<> real modf(real const &x, real *iptr);
template<> real nextafter(real const &x, real const &y);
template<> real inverse(real const &x);
template<> real sqrt(real const &x);
template<> real cbrt(real const &x);
template<> real pow(real const &x, real const &y);
template<> real gamma(real const &x);
template<> real ceil(real const &x);
template<> real copysign(real const &x, real const &y);
template<> real floor(real const &x);
template<> real fabs(real const &x);
template<> real round(real const &x);
template<> real fmod(real const &x, real const &y);
template<> real abs(real const &x);
template<> real fract(real const &x);
template<> real degrees(real const &x);
template<> real radians(real const &x);
template<> real franke(real const &x, real const &y);
template<> real peaks(real const &x, real const &y);

template<> std::string real::str(int ndigits) const;
template<> std::string real::xstr() const;

} /* namespace lol */

#include "../../../legacy/math/real.cpp"

