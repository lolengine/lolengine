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

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
/* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
#   include <thread>
#   include <mutex>
#   include <condition_variable>
#elif defined HAVE_PTHREAD_H
#   include <pthread.h>
#else
#   error No threading support yet :(
#endif

/* XXX: workaround for a bug in Visual Studio 2012 and 2013!
 * https://connect.microsoft.com/VisualStudio/feedback/details/747145 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#   define LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND 1
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

namespace lol
{

class mutex_base
{
public:
    //-------------------------------------------------------------------------
    mutex_base()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Nothing */
#elif defined HAVE_PTHREAD_H
        pthread_mutex_init(&m_mutex, nullptr);
#endif
    }

    //-------------------------------------------------------------------------
    ~mutex_base()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Nothing */
#elif defined HAVE_PTHREAD_H
        pthread_mutex_destroy(&m_mutex);
#endif
    }

    //Will block the thread if another has already locked ---------------------
    void lock()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        m_mutex.lock();
#elif defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
#endif
    }

    // Will not block if another thread has already locked ---------------------
    bool try_lock()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
        return false;
#elif LOL_FEATURE_CXX11_THREADS
        return m_mutex.try_lock();
#elif defined HAVE_PTHREAD_H
        return !pthread_mutex_trylock(&m_mutex);
#endif
    }

    //-------------------------------------------------------------------------
    void unlock()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        m_mutex.unlock();
#elif defined HAVE_PTHREAD_H
        pthread_mutex_unlock(&m_mutex);
#endif
    }

    //-------------------------------------------------------------------------
private:
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
    /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
    std::mutex m_mutex;
#elif defined HAVE_PTHREAD_H
    pthread_mutex_t m_mutex;
#endif
};

//A FIFO queue for threads ----------------------------------------------------
template<typename T, int N>
class queue_base
{
public:
    queue_base()
    {
        m_start = m_count = 0;
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Nothing */
#elif defined HAVE_PTHREAD_H
        m_poppers = m_pushers = 0;
        pthread_mutex_init(&m_mutex, nullptr);
        pthread_cond_init(&m_empty_cond, nullptr);
        pthread_cond_init(&m_full_cond, nullptr);
#endif
    }

    ~queue_base()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Nothing */
#elif defined HAVE_PTHREAD_H
        pthread_cond_destroy(&m_empty_cond);
        pthread_cond_destroy(&m_full_cond);
        pthread_mutex_destroy(&m_mutex);
#endif
    }

    //Will block the thread if another has already locked ---------------------
    void push(T value)
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Wait for the mutex availability or non-fullness */
        std::unique_lock<std::mutex> uni_lock(m_mutex);
        m_full_cond.wait(uni_lock, [&]{return m_count < CAPACITY; });
#elif defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        /* If queue is full, wait on the "full" cond var. */
        m_pushers++;
        while (m_count == CAPACITY)
            pthread_cond_wait(&m_full_cond, &m_mutex);
        m_pushers--;
#endif

        do_push(value); /* Push value */

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Release lock and notify empty condition var (in that order) */
        uni_lock.unlock();
        m_empty_cond.notify_one();
#elif defined HAVE_PTHREAD_H
        /* If there were poppers waiting, signal the "empty" cond var. */
        if (m_poppers)
            pthread_cond_signal(&m_empty_cond);
        pthread_mutex_unlock(&m_mutex);
#endif
    }

    //Will not block if another has already locked ----------------------------
    bool try_push(T value)
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        if (m_count == CAPACITY)
            return false;
#elif LOL_FEATURE_CXX11_THREADS
        /* Same as Push(), except .... */
        std::unique_lock<std::mutex> uni_lock(m_mutex, std::try_to_lock);
        /* Bail on fail try_lock fail */
        if (!uni_lock.owns_lock())
            return false;
        /* Bail on max CAPACITY */
        if (m_count == CAPACITY)
        {
            uni_lock.unlock();
            return false;
        }
#elif defined HAVE_PTHREAD_H
        /* Bail on fail try_lock */
        if (pthread_mutex_trylock(&m_mutex))
            return false;
        /* Bail on max CAPACITY */
        if (m_count == CAPACITY)
        {
            pthread_mutex_unlock(&m_mutex);
            return false;
        }
