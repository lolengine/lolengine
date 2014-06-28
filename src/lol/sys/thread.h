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
// The Threading classes
// ---------------------
//

#if !defined __LOL_SYS_THREAD_H__
#define __LOL_SYS_THREAD_H__

#if defined __CELLOS_LV2__
#   include "platform/ps3/threadbase.h"
#else
#   include "sys/threadbase.h"
#endif

#include "entity.h"

namespace lol
{

class Mutex : public MutexBase
{
public:
    Mutex() : MutexBase() {}
};

template<typename T, int N = 128> class Queue : public QueueBase<T, N>
{
public:
    Queue() : QueueBase<T, N>() {}
};

#if LOL_FEATURE_THREADS
class Thread : ThreadBase
{
public:
    Thread(void *(*fn)(void *), void *data) : ThreadBase(fn, data) {}
    virtual ~Thread() {}
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
    static void *BaseThreadWork(void* data);
    virtual void TickGame(float seconds);
    //Default behaviour : delete the job result
    virtual void TreatResult(ThreadJob* result) { delete(result); }

    /* Worker threads */
    int                     m_thread_count;
    array<Thread*>          m_threads;
    Queue<ThreadStatus>     m_spawnqueue, m_donequeue;
    Queue<ThreadJob*>       m_jobqueue;
    Queue<ThreadJob*>       m_resultqueue;
    array<ThreadJob*>       m_job_dispatch;
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

#endif // __LOL_SYS_THREAD_H__

