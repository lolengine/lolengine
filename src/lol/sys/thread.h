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
        WORK_FAILED,
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

class ThreadManager : public Entity
{
public:
    ThreadManager(int thread_count);
    ~ThreadManager();

    //Initialize, Ticker::Ref and start the thread
    bool Start();
    //Stop the threads
    bool Stop();
    //Work stuff
    bool AddWork(ThreadJob* job);
    bool FetchResult(Array<ThreadJob*>& results);
    //Base thread work function
    static void *BaseThreadWork(void* data);
    virtual void TickGame(float seconds);
    virtual void TreatResult(ThreadJob* result) { }

    char const *GetName() { return "<ThreadManager>"; }

protected:
    /* Worker threads */
    int                     m_thread_count;
    Array<Thread*>          m_threads;
    Queue<ThreadStatus>     m_spawnqueue, m_donequeue;
    Queue<ThreadJob*>       m_jobqueue;
    Queue<ThreadJob*>       m_resultqueue;
    Array<ThreadJob*>       m_job_dispatch;
};
#endif

} /* namespace lol */

#endif // __LOL_SYS_THREAD_H__

