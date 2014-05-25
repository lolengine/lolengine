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
    enum Value
    {
        NOTHING = 0,
        THREAD_STARTED,
        THREAD_STOPPED,

        MAX
    }
    m_value;

    inline ThreadStatus()                   : m_value(ThreadStatus::NOTHING) {}
    inline ThreadStatus(Value v)            : m_value(v) {}
    bool operator==(const ThreadStatus& v)  { return m_value == v.m_value; }
};

struct ThreadJobType
{
    enum Value
    {
        NONE = 0,
        WORK_TODO,
        WORK_DONE,
        THREAD_STOP,

        MAX
    }
    m_value;

    inline ThreadJobType()                  : m_value(ThreadJobType::MAX) {}
    inline ThreadJobType(Value v)           : m_value(v) {}
    bool operator==(const ThreadJobType& o) { return m_value == o.m_value; }
};

class ThreadJob
{
    friend class ThreadManager;

public:
    ThreadJob()                             { m_type = ThreadJobType::NONE; }
    ThreadJob(ThreadJobType type)           { m_type = type; }
    ThreadJobType GetJobType()              { return m_type; }
    void SetJobType(ThreadJobType type)     { m_type = type; }
    bool operator==(const ThreadJobType& o) { return GetJobType() == o; }
protected:
    virtual bool DoWork()                   { return false; }

    ThreadJobType m_type;
};

class ThreadManager
{
public:
    ThreadManager(int thread_count)
    {
        m_thread_count = thread_count;
    }

    //Initialize and start the thread
    bool Start()
    {
        if (m_threads.Count() > 0)
            return false;

        /* Spawn worker threads and wait for their readiness. */
        m_threads.Resize(m_thread_count);
        for (int i = 0; i < m_thread_count; i++)
            m_threads[i] = new Thread(BaseThreadWork, this);
        for (int i = 0; i < m_thread_count; i++)
            m_spawnqueue.Pop();

        return true;
    }

    bool AddWork(ThreadJob* job)
    {
        if (m_jobqueue.TryPush(job))
            return true;
        return false;
    }
    bool FetchResult(Array<ThreadJob*>& results)
    {
        ThreadJob* result;
        while (m_resultqueue.TryPop(result))
            results << result;
        return results.Count() > 0;
    }

    static void *BaseThreadWork(void* data)
    {
        ThreadManager *that = (ThreadManager *)data;
        that->m_spawnqueue.Push(ThreadStatus::THREAD_STARTED);
        for ( ; ; )
        {
            ThreadJob* job = that->m_jobqueue.Pop();
            if (job->GetJobType() == ThreadJobType::THREAD_STOP)
                break;
            else if (*job == ThreadJobType::WORK_TODO)
            {
                if (job->DoWork())
                {
                    job->SetJobType(ThreadJobType::WORK_DONE);
                    that->m_resultqueue.Push(job);
                }
            }
        }
        that->m_donequeue.Push(ThreadStatus::THREAD_STOPPED);
        return NULL;
    }

    //Stop the thread
    bool Stop()
    {
        if (m_threads.Count() <= 0)
            return false;

        /* Signal worker threads for completion and wait for
         * them to quit. */
        ThreadJob stop_job(ThreadJobType::THREAD_STOP);
        for (int i = 0; i < m_thread_count; i++)
            m_jobqueue.Push(&stop_job);
        for (int i = 0; i < m_thread_count; i++)
            m_donequeue.Pop();

        return true;
    }

protected:
    /* Worker threads */
    int                     m_thread_count;
    Array<Thread*>          m_threads;
    Queue<ThreadStatus>     m_spawnqueue, m_donequeue;
    Queue<ThreadJob*>       m_jobqueue;
    Queue<ThreadJob*>       m_resultqueue;
};
#endif

} /* namespace lol */

#endif // __LOL_SYS_THREAD_H__

