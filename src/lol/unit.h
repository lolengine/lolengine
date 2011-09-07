//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Unit test framework
// -----------------------
//

#if !defined __LOL_UNIT_H__
#define __LOL_UNIT_H__

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>

namespace lol
{

using namespace std;

class FixtureBase
{
    friend class TestRunner;

public:
    virtual void setUp(void) {};
    virtual void tearDown(void) {};

protected:
    FixtureBase() : m_next(NULL), m_testcases(0), m_failcases(0) {}
    virtual ~FixtureBase() {}

    /* The FixtureBase class keeps track of all instanciated children
     * fixtures through this method. */
    static FixtureBase *GetOrSetTest(FixtureBase *set = NULL)
    {
        static FixtureBase *head = NULL, *tail = NULL;
        if (set)
        {
            if (!head)
                head = set;
            if (tail)
                tail->m_next = set;
            tail = set;
        }
        return head;
    }

    virtual void RunFixture() = 0;

    /* Prevent compiler complaints about unreachable code */
    static inline bool True() { return true; }

    FixtureBase *m_next;
    int m_testcases, m_failcases;
    int m_asserts, m_failure;
    char const *m_fixturename, *m_currentname;
    std::stringstream m_errorlog;
};

template<class T> class Fixture : protected FixtureBase
{
public:
    typedef T FixtureClass;

    class TestCase
    {
    public:
        void (FixtureClass::* m_fun)();
        char const *m_testname;
        TestCase *m_next;

        static void AddTestCase(TestCase *that, char const *name,
                                void (FixtureClass::*fun)())
        {
            that->m_testname = name;
            that->m_fun = fun;
            GetOrSetTestCase(that);
        }
    };

    Fixture<T>()
    {
        GetOrSetTest(this);
    }

    /* Run all test cases in this fixture. */
    virtual void RunFixture()
    {
        m_errorlog.str("");
        m_testcases = 0;
        m_failcases = 0;
        for (TestCase *c = GetOrSetTestCase(); c; c = c->m_next)
        {
            m_testcases++;
            m_asserts = 0;
            m_failure = false;
            m_currentname = c->m_testname;
            (static_cast<FixtureClass *>(this)->*c->m_fun)();
            std::cout << (m_failure ? 'F' : '.');
        }
    }

    /* Each Fixture class specialisation keeps track of its instanciated
     * test cases through this method. */
    static TestCase *GetOrSetTestCase(TestCase *set = NULL)
    {
        static TestCase *head = NULL, *tail = NULL;
        if (set)
        {
            if (!head) head = set;
            if (tail) tail->m_next = set;
            tail = set;
        }
        return head;
    }
};

class TestRunner
{
public:
    bool Run()
    {
        bool ret = true;
        std::stringstream errors("");
        int failcases = 0, testcases = 0;

        for (FixtureBase *f = FixtureBase::GetOrSetTest(); f; f = f->m_next)
        {
            f->setUp();
            f->RunFixture();
            f->tearDown();

            errors << f->m_errorlog.str();
            testcases += f->m_testcases;
            failcases += f->m_failcases;
        }
        std::cout << std::endl;

        std::cout << std::endl << std::endl;
        if (failcases)
        {
            std::cout << "!!!FAILURES!!!" << std::endl;
            std::cout << "Test Results:" << std::endl;
            std::cout << "Run:  " << testcases
                      << "  Failures: " << failcases
                      << "  Errors: 0" << std::endl; /* TODO: handle errors */

            std::cout << errors.str();
            ret = false;
        }
        else
        {
            std::cout << "OK (" << testcases << " tests)" << std::endl;
        }
        std::cout << std::endl << std::endl;

        return ret;
    }
};

#define LOLUNIT_FIXTURE(FixtureName) \
    class FixtureName; \
    static char const *LolUnitFixtureName(FixtureName *p) \
    { \
        (void)p; \
        return #FixtureName; \
    } \
    class FixtureName : public lol::Fixture<FixtureName>

#define LOLUNIT_TEST(TestCaseName) \
    class TestCase##TestCaseName : public TestCase \
    { \
    public: \
        TestCase##TestCaseName() \
        { \
            AddTestCase(this, #TestCaseName, \
                    (void (FixtureClass::*)()) &FixtureClass::TestCaseName); \
        } \
    }; \
    TestCase##TestCaseName lol_unit_test_case_##TestCaseName; \
    void TestCaseName()

#define LOLUNIT_SETUP_FIXTURE(ClassName) \
    ClassName ClassName##Test_Instance;

#define LOLUNIT_ASSERT(cond) \
    do { \
        m_asserts++; \
        if (True() && !(cond)) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << LolUnitFixtureName(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << "assertion failed" << std::endl; \
            m_errorlog << "- Expression: " << #cond << std::endl; \
            m_failure = true; \
            return; \
        } \
    } while(!True())

#define LOLUNIT_ASSERT_EQUAL(a, b) \
    do { \
        m_asserts++; \
        if (True() && (a) != (b)) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << LolUnitFixtureName(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << "equality assertion failed" << std::endl; \
            m_errorlog << "- Expected: " << (b) << std::endl; \
            m_errorlog << "- Actual  : " << (a) << std::endl; \
            m_errorlog << message; \
            m_failure = true; \
            return; \
        } \
    } while(!True())

#define LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, t) \
    do { \
        m_asserts++; \
        if (True() && fabs((a) - (b)) > fabs((t))) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << LolUnitFixtureName(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << "double equality assertion failed" << std::endl; \
            m_errorlog << "- Expected: " << (b) << std::endl; \
            m_errorlog << "- Actual  : " << (a) << std::endl; \
            m_errorlog << "- Delta   : " << (t) << std::endl; \
            m_failure = true; \
            return; \
        } \
    } while(!True())

} /* namespace lol */

#endif // __LOL_UNIT_H__

