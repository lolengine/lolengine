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
// The Unit test framework
// -----------------------
//

#if !defined __LOL_UNIT_H__
#define __LOL_UNIT_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>

namespace lol
{

using namespace std;

/*
 * This is the base class for all fixtures. It keeps track of all
 * fixtures registered through the LOLUNIT_FIXTURE macro and puts them
 * in a linked list.
 */
class FixtureBase
{
    friend class TextTestRunner;

public:
    virtual void SetUp(void) {};
    virtual void TearDown(void) {};

protected:
    FixtureBase() : m_next(NULL), m_testcases(0), m_failcases(0) {}
    virtual ~FixtureBase() {}

    static void AddFixture(FixtureBase *fixture)
    {
        /* Protect against several instances of the same Fixture subclass */
        if (fixture->MarkFixture())
            return;
        FixtureListHelper(fixture);
    }
    static FixtureBase *FixtureList() { return FixtureListHelper(NULL); }

    virtual void RunFixture() = 0;
    virtual bool MarkFixture() = 0;

    /* Prevent compiler complaints about unreachable code */
    static inline bool True() { return true; }

    FixtureBase *m_next;
    int m_testcases, m_failcases;
    int m_asserts, m_failure;
    char const *m_fixturename, *m_currentname;
    std::stringstream m_errorlog, m_context;

private:
    /* The FixtureBase class keeps track of all instanciated children
     * fixtures through this method. */
    static FixtureBase *FixtureListHelper(FixtureBase *set)
    {
        static FixtureBase *head = NULL, *tail = NULL;

        if (set)
        {
            if (!head) head = set;
            if (tail) tail->m_next = set;
            tail = set;
        }
        return head;
    }
};

/*
 * This template specialises FixtureBase and provides registration of
 * test cases in a linked list through the LOLUNIT_TEST macro.
 */
template<class T> class Fixture : protected FixtureBase
{
public:
    typedef T FixtureClass;

    struct TestCase
    {
        void (FixtureClass::* m_fun)();
        char const *m_testname;
        TestCase *m_next;
    };

    Fixture<T>()
    {
        AddFixture(this);
    }

    /* Run all test cases in this fixture. */
    virtual void RunFixture()
    {
        m_errorlog.str("");
        m_testcases = 0;
        m_failcases = 0;
        for (TestCase *c = TestCaseList(); c; c = c->m_next)
        {
            m_testcases++;
            m_asserts = 0;
            m_failure = false;
            m_currentname = c->m_testname;
            m_context.str("");

            std::cout << '.';
            (static_cast<FixtureClass *>(this)->*c->m_fun)();
            if (m_failure) std::cout << 'F';
        }
    }

    /* Mark the current fixture type as already registered and return whether
     * it was seen before. */
    virtual bool MarkFixture()
    {
        static bool seen = false;
        if (seen)
        {
            SealFixture();
            return true;
        }
        seen = true;
        return false;
    }

    /* Manage Fixture sealing. Once SealFixture() has been called, we
     * will no longer accept TestCase registrations. */
    static void SealFixture() { SealFixtureHelper(true); }
    static bool IsFixtureSealed() { return SealFixtureHelper(false); }

    /* Each Fixture class specialisation keeps track of its instanciated
     * test cases. */
    static void AddTestCase(TestCase *that, char const *name,
                            void (FixtureClass::*fun)())
    {
        if (IsFixtureSealed())
            return;

        that->m_fun = fun;
        that->m_testname = name;
        that->m_next = NULL;
        TestCaseListHelper(that);
    }
    static TestCase *TestCaseList() { return TestCaseListHelper(NULL); }

private:
    static bool SealFixtureHelper(bool set)
    {
        static bool sealed = false;
        if (set) sealed = true;
        return sealed;
    }

    static TestCase *TestCaseListHelper(TestCase *set)
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

/*
 * This simple class runs all automatically registered tests and reports
 * on error and success in the standard output.
 */
class TextTestRunner
{
public:
    bool Run()
    {
        bool ret = true;
        std::stringstream errors("");
        int failcases = 0, testcases = 0;

        for (FixtureBase *f = FixtureBase::FixtureList(); f; f = f->m_next)
        {
            f->SetUp();
            f->RunFixture();
            f->TearDown();

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

#define LOLUNIT_ASSERT_GENERIC(msg, cond) \
    do { \
        m_asserts++; \
        if (True() && !(cond)) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << "assertion failed" << std::endl; \
            m_errorlog << "- Expression: " << #cond << std::endl; \
            m_errorlog << msg; \
            m_failure = true; \
            return; \
        } \
    } while (!True())

#define LOLUNIT_ASSERT_OP(op, modifier, opdesc, msg, a, b) \
    do { \
        m_asserts++; \
        if (True() && !modifier((a) op (b))) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << opdesc << " assertion failed" << std::endl; \
            m_errorlog << "- Expected: " << #a << " = " << (a) << std::endl; \
            m_errorlog << "- Actual  : " << #b << " = " << (b) << std::endl; \
            m_errorlog << msg; \
            m_errorlog << m_context.str(); \
            m_failure = true; \
            return; \
        } \
    } while (!True())

#define LOLUNIT_MSG(msg) \
    "- " << msg << std::endl

#define LOLUNIT_ASSERT_DOUBLES_EQUAL_GENERIC(msg, a, b, t) \
    do { \
        m_asserts++; \
        using std::fabs; \
        if (True() && fabs((a) - (b)) > fabs((t))) \
        { \
            m_errorlog << std::endl << std::endl; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << std::endl; \
            m_errorlog << "double equality assertion failed" << std::endl; \
            std::streamsize old_prec = m_errorlog.precision(); \
            m_errorlog << std::setprecision(16); \
            m_errorlog << "- Expected: " << #a << " = " << (a) << std::endl; \
            m_errorlog << "- Actual  : " << #b << " = " << (b) << std::endl; \
            m_errorlog << "- Delta   : " << (t) << std::endl; \
            m_errorlog << std::setprecision(old_prec); \
            m_errorlog << msg; \
            m_errorlog << m_context.str(); \
            m_failure = true; \
            return; \
        } \
    } while (!True())

/*
 * Public helper macros
 */

#define LOLUNIT_FIXTURE(N) \
    class N; \
    /* This pattern allows us to statically create a Fixture instance \
     * before its exact implementation was defined. */ \
    template<typename T> struct lol_unit_helper_fixture_##N \
    { \
        lol_unit_helper_fixture_##N() { p = new T(); } \
        ~lol_unit_helper_fixture_##N() { delete p; } \
        T *p; \
    }; \
    lol_unit_helper_fixture_##N<N> lol_unit_helper_fixture_##N##_instance; \
    /* Allow to retrieve the class name without using RTTI and without \
     * knowing the type of "this". */ \
    static inline char const *lol_unit_helper_name(N *p) \
    { \
        (void)p; \
        return #N; \
    } \
    /* Now the user can define the implementation */ \
    class N : public lol::Fixture<N>

#define LOLUNIT_TEST(N) \
    /* For each test in the fixture, we create an object that will \
     * automatically register the test method in a list global to the \
     * specialised fixture. */ \
    class lol_unit_helper_test_##N : public TestCase \
    { \
    public: \
        lol_unit_helper_test_##N() \
        { \
            AddTestCase(this, #N, \
                        (void (FixtureClass::*)()) &FixtureClass::N); \
        } \
    }; \
    lol_unit_helper_test_##N lol_unit_helper_test_instance_##N; \
    void N()

/*
 * Provide context for error messages
 */

#define LOLUNIT_SET_CONTEXT(n) \
    do { \
        m_context.str(""); \
        m_context << "- Context : " << #n << " = " << (n) << std::endl; \
    } while (!True())

#define LOLUNIT_UNSET_CONTEXT(n) \
    m_context.str("")

/*
 * Public assert macros
 */

#define LOLUNIT_FAIL(msg) \
    do { \
        m_asserts++; \
        m_errorlog << std::endl << std::endl; \
        m_errorlog << ++m_failcases << ") test: " \
                   << lol_unit_helper_name(this) << "::" << m_currentname \
                   << " (F) line: " << __LINE__ << " " \
                   << __FILE__ << std::endl; \
        m_errorlog << "forced failure" << std::endl; \
        m_errorlog << LOLUNIT_MSG(msg); \
        m_errorlog << m_context.str(); \
        m_failure = true; \
        return; \
    } while (!True())

#define LOLUNIT_ASSERT(cond) \
    LOLUNIT_ASSERT_GENERIC("", cond)
#define LOLUNIT_ASSERT_MESSAGE(m, cond) \
    LOLUNIT_ASSERT_GENERIC(LOLUNIT_MSG(m), cond)


#define LOLUNIT_ASSERT_EQUAL(a, b) \
    LOLUNIT_ASSERT_OP(==, (bool), "equality", "", a, b)
#define LOLUNIT_ASSERT_EQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(==, (bool), "equality", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_DIFFERENT(a, b) \
    LOLUNIT_ASSERT_OP(!=, (bool), "inequality", "", a, b)
#define LOLUNIT_ASSERT_DIFFERENT_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(!=, (bool), "inequality", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_LESS(a, b) \
    LOLUNIT_ASSERT_OP(<, (bool), "less than", "", a, b)
#define LOLUNIT_ASSERT_LESS_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(<, (bool), "less than", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_LEQUAL(a, b) \
    LOLUNIT_ASSERT_OP(<=, (bool), "less than or equal", "", a, b)
#define LOLUNIT_ASSERT_LEQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(<=, (bool), "less than or equal", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_GREATER(a, b) \
    LOLUNIT_ASSERT_OP(>, (bool), "greater than", "", a, b)
#define LOLUNIT_ASSERT_GREATER_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(>, (bool), "greater than", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_GEQUAL(a, b) \
    LOLUNIT_ASSERT_OP(>=, (bool), "greater than or equal", "", a, b)
#define LOLUNIT_ASSERT_GEQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(>=, (bool), "greater than or equal", LOLUNIT_MSG(m), a, b)


#define LOLUNIT_ASSERT_NOT_EQUAL(a, b) \
    LOLUNIT_ASSERT_OP(==, !, "not equality", "", a, b)
#define LOLUNIT_ASSERT_NOT_EQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(==, !, "not equality", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_NOT_DIFFERENT(a, b) \
    LOLUNIT_ASSERT_OP(!=, !, "not inequality", "", a, b)
#define LOLUNIT_ASSERT_NOT_DIFFERENT_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(!=, !, "not inequality", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_NOT_LESS(a, b) \
    LOLUNIT_ASSERT_OP(<, !, "not less than", "", a, b)
#define LOLUNIT_ASSERT_NOT_LESS_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(<, !, "not less than", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_NOT_LEQUAL(a, b) \
    LOLUNIT_ASSERT_OP(<=, !, "not less than or equal", "", a, b)
#define LOLUNIT_ASSERT_NOT_LEQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(<=, !, "not less than or equal", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_NOT_GREATER(a, b) \
    LOLUNIT_ASSERT_OP(>, !, "not greater than", "", a, b)
#define LOLUNIT_ASSERT_NOT_GREATER_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(>, !, "not greater than", LOLUNIT_MSG(m), a, b)
#define LOLUNIT_ASSERT_NOT_GEQUAL(a, b) \
    LOLUNIT_ASSERT_OP(>=, !, "not greater than or equal", "", a, b)
#define LOLUNIT_ASSERT_NOT_GEQUAL_MESSAGE(m, a, b) \
    LOLUNIT_ASSERT_OP(>=, !, "not greater than or equal", LOLUNIT_MSG(m), a, b)

#define LOLUNIT_ASSERT_DOUBLES_EQUAL(a, b, t) \
    LOLUNIT_ASSERT_DOUBLES_EQUAL_GENERIC("", a, b, t)
#define LOLUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg, a, b, t) \
    LOLUNIT_ASSERT_DOUBLES_EQUAL_GENERIC(LOLUNIT_MSG(msg), a, b, t)

} /* namespace lol */

#endif // __LOL_UNIT_H__

