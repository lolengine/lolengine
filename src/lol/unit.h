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
    FixtureBase() : m_next(NULL), m_tests(0), m_fails(0) {}
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

    virtual int RunFixture() = 0;

    /* Prevent compiler complaints about unreachable code */
    static inline bool True() { return true; }

    FixtureBase *m_next;
    int m_tests, m_fails;
};

template<class T> class Fixture : protected FixtureBase
{
public:
    typedef T FixtureClass;

    class TestCase
    {
    public:
        void (FixtureClass::* m_fun)();
        char const *m_name;
        TestCase *m_next;

        static void AddTestCase(TestCase *that, char const *name,
                                void (FixtureClass::*fun)())
        {
            that->m_name = name;
            that->m_fun = fun;
            GetOrSetTestCase(that);
        }
    };

    Fixture<T>()
    {
        GetOrSetTest(this);
    }

    /* Run all test cases in this fixture. */
    virtual int RunFixture()
    {
        for (TestCase *head = GetOrSetTestCase(); head; head = head->m_next)
        {
            (static_cast<FixtureClass *>(this)->*head->m_fun)();
            std::cout << ".";
        }
        return 0;
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
    int Run()
    {
        int ret = 0;
        for (FixtureBase *head = FixtureBase::GetOrSetTest(); head; head = head->m_next)
        {
            head->setUp();
            if (head->RunFixture())
                ret = 1;
            head->tearDown();
        }
        std::cout << std::endl;
        return ret;
    }
};

#define LOLUNIT_FIXTURE(FixtureName) \
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
    TestCase##TestCaseName test_case_##TestCaseName; \
    void TestCaseName()

#define LOLUNIT_SETUP_FIXTURE(ClassName) \
    ClassName ClassName##Test_Instance;

#define LOLUNIT_ASSERT(cond) \
    do { if (True() && !(cond)) \
    { \
        std::cout << "FAIL! " #cond << std::endl; \
        return; \
    } } while(!True())

#define LOLUNIT_ASSERT_EQUAL(a, b) \
    do { if (True() && (a) != (b)) \
    { \
        std::cout << "FAIL! " #a " != " #b << std::endl; \
        std::cout << "expected: " << (a) << std::endl; \
        std::cout << "returned: " << (b) << std::endl; \
        return; \
    } } while(!True())

#define LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, t) \
    do { if (True() && fabs((a) - (b)) > fabs((t))) \
    { \
        std::cout << "FAIL! " #a " != " #b << std::endl; \
        std::cout << "expected: " << (a) << std::endl; \
        std::cout << "returned: " << (b) << std::endl; \
        return; \
    } } while(!True())

} /* namespace lol */

#endif // __LOL_UNIT_H__

