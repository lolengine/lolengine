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

#if defined __linux__ || defined __native_client__
#   include <cstring>
#   include <pthread.h>
#elif defined _WIN32
#   include <windows.h>
#else
#   error No threading support yet :(
#endif

namespace lol
{

class MutexBase
{
public:
    MutexBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_init(&m_mutex, NULL);
#elif defined _WIN32
        InitializeCriticalSection(&m_mutex);
#endif
    }

    ~MutexBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_destroy(&m_mutex);
#elif defined _WIN32
        DeleteCriticalSection(&m_mutex);
#endif
    }

    void Lock()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_lock(&m_mutex);
#elif defined _WIN32
        EnterCriticalSection(&m_mutex);
#endif
    }

    void Unlock()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_unlock(&m_mutex);
#elif defined _WIN32
        LeaveCriticalSection(&m_mutex);
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    pthread_mutex_t m_mutex;
#elif defined _WIN32
    CRITICAL_SECTION m_mutex;
#endif
};

class QueueBase
{
public:
    QueueBase()
    {
#if defined __linux__ || defined __native_client__
        memset(m_values, 0, sizeof(m_values));
        m_waiting = 0;
        m_size = 0;
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_cond, NULL);
#endif
    }

    ~QueueBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_cond_destroy(&m_cond);
        pthread_mutex_destroy(&m_mutex);
#endif
    }

    void Push(int value)
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_lock(&m_mutex);
        m_values[m_size] = value;
        m_size++;
        if (m_waiting)
            pthread_cond_signal(&m_cond);
        pthread_mutex_unlock(&m_mutex);
#endif
    }

    int Pop()
    {
#if defined __linux__ || defined __native_client__
        pthread_mutex_lock(&m_mutex);
        /* Loop until there is something in the queue. Be careful, we
         * could get woken up but another thread may have eaten the
         * message in the meantime. */
        while (!m_size)
        {
            m_waiting++;
            pthread_cond_wait(&m_cond, &m_mutex);
            m_waiting--;
        }
        m_size--;
        int ret = m_values[0];
        memmove(m_values, m_values + 1, m_size * sizeof(m_values[0]));
        pthread_mutex_unlock(&m_mutex);
        return ret;
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    int m_values[100];
    size_t m_waiting, m_size;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
#endif
};

class ThreadBase
{
public:
    ThreadBase(void *(*fn)(void *), void *data)
    {
#if defined __linux__ || defined __native_client__
        /* Set the joinable attribute for systems who don't play nice */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&m_thread, &attr, fn, data);
#elif defined _WIN32
        m_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fn,
                                data, 0, &m_tid);
#endif
    }

    virtual ~ThreadBase()
    {
#if defined __linux__ || defined __native_client__
        pthread_join(m_thread, NULL);
#elif defined _WIN32
        WaitForSingleObject(m_thread, INFINITE);
#endif
    }

private:
#if defined __linux__ || defined __native_client__
    pthread_t m_thread;
#elif defined _WIN32
    HANDLE m_thread;
    DWORD m_tid;
#endif
};

} /* namespace lol */

#endif // __LOL_THREADBASE_H__

