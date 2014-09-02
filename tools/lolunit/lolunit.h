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

/*
 * This is the base class for all fixtures. It keeps track of all
 * fixtures registered through the lolunit_declare_fixture macro and puts them
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
 * test cases in a linked list through the lolunit_declare_test macro.
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

    protected:
        static inline std::string make_msg(std::string const str)
        {
            return "- " + str + "\n";
        }
    };

    Fixture()
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
        std::cout << "\n";

        std::cout << "\n\n";
        if (failcases)
        {
            std::cout << "!!!FAILURES!!!\n";
            std::cout << "Test Results:\n";
            std::cout << "Run:  " << testcases
                      << "  Failures: " << failcases
                      << "  Errors: 0\n"; /* TODO: handle errors */

            std::cout << errors.str();
            ret = false;
        }
        else
        {
            std::cout << "OK (" << testcases << " tests)\n";
        }
        std::cout << "\n\n";

        return ret;
    }
};

#define lolunit_assert_generic(msg, cond) \
    do { \
        m_asserts++; \
        if (True() && !(cond)) \
        { \
            m_errorlog << "\n\n"; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << "\n"; \
            m_errorlog << "assertion failed\n"; \
            m_errorlog << "- Expression: " << #cond << "\n"; \
            m_errorlog << msg; \
            m_failure = true; \
            return; \
        } \
    } while (!True())

#define lolunit_assert_op(op, modifier, opdesc, msg, a, b) \
    do { \
        m_asserts++; \
        if (True() && !modifier((a) op (b))) \
        { \
            m_errorlog << "\n\n"; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << "\n"; \
            m_errorlog << opdesc << " assertion failed\n"; \
            m_errorlog << "- Expected: " << #a << " = " << (a) << "\n"; \
            m_errorlog << "- Actual  : " << #b << " = " << (b) << "\n"; \
            m_errorlog << msg; \
            m_errorlog << m_context.str(); \
            m_failure = true; \
            return; \
        } \
    } while (!True())

#define lolunit_assert_doubles_equal_generic(msg, a, b, t) \
    do { \
        m_asserts++; \
        using std::fabs; \
        if (True() && fabs((a) - (b)) > fabs((t))) \
        { \
            m_errorlog << "\n\n"; \
            m_errorlog << ++m_failcases << ") test: " \
                       << lol_unit_helper_name(this) << "::" << m_currentname \
                       << " (F) line: " << __LINE__ << " " \
                       << __FILE__ << "\n"; \
            m_errorlog << "double equality assertion failed\n"; \
            std::streamsize old_prec = m_errorlog.precision(); \
            m_errorlog << std::setprecision(16); \
            m_errorlog << "- Expected: " << #a << " = " << (a) << "\n"; \
            m_errorlog << "- Actual  : " << #b << " = " << (b) << "\n"; \
            m_errorlog << "- Delta   : " << (t) << "\n"; \
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

#define lolunit_declare_fixture(N) \
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

#define lolunit_declare_test(N) \
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

#define lolunit_set_context(n) \
    do { \
        m_context.str(""); \
        m_context << "- Context : " << #n << " = " << (n) << "\n"; \
    } while (!True())

#define lolunit_unset_context(n) \
    m_context.str("")

/*
 * Public assert macros
 */

#define lolunit_fail(msg) \
    do { \
        m_asserts++; \
        m_errorlog << "\n\n"; \
        m_errorlog << ++m_failcases << ") test: " \
                   << lol_unit_helper_name(this) << "::" << m_currentname \
                   << " (F) line: " << __LINE__ << " " \
                   << __FILE__ << "\n"; \
        m_errorlog << "forced failure\n"; \
        m_errorlog << make_msg(msg); \
        m_errorlog << m_context.str(); \
        m_failure = true; \
        return; \
    } while (!True())

#define lolunit_assert(cond) \
    lolunit_assert_generic("", cond)
#define lolunit_assert_message(m, cond) \
    lolunit_assert_generic(make_msg(m), cond)


#define lolunit_assert_equal(a, b) \
    lolunit_assert_op(==, (bool), "equality", "", a, b)
#define lolunit_assert_equal_message(m, a, b) \
    lolunit_assert_op(==, (bool), "equality", make_msg(m), a, b)
#define lolunit_assert_different(a, b) \
    lolunit_assert_op(!=, (bool), "inequality", "", a, b)
#define lolunit_assert_different_message(m, a, b) \
    lolunit_assert_op(!=, (bool), "inequality", make_msg(m), a, b)
#define lolunit_assert_less(a, b) \
    lolunit_assert_op(<, (bool), "less than", "", a, b)
#define lolunit_assert_less_message(m, a, b) \
    lolunit_assert_op(<, (bool), "less than", make_msg(m), a, b)
#define lolunit_assert_lequal(a, b) \
    lolunit_assert_op(<=, (bool), "less than or equal", "", a, b)
#define lolunit_assert_lequal_message(m, a, b) \
    lolunit_assert_op(<=, (bool), "less than or equal", make_msg(m), a, b)
#define lolunit_assert_greater(a, b) \
    lolunit_assert_op(>, (bool), "greater than", "", a, b)
#define lolunit_assert_greater_message(m, a, b) \
    lolunit_assert_op(>, (bool), "greater than", make_msg(m), a, b)
#define lolunit_assert_gequal(a, b) \
    lolunit_assert_op(>=, (bool), "greater than or equal", "", a, b)
#define lolunit_assert_gequal_message(m, a, b) \
    lolunit_assert_op(>=, (bool), "greater than or equal", make_msg(m), a, b)


#define lolunit_assert_not_equal(a, b) \
    lolunit_assert_op(==, !, "not equality", "", a, b)
#define lolunit_assert_not_equal_message(m, a, b) \
    lolunit_assert_op(==, !, "not equality", make_msg(m), a, b)
#define lolunit_assert_not_different(a, b) \
    lolunit_assert_op(!=, !, "not inequality", "", a, b)
#define lolunit_assert_not_different_message(m, a, b) \
    lolunit_assert_op(!=, !, "not inequality", make_msg(m), a, b)
#define lolunit_assert_not_less(a, b) \
    lolunit_assert_op(<, !, "not less than", "", a, b)
#define lolunit_assert_not_less_message(m, a, b) \
    lolunit_assert_op(<, !, "not less than", make_msg(m), a, b)
#define lolunit_assert_not_lequal(a, b) \
    lolunit_assert_op(<=, !, "not less than or equal", "", a, b)
#define lolunit_assert_not_lequal_message(m, a, b) \
    lolunit_assert_op(<=, !, "not less than or equal", make_msg(m), a, b)
#define lolunit_assert_not_greater(a, b) \
    lolunit_assert_op(>, !, "not greater than", "", a, b)
#define lolunit_assert_not_greater_message(m, a, b) \
    lolunit_assert_op(>, !, "not greater than", make_msg(m), a, b)
#define lolunit_assert_not_gequal(a, b) \
    lolunit_assert_op(>=, !, "not greater than or equal", "", a, b)
#define lolunit_assert_not_gequal_message(m, a, b) \
    lolunit_assert_op(>=, !, "not greater than or equal", make_msg(m), a, b)

#define lolunit_assert_doubles_equal(a, b, t) \
    lolunit_assert_doubles_equal_generic("", a, b, t)
#define lolunit_assert_doubles_equal_message(msg, a, b, t) \
    lolunit_assert_doubles_equal_generic(make_msg(msg), a, b, t)

} /* namespace lol */

#endif // __LOL_UNIT_H__

