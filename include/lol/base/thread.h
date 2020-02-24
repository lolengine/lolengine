//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Threading classes
// —————————————————————
//

#include <lol/base/features.h>
#include <lol/base/core.h>

#include <functional> // std::function
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>  // std::chrono
#include <cassert> // assert()

/* XXX: workaround for a bug in Visual Studio 2012 and 2013!
 * https://connect.microsoft.com/VisualStudio/feedback/details/747145 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#   define LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND 1
#   ifdef WIN32_LEAN_AND_MEAN
#       include <windows.h>
#   else
#       define WIN32_LEAN_AND_MEAN 1
#       include <windows.h>
#       undef WIN32_LEAN_AND_MEAN
#   endif
#endif

/* XXX: workaround for missing std::thread in mingw */
#if _GLIBCXX_MUTEX && !_GLIBCXX_HAS_GTHREADS && _WIN32
#   include "../3rdparty/mingw-std-threads/mingw.thread.h"
#   include "../3rdparty/mingw-std-threads/mingw.mutex.h"
#   include "../3rdparty/mingw-std-threads/mingw.condition_variable.h"
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

namespace lol
{

// Base class for threads
class thread
{
public:
    thread(std::function<void(thread*)> fn)
      : m_function(fn)
    {
        m_thread = std::thread(trampoline, this);
    }

    ~thread()
    {
#if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        m_thread.detach();
#else
        m_thread.join();
#endif
    }

    // FIXME: move to os::has_threads?
    static bool has_threads()
    {
        static bool const disable_threads = os::getenv("LOL_NOTHREADS").size() > 0;
#if defined __EMSCRIPTEN__ && !defined __EMSCRIPTEN_PTHREADS__
        // For some reason hardware_concurrency() will return the actual number
        // of threads/cores even though the system cannot spawn threads.
        return false;
#endif
        return !disable_threads && std::thread::hardware_concurrency() > 1;
    }

private:
    static void trampoline(thread *that)
    {
        that->m_function(that);
#if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        ExitThread(0);
#endif
    }

    std::thread m_thread;
    std::function<void(thread*)> m_function;
};

// This is like std::mutex but we can add debug information to it
class mutex
{
public:
    inline void lock() { m_mutex.lock(); }

    inline bool try_lock() { return m_mutex.try_lock(); }

    inline void unlock() { m_mutex.unlock(); }

private:
    std::mutex m_mutex;
};

// A FIFO queue for threads
template<typename T, int N = 128>
class queue
{
public:
    int size() const { return m_count; }

    // Will block the thread if another has already locked
    void push(T value)
    {
        std::unique_lock<std::mutex> uni_lock(m_mutex);

        if (thread::has_threads())
        {
            // Wait for the mutex availability or non-fullness
            m_full_cond.wait(uni_lock, [&]{ return m_count < CAPACITY; });
        }

        do_push(value); /* Push value */

        if (thread::has_threads())
        {
            // Release lock and notify empty condition var (in that order)
            uni_lock.unlock();
            m_empty_cond.notify_one();
        }
    }

    // Will not block if another has already locked
    bool try_push(T value)
    {
        std::unique_lock<std::mutex> uni_lock(m_mutex, std::defer_lock);

        if (thread::has_threads())
        {
            // Try to lock, bail out if we fail
            if (!uni_lock.try_lock())
                return false;
        }

        if (m_count == CAPACITY)
            return false; // unlocks uni_lock

        do_push(value);

        if (thread::has_threads())
        {
            // Release lock and notify empty condition var (in that order)
            uni_lock.unlock();
            m_empty_cond.notify_one();
        }

        return true;
    }

    // Will block the thread if another has already locked
    T pop()
    {
        assert(thread::has_threads());

        // Wait for the mutex availability or non-emptiness
        std::unique_lock<std::mutex> uni_lock(m_mutex);
        m_empty_cond.wait(uni_lock, [&]{return m_count > 0; });

        T ret = do_pop();

        // Release lock and notify full condition var (in that order)
        uni_lock.unlock();
        m_full_cond.notify_one();

        return ret;
    }

    // Will not block if another has already locked
    bool try_pop(T &ret)
    {
        std::unique_lock<std::mutex> uni_lock(m_mutex, std::defer_lock);

        if (thread::has_threads())
        {
            if (!uni_lock.try_lock())
                return false;
        }

        if (m_count == 0)
            return false;

        ret = do_pop(); /* Pop value */

        if (thread::has_threads())
        {
            // Release lock and notify full condition var (in that order)
            uni_lock.unlock();
            m_full_cond.notify_one();
        }

        return true;
    }

    // Inner methods for actual update
private:
    void do_push(T &value)
    {
        m_values[(m_start + m_count) % CAPACITY] = value;
        m_count++;
    }

    T& do_pop()
    {
        int idx = m_start;
        m_start = (m_start + 1) % CAPACITY;
        m_count--;
        return m_values[idx];
    }

private:
    static int const CAPACITY = N;
    T m_values[CAPACITY];
    int m_start = 0, m_count = 0;

    std::mutex m_mutex;
    std::condition_variable m_empty_cond, m_full_cond;
};

class timer
{
public:
    inline timer() { (void)get_seconds(true); }

    inline void reset() { (void)get_seconds(true); }
    inline float get() { return get_seconds(true); }
    inline float poll() { return get_seconds(false); }

    void wait(float seconds)
    {
        if (seconds > 0.0f)
        {
            float secs_elapsed = get_seconds(false);
            std::this_thread::sleep_for(std::chrono::duration<float>(seconds - secs_elapsed));
        }
    }

private:
    std::chrono::steady_clock::time_point m_tp;

    float get_seconds(bool do_reset)
    {
        auto tp = std::chrono::steady_clock::now(), tp0 = m_tp;

        if (do_reset)
            m_tp = tp;

        return std::chrono::duration_cast<std::chrono::duration<float>>(tp - tp0).count();
    }
};

} /* namespace lol */

