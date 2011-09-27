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

    bool operator ==(real const &x) const;
    bool operator !=(real const &x) const;
    bool operator <(real const &x) const;
    bool operator >(real const &x) const;
    bool operator <=(real const &x) const;
    bool operator >=(real const &x) const;

    friend real fres(real const &x);
    friend real sqrt(real const &x);
    friend real exp(real const &x);

    void print(int ndigits = 150) const;

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

