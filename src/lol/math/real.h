//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Real class
// --------------
//

#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers fuck with it. */
#undef min
#undef max

/* Avoid issues with NaCl headers */
#undef log2

/*
 * The base class for reals. The only real reason for making this a template
 * class is so we can have implicit constructors ("real x = 1" works) but
 * avoid accidental implicit conversions ("int x = 1; sqrt(x)" will never
 * call real::sqrt).
 */
template<int N> class Real
{
public:
    Real();
    Real(Real<N> const &x);
    Real const &operator =(Real<N> const &x);
    ~Real();

    Real(float f);
    Real(double f);
    Real(int32_t i);
    Real(uint32_t i);
    Real(int64_t i);
    Real(uint64_t i);

    Real(char const *str);

    operator float() const;
    operator double() const;
    operator int() const;
    operator unsigned int() const;

    Real<N> operator +() const;
    Real<N> operator -() const;
    Real<N> operator +(Real<N> const &x) const;
    Real<N> operator -(Real<N> const &x) const;
    Real<N> operator *(Real<N> const &x) const;
    Real<N> operator /(Real<N> const &x) const;
    Real<N> const &operator +=(Real<N> const &x);
    Real<N> const &operator -=(Real<N> const &x);
    Real<N> const &operator *=(Real<N> const &x);
    Real<N> const &operator /=(Real<N> const &x);

    bool operator ==(Real<N> const &x) const;
    bool operator !=(Real<N> const &x) const;
    bool operator <(Real<N> const &x) const;
    bool operator >(Real<N> const &x) const;
    bool operator <=(Real<N> const &x) const;
    bool operator >=(Real<N> const &x) const;

    bool operator !() const;
    operator bool() const;

    /* Comparison functions */
    template<int K> friend Real<K> min(Real<K> const &a, Real<K> const &b);
    template<int K> friend Real<K> max(Real<K> const &a, Real<K> const &b);
    template<int K> friend Real<K> clamp(Real<K> const &x,
                                         Real<K> const &a, Real<K> const &b);

    /* Trigonometric functions */
    template<int K> friend Real<K> sin(Real<K> const &x);
    template<int K> friend Real<K> cos(Real<K> const &x);
    template<int K> friend Real<K> tan(Real<K> const &x);
    template<int K> friend Real<K> asin(Real<K> const &x);
    template<int K> friend Real<K> acos(Real<K> const &x);
    template<int K> friend Real<K> atan(Real<K> const &x);
    template<int K> friend Real<K> atan2(Real<K> const &y, Real<K> const &x);

    /* Hyperbolic functions */
    template<int K> friend Real<K> sinh(Real<K> const &x);
    template<int K> friend Real<K> cosh(Real<K> const &x);
    template<int K> friend Real<K> tanh(Real<K> const &x);

    /* Exponential and logarithmic functions */
    template<int K> friend Real<K> exp(Real<K> const &x);
    template<int K> friend Real<K> exp2(Real<K> const &x);
    template<int K> friend Real<K> log(Real<K> const &x);
    template<int K> friend Real<K> log2(Real<K> const &x);
    template<int K> friend Real<K> log10(Real<K> const &x);
    template<int K> friend Real<K> frexp(Real<K> const &x, int *exp);
    template<int K> friend Real<K> ldexp(Real<K> const &x, int exp);
    template<int K> friend Real<K> modf(Real<K> const &x, Real<K> *iptr);
    template<int K> friend Real<K> ulp(Real<K> const &x);
    template<int K> friend Real<K> nextafter(Real<K> const &x, Real<K> const &y);

    /* Power functions */
    template<int K> friend Real<K> re(Real<K> const &x);
    template<int K> friend Real<K> sqrt(Real<K> const &x);
    template<int K> friend Real<K> cbrt(Real<K> const &x);
    template<int K> friend Real<K> pow(Real<K> const &x, Real<K> const &y);
    template<int K> friend Real<K> gamma(Real<K> const &x);

    /* Rounding, absolute value, remainder etc. */
    template<int K> friend Real<K> ceil(Real<K> const &x);
    template<int K> friend Real<K> copysign(Real<K> const &x, Real<K> const &y);
    template<int K> friend Real<K> floor(Real<K> const &x);
    template<int K> friend Real<K> fabs(Real<K> const &x);
    template<int K> friend Real<K> round(Real<K> const &x);
    template<int K> friend Real<K> fmod(Real<K> const &x, Real<K> const &y);

    /* Functions inherited from GLSL */
    template<int K> friend Real<K> abs(Real<K> const &x);
    template<int K> friend Real<K> fract(Real<K> const &x);
    template<int K> friend Real<K> degrees(Real<K> const &x);
    template<int K> friend Real<K> radians(Real<K> const &x);

    void hexprint() const;
    void print(int ndigits = 150) const;
    void sprintf(char *str, int ndigits = 150) const;

    /* Additional operators using base C++ types */
#define __LOL_REAL_OP_HELPER_GENERIC(op, type) \
    inline Real<N> operator op(type x) const { return *this op (Real<N>)x; } \
    inline Real<N> const &operator op##=(type x) { return *this = (*this op x); }
#define __LOL_REAL_OP_HELPER_FASTMULDIV(op, type) \
    inline Real<N> operator op(type x) const \
    { \
        Real<N> tmp = *this; return tmp op##= x; \
    } \
    inline Real<N> const &operator op##=(type x) \
    { \
        /* If multiplying or dividing by a power of two, take a shortcut */ \
        if ((m_signexp << 1) && x && !(x & (x - 1))) \
        { \
            while (x >>= 1) \
                m_signexp += 1 op 2 - 1; /* 1 if op is *, -1 if op is / */ \
        } \
        else \
            *this = *this op (Real<N>)x; \
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
    __LOL_REAL_OP_HELPER_FLOAT(float)
    __LOL_REAL_OP_HELPER_FLOAT(double)

    /* Constants */
    static Real<N> const& R_0();
    static Real<N> const& R_1();
    static Real<N> const& R_2();
    static Real<N> const& R_3();
    static Real<N> const& R_4();
    static Real<N> const& R_10();

    static Real<N> const& R_E();
    static Real<N> const& R_LOG2E();
    static Real<N> const& R_LOG10E();
    static Real<N> const& R_LN2();
    static Real<N> const& R_LN10();
    static Real<N> const& R_PI();
    static Real<N> const& R_PI_2();
    static Real<N> const& R_PI_3();
    static Real<N> const& R_PI_4();
    static Real<N> const& R_1_PI();
    static Real<N> const& R_2_PI();
    static Real<N> const& R_2_SQRTPI();
    static Real<N> const& R_SQRT2();
    static Real<N> const& R_SQRT3();
    static Real<N> const& R_SQRT1_2();

    /* XXX: changing this requires tuning real::fres (the number of
     * Newton-Raphson iterations) and real::print (the number of printed
     * digits) */
    static int const BIGITS = N;
    static int const BIGIT_BITS = 32;

private:
    uint32_t *m_mantissa;
    uint32_t m_signexp;
};

/*
 * Mandatory forward declarations of template specialisations
 */
template<> real::Real();
template<> real::Real(real const &x);
template<> real const &real::operator =(real const &x);
template<> real::~Real();
template<> real::Real(float f);
template<> real::Real(double f);
template<> real::Real(int32_t i);
template<> real::Real(uint32_t i);
template<> real::Real(int64_t i);
template<> real::Real(uint64_t i);
template<> real::Real(char const *str);

template<> real::operator float() const;
template<> real::operator double() const;
template<> real::operator int() const;
template<> real::operator unsigned int() const;
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
template<> bool real::operator ==(real const &x) const;
template<> bool real::operator !=(real const &x) const;
template<> bool real::operator <(real const &x) const;
template<> bool real::operator >(real const &x) const;
template<> bool real::operator <=(real const &x) const;
template<> bool real::operator >=(real const &x) const;
template<> bool real::operator !() const;
template<> real::operator bool() const;

template<int K> Real<K> min(Real<K> const &a, Real<K> const &b);
template<int K> Real<K> max(Real<K> const &a, Real<K> const &b);
template<int K> Real<K> clamp(Real<K> const &x,
                              Real<K> const &a, Real<K> const &b);

template<int K> Real<K> sin(Real<K> const &x);
template<int K> Real<K> cos(Real<K> const &x);
template<int K> Real<K> tan(Real<K> const &x);
template<int K> Real<K> asin(Real<K> const &x);
template<int K> Real<K> acos(Real<K> const &x);
template<int K> Real<K> atan(Real<K> const &x);
template<int K> Real<K> atan2(Real<K> const &y, Real<K> const &x);
template<int K> Real<K> sinh(Real<K> const &x);
template<int K> Real<K> cosh(Real<K> const &x);
template<int K> Real<K> tanh(Real<K> const &x);
template<int K> Real<K> exp(Real<K> const &x);
template<int K> Real<K> exp2(Real<K> const &x);
template<int K> Real<K> log(Real<K> const &x);
template<int K> Real<K> log2(Real<K> const &x);
template<int K> Real<K> log10(Real<K> const &x);
template<int K> Real<K> frexp(Real<K> const &x, int *exp);
template<int K> Real<K> ldexp(Real<K> const &x, int exp);
template<int K> Real<K> modf(Real<K> const &x, Real<K> *iptr);
template<int K> Real<K> ulp(Real<K> const &x);
template<int K> Real<K> nextafter(Real<K> const &x, Real<K> const &y);
template<int K> Real<K> re(Real<K> const &x);
template<int K> Real<K> sqrt(Real<K> const &x);
template<int K> Real<K> cbrt(Real<K> const &x);
template<int K> Real<K> pow(Real<K> const &x, Real<K> const &y);
template<int K> Real<K> gamma(Real<K> const &x);
template<int K> Real<K> ceil(Real<K> const &x);
template<int K> Real<K> copysign(Real<K> const &x, Real<K> const &y);
template<int K> Real<K> floor(Real<K> const &x);
template<int K> Real<K> fabs(Real<K> const &x);
template<int K> Real<K> round(Real<K> const &x);
template<int K> Real<K> fmod(Real<K> const &x, Real<K> const &y);
template<int K> Real<K> abs(Real<K> const &x);
template<int K> Real<K> fract(Real<K> const &x);
template<int K> Real<K> degrees(Real<K> const &x);
template<int K> Real<K> radians(Real<K> const &x);

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
template<> real log(real const &x);
template<> real log2(real const &x);
template<> real log10(real const &x);
template<> real frexp(real const &x, int *exp);
template<> real ldexp(real const &x, int exp);
template<> real modf(real const &x, real *iptr);
template<> real ulp(real const &x);
template<> real nextafter(real const &x, real const &y);
template<> real re(real const &x);
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

template<> void real::hexprint() const;
template<> void real::print(int ndigits) const;
template<> void real::sprintf(char *str, int ndigits) const;

} /* namespace lol */

