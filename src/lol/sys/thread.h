//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
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
// ---------------------
//

#include "engine/entity.h"

#include <functional>

#if LOL_FEATURE_THREADS
#   include <thread>
#   include <mutex>
#   include <condition_variable>
#else
/* Nothing */
#endif

/* XXX: workaround for a bug in Visual Studio 2012 and 2013!
 * https://connect.microsoft.com/VisualStudio/feedback/details/747145 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#   define LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND 1
#   include <windows.h>
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

/* XXX: workaround for missing std::thread in mingw */
#if _GLIBCXX_MUTEX && !_GLIBCXX_HAS_GTHREADS && _WIN32
#   include "mingw.thread.h"
#   include "mingw.mutex.h"
#   include "mingw.condition_variable.h"
#   undef near /* Fuck Microsoft */
#   undef far /* Fuck Microsoft again */
#endif

namespace lol
{

class mutex
{
public:
    // Will block the thread if another has already locked
    void lock()
    {
#if LOL_FEATURE_THREADS
        m_mutex.lock();
#endif
    }

    // Will not block if another thread has already locked
    bool try_lock()
    {
#if LOL_FEATURE_THREADS
        return m_mutex.try_lock();
#else
        return false;
#endif
    }

    void unlock()
    {
#if LOL_FEATURE_THREADS
        m_mutex.unlock();
#endif
    }

private:
#if LOL_FEATURE_THREADS
    std::mutex m_mutex;
#endif
};

// A FIFO queue for threads
template<typename T, int N = 128>
class queue
{
public:
    queue()
      : m_start(0),
        m_count(0)
    {
    }

    // Will block the thread if another has already locked
    void push(T value)
    {
#if LOL_FEATURE_THREADS
        /* Wait for the mutex availability or non-fullness */
        std::unique_lock<std::mutex> uni_lock(m_mutex);
        m_full_cond.wait(uni_lock, [&]{ return m_count < CAPACITY; });
#endif

        do_push(value); /* Push value */

#if LOL_FEATURE_THREADS
        /* Release lock and notify empty condition var (in that order) */
        uni_lock.unlock();
        m_empty_cond.notify_one();
#endif
    }

    // Will not block if another has already locked
    bool try_push(T value)
    {
#if LOL_FEATURE_THREADS
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
#else
        if (m_count == CAPACITY)
            return false;
#endif

        do_push(value); /* Push value */

#if LOL_FEATURE_THREADS
        /* Release lock and notify empty condition var (in that order) */
        uni_lock.unlock();
        m_empty_cond.notify_one();
#endif

        return true;
    }

    // Will block the thread if another has already locked
    T pop()
    {
#if LOL_FEATURE_THREADS
        /* Wait for the mutex availability or non-emptiness */
        std::unique_lock<std::mutex> uni_lock(m_mutex);
        m_empty_cond.wait(uni_lock, [&]{return m_count > 0; });
#else
        ASSERT(0, "Pop should only be used with threads. Use try_pop instead.");
#endif

        T ret = do_pop(); /* Pop value */

#if LOL_FEATURE_THREADS
        /* Release lock and notify full condition var (in that order) */
        uni_lock.unlock();
        m_full_cond.notify_one();
#endif

        return ret;
    }

    // Will not block if another has already locked
    bool try_pop(T &ret)
    {
#if LOL_FEATURE_THREADS
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
#else
        if (m_count == 0)
            return false;
#endif

        ret = do_pop(); /* Pop value */

#if LOL_FEATURE_THREADS
        /* Release lock and notify full condition var (in that order) */
        uni_lock.unlock();
        m_full_cond.notify_one();
#endif

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
    int m_start, m_count;
#if LOL_FEATURE_THREADS
    std::mutex m_mutex;
    std::condition_variable m_empty_cond, m_full_cond;
#endif
};

// Base class for threads
class thread
{
public:
    thread(std::function<void(thread*)> fn)
      : m_function(fn)
    {
#if LOL_FEATURE_THREADS
        m_thread = std::thread(trampoline, this);
#endif
    }

