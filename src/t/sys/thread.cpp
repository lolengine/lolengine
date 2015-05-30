//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(thread_test)
{
    //FileUpdateTesterJob ---------------------------------------------------------
    class UnitTestJob : public ThreadJob
    {
        friend class UnitTestThreadManager;
    public:
        char const *GetName() { return "<UnitTestJob>"; }
        UnitTestJob() : ThreadJob(ThreadJobType::WORK_TODO)
        { }
        bool IsDone()
        {
            return m_done;
        }

    protected:
        virtual bool DoWork()
        {
            Timer timer;
            m_done = false;
            msg::info("%s: STARTED WORK\n", GetName());
            timer.Wait(2.f);
            msg::info("%s: ENDED WORK\n", GetName());
            m_done = true;
            return true;
        }
        bool m_done = false;
    };
    //Unit test thread manager
    class UnitTestThreadManager : public BaseThreadManager
    {
        typedef BaseThreadManager super;

        struct UnitTestStatusBase : public StructSafeEnum
        {
            enum Type
            {
                NOT_QUEUED,
                QUEUED,
                RETRIEVED,
                DONE,
            };
        protected:
            virtual bool BuildEnumMap(map<int64_t, String>& enum_map)
            {
                enum_map[NOT_QUEUED] = "NOT_QUEUED";
                enum_map[QUEUED] = "QUEUED";
                enum_map[RETRIEVED] = "RETRIEVED";
                enum_map[DONE] = "DONE";
                return true;
            }
        };
        typedef SafeEnum<UnitTestStatusBase> UnitTestStatus;

    public:
        char const *GetName() { return "<UnitTestThreadManager>"; }
        UnitTestThreadManager() : BaseThreadManager(4, 1)
        { }
        virtual ~UnitTestThreadManager()
        { }

        void AddJob(ThreadJob* job)
        {
            msg::info("%s DISPATCHING JOB %s\n", GetName(), job->GetName());
            DispatchJob(job);
        }
        bool GetWorkResult(array<ThreadJob*>& results)
        {
            results += m_job_result;
            m_job_result.empty();
            msg::info("%s GETWORKRESULT (%i)\n", GetName(), results.count());
            return results.count() > 0;
        }

        virtual void TickGame(float seconds)
        {
            switch (m_status.ToScalar())
            {
            case UnitTestStatus::NOT_QUEUED:
                if (!!GetDispatchCount())
                {
                    msg::info("%s TICKGAME %s\n", GetName(), m_status.ToString().C());
                    m_status = UnitTestStatus::QUEUED;
                }
                break;
            case UnitTestStatus::QUEUED:
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
                if (!GetDispatchedCount())
#else //LOL_FEATURE_THREADS
                if (GetDispatchedCount())
#endif
                {
                    msg::info("%s TICKGAME %s\n", GetName(), m_status.ToString().C());
                    m_status = UnitTestStatus::RETRIEVED;
                }
                break;
            case UnitTestStatus::RETRIEVED:
                if (m_job_result.count() == 4)
                {
                    msg::info("%s TICKGAME %s\n", GetName(), m_status.ToString().C());
                    m_status = UnitTestStatus::DONE;
                }
                break;
            default:
                break;
            }
            //Debug error fix-up
#if !LOL_BUILD_RELEASE
            m_tickstate = STATE_PRETICK_GAME;
#endif
            super::TickGame(seconds);
        }
        bool IsDone() { return m_status == UnitTestStatus::DONE; }
        int Test_GetDispatchCount() { return GetDispatchCount(); }
        int Test_GetDispatchedCount() { return GetDispatchedCount(); }

    protected:
        virtual void TreatResult(ThreadJob* result) { m_job_result << result; }
        array<ThreadJob*> m_job_result;
        UnitTestStatus m_status;
    };
    UnitTestThreadManager m_manager;

    void SetUp()
    {
    }

    void TearDown()
    {
    }

    lolunit_declare_test(threads)
    {
        msg::info("%s START\n", m_manager.GetName());
        //Start threads manager
        m_manager.Start();
        msg::info("%s STARTED\n", m_manager.GetName());

        UnitTestJob job[4];
        lolunit_assert_equal(0, m_manager.Test_GetDispatchCount());
        m_manager.AddJob(&job[0]);
        lolunit_assert_equal(1, m_manager.Test_GetDispatchCount());
        lolunit_assert_equal(false, job[0].IsDone());
        bool dispatch_check = true;
        while (!m_manager.IsDone())
        {
            m_manager.TickGame(1.f / 60.f);
            if (dispatch_check)
            {
                lolunit_assert_equal(0, m_manager.Test_GetDispatchCount());
#if !defined(LOL_FEATURE_THREADS) || !LOL_FEATURE_THREADS
                lolunit_assert_equal(0, m_manager.Test_GetDispatchedCount());
#else //LOL_FEATURE_THREADS
                lolunit_assert_equal(1, m_manager.Test_GetDispatchedCount());
#endif
                m_manager.AddJob(&job[1]);
                m_manager.AddJob(&job[2]);
                m_manager.AddJob(&job[3]);
                dispatch_check = false;
            }
        }
        lolunit_assert_equal(true, job[0].IsDone());
        lolunit_assert_equal(true, job[1].IsDone());
        lolunit_assert_equal(true, job[2].IsDone());
        lolunit_assert_equal(true, job[3].IsDone());
        array<ThreadJob*> results;
        m_manager.GetWorkResult(results);
        lolunit_assert_equal(4, results.count());

        msg::info("%s STOP\n", m_manager.GetName());
        //Stop manager
        m_manager.Stop();
        msg::info("%s STOPPED\n", m_manager.GetName());
    }

    lolunit_declare_test(queue_try_push)
    {
        queue<int, 1> q;

        bool b1 = q.try_push(0);
        lolunit_assert_equal(true, b1);

        bool b2 = q.try_push(1);
        lolunit_assert_equal(false, b2);
    }

    lolunit_declare_test(queue_try_pop)
    {
        queue<int, 1> q;
        int tmp;

        q.push(42);

        bool b1 = q.try_pop(tmp);
        lolunit_assert_equal(true, b1);
        lolunit_assert_equal(42, tmp);

        bool b2 = q.try_pop(tmp);
        lolunit_assert_equal(false, b2);
        lolunit_assert_equal(42, tmp);
    }
};

} /* namespace lol */

