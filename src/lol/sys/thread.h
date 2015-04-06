//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//
//  This library is free software. It comes without any warranty, to
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

#include "sys/threadbase.h"
#include "entity.h"

#include <functional>

namespace lol
{

class mutex : public mutex_base
{
public:
    mutex() : mutex_base() {}
};

template<typename T, int N = 128> class queue : public queue_base<T, N>
{
public:
    queue() : queue_base<T, N>() {}
};

class thread : thread_base
{
public:
    thread(std::function<void(thread*)> fn)
        : thread_base(std::bind(&thread::do_trampoline, this)),
        m_thread_function(fn)
    {
        Init();
    }
    virtual ~thread()
    { }

protected:
    static void do_trampoline(thread *that)
    {
        that->m_thread_function(that);
    }

    std::function<void(thread*)> m_thread_function;
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

