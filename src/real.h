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

    friend real fabs(real const &x);

    friend real fres(real const &x);
    friend real sqrt(real const &x);
    friend real log(real const &x);
    friend real exp(real const &x);

    friend real sin(real const &x);
    friend real cos(real const &x);

    void print(int ndigits = 150) const;

    static real const R_E;
    static real const R_LOG2E;
    static real const R_LOG10E;
    static real const R_LN2;
    static real const R_LN10;
    static real const R_PI;
    static real const R_PI_2;
    static real const R_PI_4;
    static real const R_1_PI;
    static real const R_2_PI;
    static real const R_2_SQRTPI;
    static real const R_SQRT2;
    static real const R_SQRT1_2;

private:
    /* XXX: changing this requires tuning real::fres (the number of
     * Newton-Raphson iterations) and real::print (the number of printed
     * digits) */
    static int const BIGITS = 32;

    uint32_t m_size;
    uint32_t m_signexp;
    uint16_t m_mantissa[BIGITS];
};

} /* namespace lol */

#endif // __LOL_REAL_H__

