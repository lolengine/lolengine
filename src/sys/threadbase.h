//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The base classes for multithreading
// -----------------------------------
//

#if defined HAVE_PTHREAD_H
#   include <pthread.h>
#elif defined _XBOX
#   include <xtl.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#elif defined _WIN32
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#else
#   error No threading support yet :(
#endif

namespace lol
{

class mutex_base
{
public:
    mutex_base()
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_init(&m_mutex, nullptr);
#elif defined _WIN32
        InitializeCriticalSection(&m_mutex);
#endif
    }

    ~mutex_base()
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_destroy(&m_mutex);
#elif defined _WIN32
        DeleteCriticalSection(&m_mutex);
#endif
    }

    void lock()
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
#elif defined _WIN32
        EnterCriticalSection(&m_mutex);
#endif
    }

    void unlock()
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_unlock(&m_mutex);
#elif defined _WIN32
        LeaveCriticalSection(&m_mutex);
#endif
    }

private:
#if defined HAVE_PTHREAD_H
    pthread_mutex_t m_mutex;
#elif defined _WIN32
    CRITICAL_SECTION m_mutex;
#endif
};

template<typename T, int N>
class queue_base
{
public:
    queue_base()
    {
        m_start = m_count = 0;
#if defined HAVE_PTHREAD_H
        m_poppers = m_pushers = 0;
        pthread_mutex_init(&m_mutex, nullptr);
        pthread_cond_init(&m_empty_cond, nullptr);
        pthread_cond_init(&m_full_cond, nullptr);
#elif defined _WIN32
        m_empty_sem = CreateSemaphore(nullptr, CAPACITY, CAPACITY, nullptr);
        m_full_sem = CreateSemaphore(nullptr, 0, CAPACITY, nullptr);
        InitializeCriticalSection(&m_mutex);
#endif
    }

    ~queue_base()
    {
#if defined HAVE_PTHREAD_H
        pthread_cond_destroy(&m_empty_cond);
        pthread_cond_destroy(&m_full_cond);
        pthread_mutex_destroy(&m_mutex);
#elif defined _WIN32
        CloseHandle(m_empty_sem);
        CloseHandle(m_full_sem);
        DeleteCriticalSection(&m_mutex);
#endif
    }

    void push(T value)
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        /* If queue is full, wait on the "full" cond var. */
        m_pushers++;
        while (m_count == CAPACITY)
            pthread_cond_wait(&m_full_cond, &m_mutex);
        m_pushers--;
#elif defined _WIN32
        WaitForSingleObject(m_empty_sem, INFINITE);
        EnterCriticalSection(&m_mutex);
#endif

        /* Push value */
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;

#if defined HAVE_PTHREAD_H
        /* If there were poppers waiting, signal the "empty" cond var. */
        if (m_poppers)
            pthread_cond_signal(&m_empty_cond);
        pthread_mutex_unlock(&m_mutex);
#elif defined _WIN32
        LeaveCriticalSection(&m_mutex);
        ReleaseSemaphore(m_full_sem, 1, nullptr);
#endif
    }

    bool try_push(T value)
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        /* If queue is full, wait on the "full" cond var. */
        if (m_count == CAPACITY)
        {
            pthread_mutex_unlock(&m_mutex);
            return false;
        }
#elif defined _WIN32
        DWORD status = WaitForSingleObject(m_empty_sem, 0);
        if (status == WAIT_TIMEOUT)
            return false;
        EnterCriticalSection(&m_mutex);
#endif

        /* Push value */
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;

#if defined HAVE_PTHREAD_H
        /* If there were poppers waiting, signal the "empty" cond var. */
        if (m_poppers)
            pthread_cond_signal(&m_empty_cond);
        pthread_mutex_unlock(&m_mutex);
#elif defined _WIN32
        LeaveCriticalSection(&m_mutex);
        ReleaseSemaphore(m_full_sem, 1, nullptr);
#endif

        return true;
    }

    T pop()
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        /* Wait until there is something in the queue. Be careful, we
         * could get woken up but another thread may have eaten the
         * message in the meantime. */
        m_poppers++;
        while (m_count == 0)
            pthread_cond_wait(&m_empty_cond, &m_mutex);
        m_poppers--;
#elif defined _WIN32
        WaitForSingleObject(m_full_sem, INFINITE);
        EnterCriticalSection(&m_mutex);
#endif

        /* Pop value */
        T ret = m_values[m_start];
        m_start = (m_start + 1) % CAPACITY;
        m_count--;

#if defined HAVE_PTHREAD_H
        /* If there were pushers waiting, signal the "full" cond var. */
        if (m_pushers)
            pthread_cond_signal(&m_full_cond);
        pthread_mutex_unlock(&m_mutex);
#else
        LeaveCriticalSection(&m_mutex);
        ReleaseSemaphore(m_empty_sem, 1, nullptr);
#endif

        return ret;
    }

    bool try_pop(T &ret)
    {
#if defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        if (m_count == 0)
        {
            pthread_mutex_unlock(&m_mutex);
            return false;
        }
#elif defined _WIN32
        DWORD status = WaitForSingleObject(m_full_sem, 0);
        if (status == WAIT_TIMEOUT)
            return false;
        EnterCriticalSection(&m_mutex);
#endif

        /* Pop value */
        ret = m_values[m_start];
        m_start = (m_start + 1) % CAPACITY;
        m_count--;

#if defined HAVE_PTHREAD_H
        /* If there were pushers waiting, signal the "full" cond var. */
        if (m_pushers)
            pthread_cond_signal(&m_full_cond);
        pthread_mutex_unlock(&m_mutex);
#else
        LeaveCriticalSection(&m_mutex);
        ReleaseSemaphore(m_empty_sem, 1, nullptr);
#endif

        return true;
    }

private:
    static size_t const CAPACITY = N;
    T m_values[CAPACITY];
    size_t m_start, m_count;
#if defined HAVE_PTHREAD_H
    size_t m_poppers, m_pushers;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_empty_cond, m_full_cond;
#elif defined _WIN32
    HANDLE m_empty_sem, m_full_sem;
    CRITICAL_SECTION m_mutex;
#endif
};

class thread_base
{
public:
    thread_base(std::function<void(void)>)
    {
#if defined HAVE_PTHREAD_H
        /* Set the joinable attribute for systems who don't play nice */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&m_thread, &attr, trampoline, this);
#elif defined _WIN32
        m_thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)trampoline,
                                this, 0, &m_tid);
#endif
    }

    virtual ~thread_base()
    {
#if defined HAVE_PTHREAD_H
        pthread_join(m_thread, nullptr);
#elif defined _WIN32
        WaitForSingleObject(m_thread, INFINITE);
#endif
    }

private:
    static void *trampoline(void *data)
    {
        thread_base *that = (thread_base *)data;
        that->m_function();
        return nullptr;
    }

    std::function<void(void)> m_function;

#if defined HAVE_PTHREAD_H
    pthread_t m_thread;
#elif defined _WIN32
    HANDLE m_thread;
    DWORD m_tid;
#endif
};

} /* namespace lol */

