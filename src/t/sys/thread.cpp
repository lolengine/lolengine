//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//            © 2014—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <string>
#include <map>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(thread_test)
{
    void setup()
    {
    }

    void teardown()
    {
    }

    lolunit_declare_test(threads)
    {
        msg::info("%s START\n", m_manager.GetName().c_str());
        //Start threads manager
        m_manager.Start();
        msg::info("%s STARTED\n", m_manager.GetName().c_str());

        UnitTestJob job[4];
        lolunit_assert_equal(0, m_manager.Test_GetDispatchCount());
        m_manager.AddJob(&job[0]);
        lolunit_assert_equal(1, m_manager.Test_GetDispatchCount());
        lolunit_assert_equal(false, job[0].IsDone());
        bool dispatch_check = true;
        while (!m_manager.IsDone())
        {
            m_manager.tick_game(1.f / 60.f);
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

        msg::info("%s STOP\n", m_manager.GetName().c_str());
        //Stop manager
        m_manager.Stop();
        msg::info("%s STOPPED\n", m_manager.GetName().c_str());
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

