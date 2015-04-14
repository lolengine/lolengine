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

#include <lol/engine-internal.h>

namespace lol
{

//BaseThreadManager -----------------------------------------------------------
BaseThreadManager::BaseThreadManager(int thread_max) : BaseThreadManager(thread_max, thread_max)
{ }

BaseThreadManager::BaseThreadManager(int thread_max, int thread_min)
{
    Setup(thread_max, thread_min);
}

BaseThreadManager::~BaseThreadManager()
{
    Stop();
}

//Base Setup ------------------------------------------------------------------
void BaseThreadManager::Setup(int thread_max)
{
    Setup(thread_max, thread_max);
}
void BaseThreadManager::Setup(int thread_max, int thread_min)
{
#if LOL_FEATURE_THREADS
    m_thread_max = thread_max;
    m_thread_min = thread_min;
#endif //LOL_FEATURE_THREADS
}

//Initialize, Ticker::Ref and start the thread --------------------------------
bool BaseThreadManager::Start()
{
#if LOL_FEATURE_THREADS
    ASSERT(!!m_thread_max, "Thread count shouldn't be zero");

    if (m_threads.count() > 0)
        return false;

    //Add minimum threads
    m_threads.reserve(m_thread_max);
    //AddThreads(m_thread_min);
    AdjustThreadCount(m_thread_min);
#endif //LOL_FEATURE_THREADS

    return true;
}

//Stop the threads ------------------------------------------------------------
bool BaseThreadManager::Stop()
{
#if LOL_FEATURE_THREADS
    if (m_threads.count() <= 0)
        return false;

    //End all threads
    //RemoveThreads((int)m_threads.count());
    AdjustThreadCount(0);
    CleanAddedThread(true);
    CleanRemovedThread(true);
#endif //LOL_FEATURE_THREADS

    return true;
}

//Work stuff ------------------------------------------------------------------
bool BaseThreadManager::AddThreadWork(ThreadJob* job)
{
    return m_jobqueue.try_push(job);
}

//-----------------------------------------------------------------------------
void BaseThreadManager::AdjustThreadCount(int count)
{
#if LOL_FEATURE_THREADS
    int actual_count = (int)m_threads.count() - m_thread_removed;
    int diff = count - actual_count;

    for (int i = 0; i < lol::abs(diff); i++)
    {
        if (diff > 0)
        {
            //Spawn worker threads and ...
            m_threads << new thread(std::bind(&BaseThreadManager::BaseThreadWork, this, std::placeholders::_1));
            m_thread_added++;
            m_thread_added_timer.Reset();
        }
        else
        {
            //Signal worker threads for completion and ...
            m_jobqueue.push(new ThreadJob(ThreadJobType::THREAD_STOP));
            m_thread_removed++;
            m_thread_removed_timer.Reset();
        }
    }
#endif //LOL_FEATURE_THREADS
}

//-----------------------------------------------------------------------------
void BaseThreadManager::CleanAddedThread(bool wait)
{
#if LOL_FEATURE_THREADS
    //... Wait for their readiness.
    while (m_thread_added > 0)
    {
        thread* inst = nullptr;
        bool found = false;
        if (wait)
            found = !!(inst = m_spawnqueue.pop());
        else
            found = m_spawnqueue.try_pop(inst);
        if (found)
            m_thread_added--;
        else
            break;
    }
    //Assert if spawning took too much time
    //ASSERT(!(m_thread_added > 0 && m_thread_added_timer.Poll() > 5.f));
#endif //LOL_FEATURE_THREADS
}

//-----------------------------------------------------------------------------
void BaseThreadManager::CleanRemovedThread(bool wait)
{
#if LOL_FEATURE_THREADS
    //... Wait for them to quit.
    while (m_thread_removed > 0)
    {
        thread* inst = nullptr;
        bool found = false;
        if (wait)
            found = !!(inst = m_donequeue.pop());
        else
            found = m_donequeue.try_pop(inst);
        if (found)
        {
            m_thread_removed--;
            m_threads.remove_swap_item(inst);
            delete inst;
        }
        else
            break;
    }
    //Assert if stopping took too much time
    //ASSERT(!(m_thread_removed > 0 && m_thread_removed_timer.Poll() > 5.f));
#endif //LOL_FEATURE_THREADS
}

//-----------------------------------------------------------------------------
int BaseThreadManager::GetDispatchCount()
{
    return (int)m_job_dispatch.count();
}
int BaseThreadManager::GetDispatchedCount()
{
    return m_job_dispatched;
}

//-----------------------------------------------------------------------------
void BaseThreadManager::DispatchJob(ThreadJob* job)
{
    if (job) m_job_dispatch << job;
}
void BaseThreadManager::DispatchJob(array<ThreadJob*> const& jobs)
{
    m_job_dispatch += jobs;
}

//-----------------------------------------------------------------------------
bool BaseThreadManager::FetchResult(array<ThreadJob*>& results)
{
    ThreadJob* result;
    while (m_resultqueue.try_pop(result))
        results << result;
    return results.count() > 0;
}

//Base thread work function ---------------------------------------------------
void BaseThreadManager::BaseThreadWork(thread* inst)
{
    ThreadJob* job = nullptr;
#if LOL_FEATURE_THREADS
    //Register that the thread has started
    m_statusqueue.push(ThreadStatus::THREAD_STARTED);
    m_spawnqueue.push(inst);
    for (;;)
#else //LOL_FEATURE_THREADS
    while (m_jobqueue.try_pop(job))
#endif
    {
#if LOL_FEATURE_THREADS
        //Retrieve a job
        job = m_jobqueue.pop();
        //Stop thread
        if (job->GetJobType() == ThreadJobType::THREAD_STOP)
        {
            delete job;
            break;
        }
        //Or work
        else
#endif //LOL_FEATURE_THREADS
        if (*job == ThreadJobType::WORK_TODO)
        {
#if LOL_FEATURE_THREADS
            m_statusqueue.push(ThreadStatus::THREAD_WORKING);
#endif //LOL_FEATURE_THREADS
            if (job->DoWork())
                job->SetJobType(ThreadJobType::WORK_SUCCEEDED);
            else
                job->SetJobType(ThreadJobType::WORK_FAILED);
            m_resultqueue.push(job);
#if LOL_FEATURE_THREADS
            m_statusqueue.push(ThreadStatus::THREAD_IDLE);
#endif //LOL_FEATURE_THREADS
        }
    }
#if LOL_FEATURE_THREADS
    //Register that the thread has stopped
    m_statusqueue.push(ThreadStatus::THREAD_STOPPED);
    m_donequeue.push(inst);
#endif //LOL_FEATURE_THREADS
}

//-----------------------------------------------------------------------------
void BaseThreadManager::TickGame(float seconds)
{
    Entity::TickGame(seconds);

    //Start if needed
    Start();

    //Dispatch work task
    while (m_job_dispatch.count() > 0 && AddThreadWork(m_job_dispatch[0]))
    {
        m_job_dispatch.remove(0);
        //Keep track of added jobs
        m_job_dispatched++;
    }

    //Execute one task per frame if thread are not available
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
    BaseThreadWork(nullptr);
#endif // !LOL_FEATURE_THREADS

    array<ThreadJob*> result;
    //Fetch and treat results
    if (FetchResult(result))
    {
        for (int i = 0; i < result.count(); i++)
        {
            ThreadJob* job = result[i];
            TreatResult(job);
            //Remove job from count as it has been treated
            m_job_dispatched--;
        }
    }

    //Resize thread count if needed
#if LOL_FEATURE_THREADS
    ThreadStatus status;
    while (m_statusqueue.try_pop(status))
        m_thread_active += status == ThreadStatus::THREAD_IDLE ? -1 : +1;

    AdjustThreadCount(lol::clamp(m_job_dispatched, m_thread_min, m_thread_max));
    CleanAddedThread();
    CleanRemovedThread();
#endif //LOL_FEATURE_THREADS
}

} /* namespace lol */
