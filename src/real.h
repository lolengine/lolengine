//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Real class
// --------------
//

#if !defined __LOL_REAL_H__
#define __LOL_REAL_H__

#include <stdint.h>

namespace lol
{

class real
{
public:
    inline real() { }

    real(float f);
    real(double f);
    real(int i);
    real(unsigned int i);

    operator float() const;
    operator double() const;
    operator int() const;
    operator unsigned int() const;

    real operator +() const;
    real operator -() const;
    real operator +(real const &x) const;
    real operator -(real const &x) const;
    real operator *(real const &x) const;
    real operator /(real const &x) const;
    real &operator +=(real const &x);
    real &operator -=(real const &x);
    real &operator *=(real const &x);
    real &operator /=(real const &x);

    real operator <<(int x) const;
    real operator >>(int x) const;
    real &operator <<=(int x);
    real &operator >>=(int x);

    bool operator ==(real const &x) const;
    bool operator !=(real const &x) const;
    bool operator <(real const &x) const;
    bool operator >(real const &x) const;
    bool operator <=(real const &x) const;
    bool operator >=(real const &x) const;

    bool operator !() const;
    operator bool() const;

    /* Trigonometric functions */
    friend real sin(real const &x);
    friend real cos(real const &x);
    friend real tan(real const &x);
    friend real asin(real const &x);
    friend real acos(real const &x);
    friend real atan(real const &x);
    /* FIXME: atan2 */

    /* Hyperbolic functions */
    friend real sinh(real const &x);
    friend real cosh(real const &x);
    friend real tanh(real const &x);

    /* Exponential and logarithmic functions */
    friend real exp(real const &x);
    friend real exp2(real const &x);
    friend real log(real const &x);
    friend real log2(real const &x);
    friend real log10(real const &x);
    /* FIXME: frexp ldexp modf */

    /* Power functions */
    friend real re(real const &x);
    friend real sqrt(real const &x);
    friend real cbrt(real const &x);
    friend real pow(real const &x, real const &y);

    /* Rounding, absolute value, remainder etc. */
    friend real ceil(real const &x);
    friend real copysign(real const &x, real const &y);
    friend real floor(real const &x);
    friend real fabs(real const &x);
    friend real round(real const &x);
    friend real fmod(real const &x, real const &y);

    void hexprint() const;
    void print(int ndigits = 150) const;

    static real const R_0;
    static real const R_1;
    static real const R_2;
    static real const R_3;
    static real const R_10;

    static real const R_E;
    static real const R_LOG2E;
    static real const R_LOG10E;
    static real const R_LN2;
    static real const R_LN10;
    static real const R_PI;
    static real const R_PI_2;
    static real const R_PI_3;
    static real const R_PI_4;
    static real const R_1_PI;
    static real const R_2_PI;
    static real const R_2_SQRTPI;
    static real const R_SQRT2;
    static real const R_SQRT3;
    static real const R_SQRT1_2;

    /* XXX: changing this requires tuning real::fres (the number of
     * Newton-Raphson iterations) and real::print (the number of printed
     * digits) */
    static int const BIGITS = 16;
    static int const BIGIT_BITS = 32;

private:
    uint32_t m_size;
    uint32_t m_signexp;
    uint32_t m_mantissa[BIGITS];
};

} /* namespace lol */

#endif // __LOL_REAL_H__

