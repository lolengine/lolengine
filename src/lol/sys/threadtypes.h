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

//FileUpdateTester ------------------------------------------------------------
//Test the files registered and warns when they update
class FileUpdateTester : public BaseThreadManager
{
    typedef BaseThreadManager super;
public:
    struct Status
    {
        bool m_updated = false;
    };
public:
    char const *GetName() { return "<FileUpdateTester>"; }
    FileUpdateTester() : BaseThreadManager(1) { }
    ~FileUpdateTester() { }

    //-------------------------------------------------------------------------
    FileUpdateTester::Status* RegisterFile(String const& path);
    virtual void TickGame(float seconds);
    virtual void TreatResult(ThreadJob* result);

private:
    array<ThreadJob*>       m_job_done;
    map<String, Status*>    m_files;
};

//AsyncImageJob ---------------------------------------------------------------
class AsyncImageJob : public ThreadJob
{
public:
    AsyncImageJob()
        : ThreadJob(ThreadJobType::NONE)
    {
        m_path = String();
    }
    AsyncImageJob(String path)
        : ThreadJob(ThreadJobType::WORK_TODO)
    {
        m_path = path;
    }
    String const& GetPath() { return m_path; }
    Image const& GetImage() { return m_image; }

protected:
    virtual bool DoWork()
    {
        return m_image.Load(m_path.C());
    }

    String  m_path;
    Image   m_image;
};

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
    ~AsyncImageLoader()
    { }

    //Returns a dummy image, and start a job to load the image on a thread
    Image* Load(const lol::String& path)
    {
        //Create a job
        AsyncImageJob* job = new AsyncImageJob(path);
        //Create a dummy image
        Image* image = new Image(m_dummy_image);
        //Link the two
        m_images[path] = image;
        //Add job
        AddWork(job);
        //return Dummy image
        return image;
    }
    bool CheckStatus(Image* image)
    {
        if (m_loaded_images.find(image) != INDEX_NONE)
        {
            m_loaded_images.remove_item(image);
            return true;
        }
        return false;
    }

protected:
    virtual void TreatResult(ThreadJob* result)
    {
        AsyncImageJob* job = dynamic_cast<AsyncImageJob*>(result);
        ASSERT(job);
        //Copy image if work is OK
        if (job->GetJobType() != ThreadJobType::WORK_FAILED)
        {
            Image* src = m_images[job->GetPath()];
            m_images.remove(job->GetPath());
            src->Copy(job->GetImage());
            m_loaded_images.PushUnique(src);
        }
        else
        {
            Log::Error("AsyncImageJob FAILED. See load image error above.\n");
        }
        //Delete all that
        delete(result);
    }

private:
    Image               m_dummy_image;
    map<String, Image*> m_images;
    array<Image*>       m_loaded_images;
};

} /* namespace lol */