    ~thread()
    {
#if LOL_FEATURE_THREADS
#   if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        m_thread.detach();
#   else
        m_thread.join();
#   endif
#endif
    }

private:
#if LOL_FEATURE_THREADS
    static void trampoline(thread *that)
    {
        that->m_function(that);
#   if LOL_VISUAL_STUDIO_BUG_747145_WORKAROUND
        ExitThread(0);
#   endif
    }
#endif

#if LOL_FEATURE_THREADS
    std::thread m_thread;
#endif
    std::function<void(thread*)> m_function;
};

//ThreadStatus ----------------------------------------------------------------
struct ThreadStatusBase : public StructSafeEnum
{
    enum Type
    {
        NOTHING,
        THREAD_STARTED,
        THREAD_WORKING,
        THREAD_IDLE,
        THREAD_STOPPED,
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[NOTHING] = "NOTHING";
        enum_map[THREAD_STARTED] = "THREAD_STARTED";
        enum_map[THREAD_STOPPED] = "THREAD_STOPPED";
        return true;
    }
};
typedef SafeEnum<ThreadStatusBase> ThreadStatus;

struct ThreadJobTypeBase : public StructSafeEnum
{
    enum Type
    {
        NONE,
        WORK_TODO,
        WORK_SUCCEEDED,
        WORK_FAILED,
        WORK_DONE,
        THREAD_STOP
    };
protected:
    virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
    {
        enum_map[NONE]          = "NONE";
        enum_map[WORK_TODO]     = "WORK_TODO";
        enum_map[WORK_SUCCEEDED] = "WORK_SUCCEEDED";
        enum_map[WORK_FAILED]   = "WORK_FAILED";
        enum_map[WORK_DONE]  = "WORK_DONE";
        enum_map[THREAD_STOP]   = "THREAD_STOP";
        return true;
    }
};
typedef SafeEnum<ThreadJobTypeBase> ThreadJobType;

//ThreadJob -------------------------------------------------------------------
class ThreadJob
{
    friend class BaseThreadManager;

protected:
    inline ThreadJob(ThreadJobType type) : m_type(type) {}
public:
    char const *GetName() { return "<ThreadJob>"; }
    inline ThreadJob() : m_type(ThreadJobType::NONE) {}
    virtual ~ThreadJob() {}

    ThreadJobType GetJobType()              { return m_type; }
    void SetJobType(ThreadJobType type)     { m_type = type; }
    bool operator==(const ThreadJobType& o) { return GetJobType() == o; }
protected:
    virtual bool DoWork()                   { return false; }

    ThreadJobType m_type;
};

//Base class for thread manager -----------------------------------------------
class BaseThreadManager : public Entity
{
public:
    char const *GetName() { return "<BaseThreadManager>"; }
    BaseThreadManager(int thread_max);
    BaseThreadManager(int thread_max, int thread_min);
    virtual ~BaseThreadManager();

    //Base setup
    void Setup(int thread_max);
    void Setup(int thread_max, int thread_min);

    //Initialize, Ticker::Ref and start the thread
    bool Start();
    //Stop the threads
    bool Stop();

private:
    //Work stuff
    bool AddThreadWork(ThreadJob* job);

protected:
    //Thread count management
    void AdjustThreadCount(int count);
    void CleanAddedThread(bool wait = false);
    void CleanRemovedThread(bool wait = false);

    int GetDispatchCount();
    int GetDispatchedCount();

    //Dispatch job to threads
    void DispatchJob(ThreadJob* job);
    void DispatchJob(array<ThreadJob*> const& jobs);
    //Fetch Results
    bool FetchResult(array<ThreadJob*>& results);
    //Base thread work function
    void BaseThreadWork(thread* inst);

    virtual void TickGame(float seconds);
    //Default behaviour : delete the job result
    virtual void TreatResult(ThreadJob* result) { delete(result); }

private:
    /* Jobs */
    array<ThreadJob*>   m_job_dispatch;
    int                 m_job_dispatched = 0;

#if LOL_FEATURE_THREADS
    /* Worker threads */
    int                 m_thread_max = 0;
    int                 m_thread_min = 0;
    int                 m_thread_active = 0;
    array<thread*>      m_threads;
    Timer               m_thread_added_timer;
    int                 m_thread_added = 0;
    Timer               m_thread_removed_timer;
    int                 m_thread_removed = 0;

    queue<ThreadStatus> m_statusqueue;
    queue<thread*>      m_spawnqueue, m_donequeue;
#endif //LOL_FEATURE_THREADS
    queue<ThreadJob*>   m_jobqueue;
    queue<ThreadJob*>   m_resultqueue;
};

} /* namespace lol */

