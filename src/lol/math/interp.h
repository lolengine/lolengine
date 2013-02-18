//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// Interpolator classes
// --------------------
//

#if !defined __LOL_MATH_INTERP_H__
#define __LOL_MATH_INTERP_H__

namespace lol
{

template<typename T, int N = 16> class Interp
{
public:
    inline Interp() : m_pos(-N) {}
    inline ~Interp() {}

    void Set(float key, T const &val)
    {
        if (m_pos < 0)
        {
            m_key[m_pos + N] = key;
            m_val[m_pos + N] = val;
            ++m_pos;
        }
        else
        {
            m_key[m_pos] = key;
            m_val[m_pos] = val;
            m_pos = (m_pos + 1) % N;
        }
    }

    T Get(float key)
    {
        if (m_pos == -N)
            return T();
        if (m_pos == 1 - N)
            return m_val[0];

        int start = max(0, m_pos);
        int a = 0;
        int b = min(m_pos + N, N);

        while (a + 1 < b)
        {
            int c = (a + b) / 2;
            if (m_key[(start + c) % N] >= key)
                b = c;
            else
                a = c;
        }

        float ka = m_key[(start + a) % N];
        float kb = m_key[(start + b) % N];
        float u = (key - ka) / (kb - ka);

        return (1.f - u) * m_val[(start + a) % N] + u * m_val[(start + b) % N];
    }

    inline void Reset()
    {
        m_pos = -N;
    }

private:
    float m_key[N];
    T m_val[N];

    /* If m_pos < 0, the value indicates how many free slots
     * there are. */
    int m_pos;
};

} /* namespace lol */

#endif // __LOL_MATH_INTERP_H__

