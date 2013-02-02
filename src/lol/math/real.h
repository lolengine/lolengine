//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Real class
// --------------
//

#if !defined __LOL_MATH_REAL_H__
#define __LOL_MATH_REAL_H__

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
    Real(int i);
    Real(unsigned int i);

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
    template<int M> friend Real<M> min(Real<M> const &a, Real<M> const &b);
    template<int M> friend Real<M> max(Real<M> const &a, Real<M> const &b);
    template<int M> friend Real<M> clamp(Real<M> const &x,
                                         Real<M> const &a, Real<M> const &b);

    /* Trigonometric functions */
    template<int M> friend Real<M> sin(Real<M> const &x);
    template<int M> friend Real<M> cos(Real<M> const &x);
    template<int M> friend Real<M> tan(Real<M> const &x);
    template<int M> friend Real<M> asin(Real<M> const &x);
    template<int M> friend Real<M> acos(Real<M> const &x);
    template<int M> friend Real<M> atan(Real<M> const &x);
    template<int M> friend Real<M> atan2(Real<M> const &y, Real<M> const &x);

    /* Hyperbolic functions */
    template<int M> friend Real<M> sinh(Real<M> const &x);
    template<int M> friend Real<M> cosh(Real<M> const &x);
    template<int M> friend Real<M> tanh(Real<M> const &x);

    /* Exponential and logarithmic functions */
    template<int M> friend Real<M> exp(Real<M> const &x);
    template<int M> friend Real<M> exp2(Real<M> const &x);
    template<int M> friend Real<M> log(Real<M> const &x);
    template<int M> friend Real<M> log2(Real<M> const &x);
    template<int M> friend Real<M> log10(Real<M> const &x);
    template<int M> friend Real<M> frexp(Real<M> const &x, int *exp);
    template<int M> friend Real<M> ldexp(Real<M> const &x, int exp);
    template<int M> friend Real<M> modf(Real<M> const &x, Real<M> *iptr);
    template<int M> friend Real<M> ulp(Real<M> const &x);
    template<int M> friend Real<M> nextafter(Real<M> const &x, Real<M> const &y);

    /* Power functions */
    template<int M> friend Real<M> re(Real<M> const &x);
    template<int M> friend Real<M> sqrt(Real<M> const &x);
    template<int M> friend Real<M> cbrt(Real<M> const &x);
    template<int M> friend Real<M> pow(Real<M> const &x, Real<M> const &y);
    template<int M> friend Real<M> gamma(Real<M> const &x);

    /* Rounding, absolute value, remainder etc. */
    template<int M> friend Real<M> ceil(Real<M> const &x);
    template<int M> friend Real<M> copysign(Real<M> const &x, Real<M> const &y);
    template<int M> friend Real<M> floor(Real<M> const &x);
    template<int M> friend Real<M> fabs(Real<M> const &x);
    template<int M> friend Real<M> round(Real<M> const &x);
    template<int M> friend Real<M> fmod(Real<M> const &x, Real<N> const &y);

    /* Functions inherited from GLSL */
    template<int M> friend Real<M> abs(Real<M> const &x);
    template<int M> friend Real<M> fract(Real<M> const &x);

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
template<> real::Real(int i);
template<> real::Real(unsigned int i);
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

template<> void real::hexprint() const;
template<> void real::print(int ndigits) const;

} /* namespace lol */

#endif // __LOL_MATH_REAL_H__

