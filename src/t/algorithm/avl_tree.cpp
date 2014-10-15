//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            (c) 2013-2014 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

class test_tree : public avl_tree<int, int>
{
public:
    virtual ~test_tree() {}

	int get_root_balance()
	{
		return this->m_root->get_balance();
	}
};

lolunit_declare_fixture(AvlTreeTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(AvlTreeInsert)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(1, 1), true);
        lolunit_assert_equal(tree.insert(2, 3), true);
        lolunit_assert_equal(tree.insert(2, 0), false);
    }

    lolunit_declare_test(AvlTreeBalanceCheck)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(00, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(-20, 1), false);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(11, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(13, 2), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);
    }

    lolunit_declare_test(AvlTreeDeletion)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.erase(30), true);
        lolunit_assert_equal(tree.insert(30, 1), true);

        lolunit_assert_equal(tree.erase(20), true);
        lolunit_assert_equal(tree.insert(20, 1), true);
    }

    lolunit_declare_test(AvlTreeExistence)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.erase(10), true);
        lolunit_assert_equal(tree.exists(20), true);
        lolunit_assert_equal(tree.exists(10), false);

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(40, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(50, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.erase(30), true);
        lolunit_assert_equal(tree.exists(40), true);
        lolunit_assert_equal(tree.exists(50), true);
    }


    lolunit_declare_test(AvlTreeGetValue)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(00, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(-20, 1), false);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(11, 2), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(13, 3), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        int * value_ptr = nullptr;

        lolunit_assert_equal(tree.try_get_value(-10, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 1);

        lolunit_assert_equal(tree.try_get_value(11, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 2);

        lolunit_assert_equal(tree.try_get_value(13, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 3);

        lolunit_assert_equal(tree.try_get_value(67, value_ptr), false);
        lolunit_assert_equal(*value_ptr, 3);
    }
};

}
