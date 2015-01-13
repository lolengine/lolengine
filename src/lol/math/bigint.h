//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The bigint class
// ----------------
//

#include <lol/base/types.h>

#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers fuck with it. */
#undef min
#undef max

/* Avoid issues with NaCl headers */
#undef log2

/*
 * A bigint stores its digits in an array of integers. The MSB of the
 * integers are unused. The highest used bit is the sign bit.
 *
 * Digits are stored in little endian mode.
 */

template<unsigned int N = 16, typename T = uint32_t>
class bigint
{
    int const bits_per_digit = sizeof(T) * 8 - 1;
    T const digit_mask = ~((T)1 << bits_per_digit);

public:
    inline bigint()
    {
    }

    explicit bigint(int32_t x)
    {
        /* FIXME: doesn’t work if sizeof(T) < sizeof(int32_t) */
        for (auto &digit : m_digits)
            digit = x >= 0 ? (T)0 : digit_mask;
        if (N > 0)
            m_digits[0] = x & digit_mask;
    }

    explicit bigint(uint32_t x)
    {
        /* FIXME: doesn’t work if sizeof(T) > sizeof(uint32_t) */
        for (auto &digit : m_digits)
            digit = (T)0;
        if (N > 0)
            m_digits[0] = x & digit_mask;
        if (N > 1)
            m_digits[1] = x >> bits_per_digit;
    }

    explicit inline operator uint32_t() const
    {
        /* FIXME: doesn’t work if sizeof(T) != sizeof(int32_t) */
        if (N < 1)
            return 0;
        if (N < 2)
            return (uint32_t)(m_digits[0]
                               | ((m_digits[0] << 1) & ~digit_mask));

        return (uint32_t)(m_digits[0] | (m_digits[1] << bits_per_digit));
    }

    inline operator int32_t() const
    {
        return (int32_t)(uint32_t)*this;
    }

    /*
     * bigint cast to bigint of different size: we copy all digits then
     * pad the rest (if applicable) with zeroes or ones to extend the
     * sign bit.
     */
    template<int M>
    explicit bigint(bigint<M,T> const &x)
    {
        for (int i = 0; i < (N < M) ? N : M; ++i)
            m_digits[i] = x.m_digits[i];

        if (N > M)
        {
            T padding = x.is_negative() ? digit_mask : (T)0;
            for (int i = M; i < N; ++i)
                m_digits[i] = padding;
        }
    }

    /*
     * bigint addition: we add the digits one-to-one, carrying overflows,
     * and replace digits with padding if one of the two operands is
     * shorter.
     */
    template<int M>
    bigint<(N > M) ? N : M, T> operator +(bigint<M,T> const &x) const
    {
        bigint<(N > M) ? N : M, T> ret;
        T padding = is_negative() ? digit_mask : (T)0;
        T x_padding = x.is_negative() ? digit_mask : (T)0;
        T carry(0);
        for (int i = 0; i < (N > M) ? N : M; ++i)
        {
            T digit = (i < N ? m_digits[i] : padding)
                    + (i < M ? x.m_digits[i] : x_padding)
                    + carry;
            ret.m_digits[i] = digit & digit_mask;
            carry = (digit & ~digit_mask) ? T(1) : T(0);
        }
        return ret;
    }

private:
    inline bool is_negative() const
    {
        if (N < 1)
            return false;
        return (m_digits[N - 1] & ((T)1 << (bits_per_digit - 1))) != 0;
    }

    T m_digits[N];
};

} /* namespace lol */