#endif

        do_push(value); /* Push value */

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Release lock and notify empty condition var (in that order) */
        uni_lock.unlock();
        m_empty_cond.notify_one();
#elif defined HAVE_PTHREAD_H
        /* If there were poppers waiting, signal the "empty" cond var. */
        if (m_poppers)
            pthread_cond_signal(&m_empty_cond);
        pthread_mutex_unlock(&m_mutex);
#endif

        return true;
    }

    //Will block the thread if another has already locked ---------------------
    T pop()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        ASSERT(0, "Pop should only be used with threads. Use try_pop instead.");
#elif LOL_FEATURE_CXX11_THREADS
        /* Wait for the mutex availability or non-emptiness */
        std::unique_lock<std::mutex> uni_lock(m_mutex);
        m_empty_cond.wait(uni_lock, [&]{return m_count > 0; });
#elif defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        /* Wait until there is something in the queue. Be careful, we
         * could get woken up but another thread may have eaten the
         * message in the meantime. */
        m_poppers++;
        while (m_count == 0)
            pthread_cond_wait(&m_empty_cond, &m_mutex);
        m_poppers--;
#endif

        T ret = do_pop(); /* Pop value */

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Release lock and notify full condition var (in that order) */
        uni_lock.unlock();
        m_full_cond.notify_one();
#elif defined HAVE_PTHREAD_H
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

    //Will not block if another has already locked ----------------------------
    bool try_pop(T &ret)
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        if (m_count == 0)
            return false;
#elif LOL_FEATURE_CXX11_THREADS
        /* Same as Pop(), except .... */
        std::unique_lock<std::mutex> uni_lock(m_mutex, std::try_to_lock);
        /* Bail on fail try_lock fail */
        if (!uni_lock.owns_lock())
            return false;
        /* Bail on zero count */
        if (m_count == 0)
        {
            uni_lock.unlock();
            return false;
        }
#elif defined HAVE_PTHREAD_H
        pthread_mutex_lock(&m_mutex);
        if (m_count == 0)
        {
            pthread_mutex_unlock(&m_mutex);
            return false;
        }
#endif

        ret = do_pop(); /* Pop value */

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        /* Release lock and notify full condition var (in that order) */
        uni_lock.unlock();
        m_full_cond.notify_one();
#elif defined HAVE_PTHREAD_H
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

    //Inner methods for actual update -----------------------------------------
private:
    void do_push(T &value)
    {
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;
    }

    T& do_pop()
    {
        size_t idx = m_start;
        m_start = (m_start + 1) % CAPACITY;
        m_count--;
        return m_values[idx];
    }

    //-------------------------------------------------------------------------
private:
    static size_t const CAPACITY = N;
    T m_values[CAPACITY];
    size_t m_start, m_count;
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
    /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
    std::mutex m_mutex;
    std::condition_variable m_empty_cond, m_full_cond;
#elif defined HAVE_PTHREAD_H
    size_t m_poppers, m_pushers;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_empty_cond, m_full_cond;
#endif
};

//Base class for threads ------------------------------------------------------
class thread_base
{
public:
    thread_base(std::function<void(void)> function)
        : m_function(function)
    { }

    void Init()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
        m_thread = std::thread(trampoline, this);
#elif defined HAVE_PTHREAD_H
        /* Set the joinable attribute for systems who don't play nice */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&m_thread, &attr, trampoline, this);
#endif
    }

    virtual ~thread_base()
    {
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
        /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
#   if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        m_thread.detach();
#   else
        m_thread.join();
#   endif
#elif defined HAVE_PTHREAD_H
        pthread_join(m_thread, nullptr);
#endif
    }

private:
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
    /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
    static void trampoline(thread_base *that)
    {
        that->m_function();
#   if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        ExitThread(0);
#   endif
    }
#else
    static void *trampoline(void *data)
    {
        thread_base *that = (thread_base *)data;
        that->m_function();
        return nullptr;
    }
#endif

    std::function<void(void)> m_function;

#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
    /* Nothing */
#elif LOL_FEATURE_CXX11_THREADS
    std::thread m_thread;
#elif defined HAVE_PTHREAD_H
    pthread_t m_thread;
#endif
};

} /* namespace lol */

