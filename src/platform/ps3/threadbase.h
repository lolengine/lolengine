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
// The ThreadBase class
// --------------------
//

#if !defined __LOL_THREADBASE_H__
#define __LOL_THREADBASE_H__

#include <sys/ppu_thread.h>
#include <sys/event.h>

namespace lol
{

class MutexBase
{
public:
    MutexBase()
    {
        sys_lwmutex_attribute_t attr;
        sys_lwmutex_attribute_initialize(attr);
        sys_lwmutex_create(&m_mutex, &attr);
    }

    ~MutexBase()
    {
        while (sys_lwmutex_destroy(&m_mutex) == EBUSY)
            ;
    }

    void Lock()
    {
        sys_lwmutex_lock(&m_mutex, 0);
    }

    void Unlock()
    {
        sys_lwmutex_unlock(&m_mutex);
    }

private:
    sys_lwmutex_t m_mutex;
};

template<typename T, int N> class QueueBase
{
public:
    QueueBase()
    {
        m_start = m_count = 0;
        m_poppers = m_pushers = 0;

        sys_lwmutex_attribute_t mattr;
        sys_lwmutex_attribute_initialize(mattr);
        sys_lwmutex_create(&m_mutex, &mattr);

        sys_lwcond_attribute_t cattr;
        sys_lwcond_attribute_initialize(cattr);
        sys_lwcond_create(&m_empty_cond, &m_mutex, &cattr);
        sys_lwcond_create(&m_full_cond, &m_mutex, &cattr);
    }

    ~QueueBase()
    {
        while (sys_lwcond_destroy(&m_empty_cond) == EBUSY)
            ;
        while (sys_lwcond_destroy(&m_full_cond) == EBUSY)
            ;
        while (sys_lwmutex_destroy(&m_mutex) == EBUSY)
            ;
    }

    void Push(T value)
    {
        /* FIXME: this is a copy of the pthread implementation, but we
         * should really use libsync2 instead. */
        sys_lwmutex_lock(&m_mutex, 0);
        m_pushers++;
        while (m_count == CAPACITY)
            sys_lwcond_wait(&m_full_cond, 0);
        m_pushers--;
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;
        if (m_poppers)
            sys_lwcond_signal(&m_empty_cond);
        sys_lwmutex_unlock(&m_mutex);
    }

    bool TryPush(T value)
    {
        sys_lwmutex_lock(&m_mutex, 0);
        if (m_count == CAPACITY)
        {
            sys_lwmutex_unlock(&m_mutex);
            return false;
        }
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;
        if (m_poppers)
            sys_lwcond_signal(&m_empty_cond);
        sys_lwmutex_unlock(&m_mutex);
        return true;
    }

    T Pop()
    {
        sys_lwmutex_lock(&m_mutex, 0);
        m_poppers++;
        while (m_count == 0)
            sys_lwcond_wait(&m_empty_cond, 0);
        m_poppers--;
        T ret = m_values[m_start];
        m_start = (m_start + 1) % CAPACITY;
        m_count--;
        if (m_pushers)
            sys_lwcond_signal(&m_full_cond);
        sys_lwmutex_unlock(&m_mutex);
        return ret;
    }

    bool TryPop(T &ret)
    {
        sys_lwmutex_lock(&m_mutex, 0);
        if (m_count == 0)
        {
            sys_lwmutex_unlock(&m_mutex);
            return false;
        }
        ret = m_values[m_start];
        m_start = (m_start + 1) % CAPACITY;
        m_count--;
        if (m_pushers)
            sys_lwcond_signal(&m_full_cond);
        sys_lwmutex_unlock(&m_mutex);
        return true;
    }

private:
    static size_t const CAPACITY = N;
    T m_values[CAPACITY];
    size_t m_start, m_count;
    size_t m_poppers, m_pushers;
    sys_lwmutex_t m_mutex;
    sys_lwcond_t m_empty_cond, m_full_cond;
};

class ThreadBase
{
public:
    ThreadBase(void *(*fn)(void *), void *data)
    {
        size_t stack_size = 128 * 1024;
        char const *thread_name = "new thread";

        /* FIXME: choose priority more wisely */
        sys_ppu_thread_create(&m_thread, (void (*)(uint64_t))fn,
                              (uint64_t)data, 1000, stack_size, 0, thread_name);
    }

    virtual ~ThreadBase()
    {
        sys_ppu_thread_join(m_thread, nullptr);
    }

private:
    sys_ppu_thread_t m_thread;
};

} /* namespace lol */

#endif // __LOL_THREADBASE_H__

