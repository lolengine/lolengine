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
    DefaultThreadManager(int thread_count)
        : BaseThreadManager(thread_count, thread_count)
    { }
    DefaultThreadManager(int thread_count, int thread_min)
        : BaseThreadManager(thread_count, thread_min)
    { }

    char const *GetName() { return "<DefaultThreadManager>"; }

    //Work stuff
    bool AddJob(ThreadJob* job);
    bool GetWorkResult(array<ThreadJob*>& results);

protected:
    virtual void TreatResult(ThreadJob* result) { m_job_result << result; }
    array<ThreadJob*> m_job_result;
};

//FileUpdateTesterJob ---------------------------------------------------------
class FileUpdateTesterJob : public ThreadJob
{
public:
    FileUpdateTesterJob()
        : ThreadJob(ThreadJobType::NONE) { }
    FileUpdateTesterJob(String path)
        : ThreadJob(ThreadJobType::WORK_TODO)
    { m_path = path; }
    String& GetPath()   { return m_path; }
    bool HasUpdated()   { return m_updated; }
    void Restart()
    {
        SetJobType(ThreadJobType::WORK_TODO);
        m_updated = false;
    }

protected:
    virtual bool DoWork();

    //-----------------
    bool        m_ready = false;
    String      m_path = String();
    long int    m_time = 0;
    bool        m_updated = false;
};

//Test the files registered and warns when they update ------------------------
class FileUpdateTester : public BaseThreadManager
{
    typedef BaseThreadManager super;
public:
    struct Status
    {
        bool m_updated = false;
    };
public:
    FileUpdateTester() : BaseThreadManager(1) { }
    ~FileUpdateTester() { }
    char const *GetName() { return "<FileUpdateTester>"; }

    //-------------------------------------------------------------------------
    FileUpdateTester::Status* RegisterFile(String const& path);
    virtual void TickGame(float seconds);
    virtual void TreatResult(ThreadJob* result);

private:
    array<ThreadJob*>       m_job_done;
    map<String, Status*>    m_files;
};

} /* namespace lol */

