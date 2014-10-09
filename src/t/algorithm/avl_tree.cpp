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

#include <lol/algorithm/avl_tree.h>

#include <lolunit.h>

namespace lol
{

class test_tree : public avl_tree<int, int>
{
public:

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

        lolunit_assert_equal(tree.insert(1, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(2, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(3, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(0, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-1, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), -1);

        lolunit_assert_equal(tree.insert(-2, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);
    }
};

}
