//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Interpolator classes
// --------------------
//

namespace lol
{

template<typename T, int N = 16> class TimeInterp
{
public:
    inline TimeInterp()
      : m_precision(0.f),
        m_accum(0.f),
        m_pos(-N)
    {}

    inline ~TimeInterp() {}

    void SetPrecision(float seconds)
    {
        m_precision = seconds;
    }

    void Set(float seconds, T const &val)
    {
        m_accum += seconds;
        if (m_accum < m_precision)
            return;
        m_accum = 0.f;

        if (m_pos < 0)
        {
            if (m_pos > -N)
                seconds += m_key[m_pos + N - 1];
            m_key[m_pos + N] = seconds;
            m_val[m_pos + N] = val;
            ++m_pos;
        }
        else
        {
            if (m_pos > 0)
                seconds += m_key[m_pos - 1];
            m_key[m_pos] = seconds;
            m_val[m_pos] = val;
            m_pos = (m_pos + 1) % N;
        }
    }

    T Get(float seconds)
    {
        if (m_pos == -N)
            return T();
        if (m_pos == 1 - N)
            return m_val[0];

        seconds += m_accum;

        int start = max(0, m_pos);
        int a = 0;
        int b = min(m_pos + N - 1, N - 1);

        while (a + 1 < b)
        {
            int c = (a + b) / 2;
            if (GetTime((start + c) % N) >= seconds)
                b = c;
            else
                a = c;
        }

        float ka = GetTime((start + a) % N);
        float kb = GetTime((start + b) % N);
        float u = (seconds - ka) / (kb - ka);

        return (1.f - u) * m_val[(start + a) % N] + u * m_val[(start + b) % N];
    }

    inline void Reset()
    {
        m_pos = -N;
    }

private:
    inline float GetTime(int i)
    {
        float k = m_key[i % N];
        if (m_pos >= 0 && i >= m_pos)
            k -= m_key[N - 1];
        if (m_pos != 0)
            k -= m_key[(m_pos + N - 1) % N];
        return k;
    }

    float m_key[N];
    T m_val[N];

    float m_precision, m_accum;

    /* If m_pos < 0, the value indicates how many free slots
     * there are. */
    int m_pos;
};

} /* namespace lol */

