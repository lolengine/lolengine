//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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

#include <map>

namespace lol
{

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
    virtual bool BuildEnumMap(std::map<int64_t, String>& enum_map)
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
    virtual bool BuildEnumMap(std::map<int64_t, String>& enum_map)
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
    timer               m_thread_added_timer;
    int                 m_thread_added = 0;
    timer               m_thread_removed_timer;
    int                 m_thread_removed = 0;

    queue<ThreadStatus> m_statusqueue;
    queue<thread*>      m_spawnqueue, m_donequeue;
#endif //LOL_FEATURE_THREADS
    queue<ThreadJob*>   m_jobqueue;
    queue<ThreadJob*>   m_resultqueue;
};

//Generic class for thread manager, executes work and store results, with no specific treatment
class DefaultThreadManager : public BaseThreadManager
{
public:
    char const *GetName() { return "<DefaultThreadManager>"; }
    DefaultThreadManager(int thread_max)
        : BaseThreadManager(thread_max, thread_max)
    { }
    DefaultThreadManager(int thread_max, int thread_min)
        : BaseThreadManager(thread_max, thread_min)
    { }

    //Work stuff
    void AddJob(ThreadJob* job);
    bool GetWorkResult(array<ThreadJob*>& results);

protected:
    virtual void TreatResult(ThreadJob* result) { m_job_result << result; }
    array<ThreadJob*> m_job_result;
};

//FileUpdateTester ------------------------------------------------------------
//Test the files registered and warns when they update
class FileUpdateTester : public BaseThreadManager
{
    typedef BaseThreadManager super;
public:
    class Status
    {
        friend class FileUpdateTester;
    protected:
        mutex m_mutex;
        long int m_time = 0;
        bool m_updated = false;
    public:
        void SetTime(long int time)
        {
            m_mutex.lock();
            m_time = time;
            m_mutex.unlock();
        }
        long int GetTime()
        {
            m_mutex.lock();
            long int time = m_time;
            m_mutex.unlock();
            return time;
        }
        void SetUpdated(bool updated)
        {
            m_mutex.lock();
            m_updated = updated;
            m_mutex.unlock();
        }
        bool HasUpdated()
        {
            m_mutex.lock();
            bool updated = m_updated;
            m_mutex.unlock();
            return updated;
        }
    };
public:
    char const *GetName() { return "<FileUpdateTester>"; }
    FileUpdateTester(uint32_t frame_skip = 4)
        : BaseThreadManager(1)
    { m_frame_skip = frame_skip; }
    virtual ~FileUpdateTester();

    //-------------------------------------------------------------------------
    FileUpdateTester::Status* RegisterFile(String const& path);
protected:
    void UnregisterFile(String const& path);
public:
    void UnregisterFile(FileUpdateTester::Status*& status);
    //TODO: Add directories
    //FileUpdateTester::Status* RegisterDirectory(String const& path, bool check_files=false);
    virtual void TickGame(float seconds);
    virtual void TreatResult(ThreadJob* result);

private:
    uint32_t m_frame_skip = 4;
    uint32_t m_frame_count = 0;
    array<ThreadJob*> m_job_done;
    std::map<String, Status*> m_files;
};
typedef FileUpdateTester::Status FileUpdateStatus;

//AsyncImageLoader ------------------------------------------------------------
//Load images asynchronously, automatically updating the dummy image
class AsyncImageLoader : public BaseThreadManager
{
public:
    char const *GetName() { return "<AsyncImageLoader>"; }
    AsyncImageLoader(int thread_max)
        : BaseThreadManager(thread_max, 0)
    {
        m_dummy_image.DummyFill();
    }
    virtual ~AsyncImageLoader()
    { }

    //Returns a dummy image, and start a job to load the image on a thread
    image* Load(const lol::String& path);
    bool CheckStatus(image* img);

protected:
    virtual void TreatResult(ThreadJob* result);

private:
    image m_dummy_image;
    std::map<String, image*> m_images;
    array<image*> m_loaded_images;
};

} /* namespace lol */

