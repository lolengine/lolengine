//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_SYS_ATOMIC_H__
#define __LOL_SYS_ATOMIC_H__

namespace lol
{

/*
 * TODO: we should use real atomic stuff here.
 */
template<typename T> class Atomic
{
public:
    inline Atomic(T const &value) : m_value(value) {}

    operator T() const
    {
        return m_value;
    }

    inline T operator =(T const &value)
    {
        m_mutex.Lock();
        m_value = value;
        m_mutex.Unlock();
    }

    inline T operator++(int)
    {
        m_mutex.Lock();
        T ret = m_value++;
        m_mutex.Unlock();
        return ret;
    }

    inline T& operator++()
    {
        m_mutex.Lock();
        T ret = ++m_value;
        m_mutex.Unlock();
        return ret;
    }

    inline T operator--(int)
    {
        m_mutex.Lock();
        T ret = m_value--;
        m_mutex.Unlock();
        return ret;
    }

    inline T& operator--()
    {
        m_mutex.Lock();
        T ret = --m_value;
        m_mutex.Unlock();
        return ret;
    }

private:
    Mutex m_mutex; /* FIXME: this shouldn't be needed */
    T m_value;
};

} /* namespace lol */

#endif // __LOL_SYS_ATOMIC_H__

