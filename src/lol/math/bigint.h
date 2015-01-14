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
        for (unsigned int i = 0; i < (N < M) ? N : M; ++i)
            m_digits[i] = x.m_digits[i];

        if (N > M)
        {
            T padding = x.is_negative() ? digit_mask : (T)0;
            for (unsigned int i = M; i < N; ++i)
                m_digits[i] = padding;
        }
    }

    /*
     * bigint bitwise not: we just flip all bits except the unused one.
     */
    bigint<N,T> operator ~() const
    {
        bigint<N,T> ret;
        for (unsigned int i = 0; i < N; ++i)
            ret.m_digits[i] = m_digits[i] ^ digit_mask;
        return ret;
    }

    /*
     * bigint bitwise AND: just perform a bitwise AND on all digits.
     */
    bigint<N,T> & operator &=(bigint<N,T> const &x)
    {
        for (unsigned int i = 0; i < N; ++i)
            m_digits[i] &= x.m_digits[i];
    }

    inline bigint<N,T> operator &(bigint<N,T> const &x) const
    {
        return bigint<N,T>(*this) &= x;
    }

    /*
     * bigint bitwise OR: just perform a bitwise OR on all digits.
     */
    bigint<N,T> & operator |=(bigint<N,T> const &x)
    {
        for (unsigned int i = 0; i < N; ++i)
            m_digits[i] |= x.m_digits[i];
    }

    inline bigint<N,T> operator |(bigint<N,T> const &x) const
    {
        return bigint<N,T>(*this) |= x;
    }

    /*
     * bigint bitwise XOR: just perform a bitwise XOR on all digits.
     */
    bigint<N,T> & operator ^=(bigint<N,T> const &x)
    {
        for (unsigned int i = 0; i < N; ++i)
            m_digits[i] ^= x.m_digits[i];
    }

    inline bigint<N,T> operator ^(bigint<N,T> const &x) const
    {
        return bigint<N,T>(*this) ^= x;
    }

    /*
     * bigint unary plus: a no-op
     */
    inline bigint<N,T> operator +() const
    {
        return *this;
    }

    /*
     * bigint unary minus: flip bits and add one
     */
    bigint<N,T> operator -() const
    {
        bigint<N,T> ret;
        T carry(1);
        for (unsigned int i = 0; i < N; ++i)
        {
            T digit = (m_digits[i] ^ digit_mask) + carry;
            ret.m_digits[i] = digit & digit_mask;
            carry = (digit & ~digit_mask) ? T(1) : T(0);
        }
        return ret;
    }

    /*
     * bigint addition: we add the digits one-to-one, carrying overflows,
     * and pad missing digits if one of the two operands is shorter.
     */
    template<unsigned int M>
    bigint<(N > M) ? N : M, T> operator +(bigint<M,T> const &x) const
    {
        bigint<(N > M) ? N : M, T> ret;
        T padding = is_negative() ? digit_mask : (T)0;
        T x_padding = x.is_negative() ? digit_mask : (T)0;
        T carry(0);
        for (unsigned int i = 0; i < ((N > M) ? N : M); ++i)
        {
            T digit = (i < N ? m_digits[i] : padding)
                    + (i < M ? x.m_digits[i] : x_padding)
                    + carry;
            ret.m_digits[i] = digit & digit_mask;
            carry = (digit & ~digit_mask) ? T(1) : T(0);
        }
        return ret;
    }

    /*
     * bigint subtraction: a combination of addition and unary minus;
     * we add the result of flipping digits and adding one.
     */
    template<unsigned int M>
    bigint<(N > M) ? N : M, T> operator -(bigint<M,T> const &x) const
    {
        bigint<(N > M) ? N : M, T> ret;
        T padding = is_negative() ? digit_mask : (T)0;
        T x_padding = x.is_negative() ? digit_mask : (T)0;
        T carry(1);
        for (unsigned int i = 0; i < ((N > M) ? N : M); ++i)
        {
            T digit = (i < N ? m_digits[i] : padding)
                    + ((i < M ? x.m_digits[i] : x_padding) ^ digit_mask)
                    + carry;
            ret.m_digits[i] = digit & digit_mask;
            carry = (digit & ~digit_mask) ? T(1) : T(0);
        }
        return ret;
    }

    /*
     * bigint equality operator: just use memcmp.
     * FIXME: we could easily support operands of different sizes.
     */
    inline bool operator ==(bigint<N,T> const &x) const
    {
        return memcmp(m_digits, x.m_digits, sizeof(m_digits)) == 0;
    }

    inline bool operator !=(bigint<N,T> const &x) const
    {
        return !(*this == x);
    }

    /*
     * bigint comparison operators: take a quick decision if signs
     * differ. Otherwise, compare all digits.
     */
    bool operator >(bigint<N,T> const &x) const
    {
        if (is_negative() ^ x.is_negative())
            return x.is_negative();
        for (unsigned int i = 0; i < N; ++i)
            if (m_digits[i] != x.m_digits[i])
                return m_digits[i] > x.m_digits[i];
        return false;
    }

    bool operator <(bigint<N,T> const &x) const
    {
        if (is_negative() ^ x.is_negative())
            return is_negative();
        for (unsigned int i = 0; i < N; ++i)
            if (m_digits[i] != x.m_digits[i])
                return m_digits[i] < x.m_digits[i];
        return false;
    }

    inline bool operator >=(bigint<N,T> const &x) const
    {
        return !(*this < x);
    }

    inline bool operator <=(bigint<N,T> const &x) const
    {
        return !(*this > x);
    }

private:
    /* Allow other types of bigints to access our private members */
    template<unsigned int, typename> friend class bigint;

    inline bool is_negative() const
    {
        if (N < 1)
            return false;
        return (m_digits[N - 1] & ((T)1 << (bits_per_digit - 1))) != 0;
    }

    T m_digits[N];
};

typedef bigint<8, uint32_t> int248_t;
typedef bigint<16, uint32_t> int496_t;
typedef bigint<32, uint32_t> int992_t;

} /* namespace lol */

