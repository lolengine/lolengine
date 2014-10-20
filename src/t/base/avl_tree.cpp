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

	int GetRootBalance()
	{
		return this->m_root->GetBalance();
	}
};

lolunit_declare_fixture(AvlTreeTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(AvlTreeInsert)
    {
        test_tree tree;

        lolunit_assert_equal(tree.Insert(1, 1), true);
        lolunit_assert_equal(tree.Insert(2, 3), true);
        lolunit_assert_equal(tree.Insert(2, 0), false);
    }

    lolunit_declare_test(AvlTreeBalanceCheck)
    {
        test_tree tree;

        lolunit_assert_equal(tree.Insert(10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(30, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(00, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), -1);

        lolunit_assert_equal(tree.Insert(-10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), -1);

        lolunit_assert_equal(tree.Insert(-20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(-20, 1), false);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(11, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(13, 2), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);
    }

    lolunit_declare_test(AvlTreeDeletion)
    {
        test_tree tree;

        lolunit_assert_equal(tree.Insert(10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(30, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.GetCount(), 3);

        lolunit_assert_equal(tree.Erase(30), true);

        int test = 0;

        for (auto iterator : tree)
        {
            test += 10;
            lolunit_assert_equal(iterator.key, test);
        }

        lolunit_assert_equal(tree.GetCount(), 2);
        lolunit_assert_equal(tree.Insert(30, 1), true);

        lolunit_assert_equal(tree.Erase(20), true);
        lolunit_assert_equal(tree.Insert(20, 1), true);
        lolunit_assert_equal(tree.Exists(10), true);

        test = 0;

        for (auto iterator : tree)
        {
            test += 10;
            lolunit_assert_equal(iterator.key, test);
        }
    }

    lolunit_declare_test(AvlTreeExistence)
    {
        test_tree tree;

        lolunit_assert_equal(tree.Insert(10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Erase(10), true);
        lolunit_assert_equal(tree.Exists(20), true);
        lolunit_assert_equal(tree.Exists(10), false);

        lolunit_assert_equal(tree.Insert(10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), -1);

        lolunit_assert_equal(tree.Insert(30, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(40, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(50, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Erase(30), true);
        lolunit_assert_equal(tree.Exists(40), true);
        lolunit_assert_equal(tree.Exists(50), true);
    }


    lolunit_declare_test(AvlTreeGetValue)
    {
        test_tree tree;

        lolunit_assert_equal(tree.Insert(10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(30, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(00, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), -1);

        lolunit_assert_equal(tree.Insert(-10, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), -1);

        lolunit_assert_equal(tree.Insert(-20, 1), true);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(-20, 1), false);
        lolunit_assert_equal(tree.GetRootBalance(), 0);

        lolunit_assert_equal(tree.Insert(11, 2), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        lolunit_assert_equal(tree.Insert(13, 3), true);
        lolunit_assert_equal(tree.GetRootBalance(), 1);

        int * value_ptr = nullptr;

        lolunit_assert_equal(tree.TryGetValue(-10, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 1);

        lolunit_assert_equal(tree.TryGetValue(11, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 2);

        lolunit_assert_equal(tree.TryGetValue(13, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 3);

        lolunit_assert_equal(tree.TryGetValue(67, value_ptr), false);
        lolunit_assert_equal(*value_ptr, 3);
    }

    lolunit_declare_test(AvlTreeTestIteratorRead)
    {
        test_tree tree;

        for (int i = 1 ; i < 100 ; ++i)
            tree.Insert(i, 2 * i + i % 3);

        int tmp = 0;

        for (auto iterator : tree)
        {
            lolunit_assert_equal(iterator.key > tmp, true);
            lolunit_assert_equal(iterator.value == (iterator.key * 2 + iterator.key % 3), true);
            tmp = iterator.key;
        }

        lolunit_assert_equal(tree.GetCount(), 99);
    }

    lolunit_declare_test(AvlTreeTestIteratorCopy)
    {
        test_tree tree;

        for (int i = 1 ; i < 100 ; ++i)
            tree.Insert(i, 2 * i + i % 3);

        test_tree other = tree;

        int tmp = 0;

        for (auto iterator : other)
        {
            lolunit_assert_equal(iterator.key > tmp, true);
            lolunit_assert_equal(iterator.value == (iterator.key * 2 + iterator.key % 3), true);
            tmp = iterator.key;
        }

        lolunit_assert_equal(other.GetCount(), 99);
    }

    lolunit_declare_test(AvlTreeTestCopy)
    {
        avl_tree<int, int> test1, test2;

        for (int i = 0 ; i < 10 ; ++i)
            test1.Insert(i, 2*i);

        for (int i = 10 ; i < 15 ; ++i)
            test2.Insert(i, 3*i);

        lolunit_assert_equal(test1.GetCount(), 10);
        lolunit_assert_equal(test2.GetCount(), 5);

        int i = -1;
        for (auto iterator : test1)
        {
            ++i;
            lolunit_assert_equal(iterator.key, i);
            lolunit_assert_equal(iterator.value, 2*i);
        }

        for (auto iterator : test2)
        {
            ++i;
            lolunit_assert_equal(iterator.key, i);
            lolunit_assert_equal(iterator.value, 3*i);
        }

        test2 = test1;

        auto it1 = test1.begin();
        auto it2 = test2.begin();

        for ( ; it1 != test1.end() && it2 != test2.end() ; ++it1 , ++it2)
        {
            lolunit_assert_equal((*it1).key, (*it2).key);
            lolunit_assert_equal((*it1).value, (*it2).value);
        }

        lolunit_assert(!(it1 != test1.end()));
        lolunit_assert(!(it2 != test2.end()));

        lolunit_assert_equal(test1.GetCount(), 10);
        lolunit_assert_equal(test2.GetCount(), 10);
    }
};

}
