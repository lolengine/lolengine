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

#if LOL_FEATURE_THREADS
class thread : thread_base
{
public:
    thread(std::function<void(void)> fn) : thread_base(fn) {}
    virtual ~thread() {}
};

struct ThreadStatus
{
    DEF_VALUE
        ADD_VALUE(NOTHING)
        ADD_VALUE(THREAD_STARTED)
        ADD_VALUE(THREAD_STOPPED)
    END_E_VALUE

    LOL_DECLARE_ENUM_METHODS(ThreadStatus)
};

LOL_SAFE_ENUM(ThreadJobType,
    NONE,
    WORK_TODO,
    WORK_DONE,
    WORK_FAILED,
    WORK_FETCHED,
    THREAD_STOP
);

class ThreadJob
{
    friend class BaseThreadManager;

public:
    inline ThreadJob() : m_type(ThreadJobType::NONE) {}
    inline ThreadJob(ThreadJobType type) : m_type(type) {}
    virtual ~ThreadJob() {}

    ThreadJobType GetJobType()              { return m_type; }
    void SetJobType(ThreadJobType type)     { m_type = type; }
    bool operator==(const ThreadJobType& o) { return GetJobType() == o; }
protected:
    virtual bool DoWork()                   { return false; }

    ThreadJobType m_type;
};

//Base class for thread manager
class BaseThreadManager : public Entity
{
public:
    BaseThreadManager(int thread_count);
    ~BaseThreadManager();

    char const *GetName() { return "<BaseThreadManager>"; }

    //Initialize, Ticker::Ref and start the thread
    bool Start();
    //Stop the threads
    bool Stop();
protected:
    //Work stuff
    bool AddWork(ThreadJob* job);

    //Fetch Results
    bool FetchResult(array<ThreadJob*>& results);
    //Base thread work function
    void BaseThreadWork();

    virtual void TickGame(float seconds);
    //Default behaviour : delete the job result
    virtual void TreatResult(ThreadJob* result) { delete(result); }

    /* Worker threads */
    int                 m_thread_count;
    array<thread*>      m_threads;
    queue<ThreadStatus> m_spawnqueue, m_donequeue;
    queue<ThreadJob*>   m_jobqueue;
    queue<ThreadJob*>   m_resultqueue;
    array<ThreadJob*>   m_job_dispatch;
};

//Generic class for thread manager, executes work and store results, for you to use
class GenericThreadManager : public BaseThreadManager
{
public:
    GenericThreadManager(int thread_count)
        : BaseThreadManager(thread_count) { }

    char const *GetName() { return "<GenericThreadManager>"; }

    //Work stuff
    bool AddJob(ThreadJob* job) { return AddWork(job); }
    bool GetWorkResult(array<ThreadJob*>& results)
    {
        results += m_job_result;
        m_job_result.Empty();
        return results.Count() > 0;
    }

protected:
    virtual void TreatResult(ThreadJob* result) { m_job_result << result; }

    array<ThreadJob*>       m_job_result;
};
#endif

} /* namespace lol */

