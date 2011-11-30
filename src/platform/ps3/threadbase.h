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

class ConditionBase
{
public:
    ConditionBase()
    {
    }

    ~ConditionBase()
    {
    }

    void Acquire()
    {
    }

    void Release()
    {
    }

    void Wait()
    {
    }

    void Notify()
    {
    }

private:
};

class ThreadBase
{
public:
    ThreadBase(void *(*fn)(void *), void *data)
    {
        /* FIXME: choose priority more wisely */
        sys_ppu_thread_create(&m_thread, (void (*)(uint64_t))fn,
                              (uint64_t)data, 1000, 0, 0, "new thread");
    }

    virtual ~ThreadBase()
    {
        sys_ppu_thread_join(m_thread, NULL);
    }

private:
    sys_ppu_thread_t m_thread;
};

} /* namespace lol */

#endif // __LOL_THREADBASE_H__

