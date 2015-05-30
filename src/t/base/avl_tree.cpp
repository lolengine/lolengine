//
//  Lol Engine — Unit tests
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//            © 2013—2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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

lolunit_declare_fixture(avl_tree_test)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(insert)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(1, 1), true);
        lolunit_assert_equal(tree.insert(2, 3), true);
        lolunit_assert_equal(tree.insert(2, 0), false);
    }

    lolunit_declare_test(balance_check)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(0, 1), true);
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

    lolunit_declare_test(deletion)
    {
        test_tree tree;

        lolunit_assert_equal(tree.insert(10, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 1);

        lolunit_assert_equal(tree.insert(30, 1), true);
        lolunit_assert_equal(tree.get_root_balance(), 0);

        lolunit_assert_equal(tree.count(), 3);

        lolunit_assert_equal(tree.erase(30), true);

        int test = 0;

        for (auto iterator : tree)
        {
            test += 10;
            lolunit_assert_equal(iterator.key, test);
        }

        lolunit_assert_equal(tree.count(), 2);
        lolunit_assert_equal(tree.insert(30, 1), true);

        lolunit_assert_equal(tree.erase(20), true);
        lolunit_assert_equal(tree.insert(20, 1), true);
        lolunit_assert_equal(tree.exists(10), true);

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

        lolunit_assert_equal(tree.insert(0, 1), true);
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

        lolunit_assert_equal(tree.try_get(-10, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 1);

        lolunit_assert_equal(tree.try_get(11, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 2);

        lolunit_assert_equal(tree.try_get(13, value_ptr), true);
        lolunit_assert_equal(*value_ptr, 3);

        lolunit_assert_equal(tree.try_get(67, value_ptr), false);
        lolunit_assert_equal(*value_ptr, 3);
    }

    lolunit_declare_test(AvlTreeTestIteratorRead)
    {
        test_tree tree;

        for (int i = 1 ; i < 100 ; ++i)
            tree.insert(i, 2 * i + i % 3);

        int tmp = 0;

        for (auto iterator : tree)
        {
            lolunit_assert_equal(iterator.key > tmp, true);
            lolunit_assert_equal(iterator.value == (iterator.key * 2 + iterator.key % 3), true);
            tmp = iterator.key;
        }

        lolunit_assert_equal(tree.count(), 99);
    }

    lolunit_declare_test(AvlTreeTestIteratorCopy)
    {
        test_tree tree;

        for (int i = 1 ; i < 100 ; ++i)
            tree.insert(i, 2 * i + i % 3);

        test_tree other = tree;

        int tmp = 0;

        for (auto iterator : other)
        {
            lolunit_assert_equal(iterator.key > tmp, true);
            lolunit_assert_equal(iterator.value == (iterator.key * 2 + iterator.key % 3), true);
            tmp = iterator.key;
        }

        lolunit_assert_equal(other.count(), 99);
    }

    lolunit_declare_test(AvlTreeTestCopy)
    {
        avl_tree<int, int> test1, test2;

        for (int i = 0 ; i < 10 ; ++i)
            test1.insert(i, 2*i);

        for (int i = 10 ; i < 15 ; ++i)
            test2.insert(i, 3*i);

        lolunit_assert_equal(test1.count(), 10);
        lolunit_assert_equal(test2.count(), 5);

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

        lolunit_assert_equal(test1.count(), 10);
        lolunit_assert_equal(test2.count(), 10);
    }
};

}
