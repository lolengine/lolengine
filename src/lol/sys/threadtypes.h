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

namespace lol
{

//Generic class for thread manager, executes work and store results, with no specific treatment
class DefaultThreadManager : public BaseThreadManager
{
public:
    char const *GetName() { return "<DefaultThreadManager>"; }
    DefaultThreadManager(int thread_count)
        : BaseThreadManager(thread_count, thread_count)
    { }
    DefaultThreadManager(int thread_count, int thread_min)
        : BaseThreadManager(thread_count, thread_min)
    { }

    //Work stuff
    bool AddJob(ThreadJob* job);
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
            return m_time;
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
    uint32_t                m_frame_skip = 4;
    uint32_t                m_frame_count = 0;
    array<ThreadJob*>       m_job_done;
    map<String, Status*>    m_files;
};
typedef FileUpdateTester::Status FileUpdateStatus;

//AsyncImageLoader ------------------------------------------------------------
//Load images asynchronously, automatically updating the dummy image
class AsyncImageLoader : public BaseThreadManager
{
public:
    char const *GetName() { return "<AsyncImageLoader>"; }
    AsyncImageLoader(int thread_count)
        : BaseThreadManager(thread_count, 0)
    {
        m_dummy_image.DummyFill();
    }
    virtual ~AsyncImageLoader()
    { }

    //Returns a dummy image, and start a job to load the image on a thread
    Image* Load(const lol::String& path);
    bool CheckStatus(Image* image);

protected:
    virtual void TreatResult(ThreadJob* result);

private:
    Image               m_dummy_image;
    map<String, Image*> m_images;
    array<Image*>       m_loaded_images;
};

} /* namespace lol */

