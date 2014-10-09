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

#pragma once


namespace lol
{


template<typename K, typename V>
class avl_tree
{
public:
    avl_tree() :
        m_root(nullptr)
    {
    }

    bool insert(K const & key, V const & value)
    {
        if (!m_root)
            this->m_root = new tree_node(key, value);
        else
        {
            tree_node * created = this->m_root->insert(key, value);

            if (created)
            {
                this->m_root->path_update_balance(key);

                tree_node * new_root = this->m_root->path_rebalance(key);
                if (new_root)
                    this->m_root = new_root;
            }
            else
                return false;
        }

        return true;
    }

protected:

    class tree_node
    {
    public:
        tree_node(K key, V value) :
            m_key(key),
            m_value(value)
        {
            m_child[0] = m_child[1] = nullptr;
            m_stairs[0] = m_stairs[1] = 0;
        }

        tree_node * insert(K const & key, V const & value)
        {
            tree_node * ret = 0;

            if (key < this->m_key)
            {
                if (this->m_child[0])
                    ret = this->m_child[0]->insert(key, value);
                else
                    ret = this->m_child[0] = new tree_node(key, value);
            }
            else if (this->m_key < key)
            {
                if (this->m_child[1])
                    ret = this->m_child[1]->insert(key, value);
                else
                    ret = this->m_child[1] = new tree_node(key, value);
            }
            else
                this->m_value = value;

            return ret;
        }

        int path_update_balance(K const & key)
        {
            if (key < this->m_key)
                this->m_stairs[0] = lol::max(this->m_child[0]->path_update_balance(key), this->m_stairs[0]);
            else if (this->m_key < key)
                this->m_stairs[1] = lol::max(this->m_child[1]->path_update_balance(key), this->m_stairs[1]);

            return lol::max(this->m_stairs[0], this->m_stairs[1]) + 1;
        }

        tree_node * path_rebalance(K const & key)
        {
            if (key < this->m_key)
            {
                tree_node * node = this->m_child[0]->path_rebalance(key);
                if (node)
                {
                    this->m_child[0] = node;
                    --this->m_stairs[0];
                }
            }
            else if (this->m_key < key)
            {
                tree_node * node = this->m_child[1]->path_rebalance(key);
                if (node)
                {
                    this->m_child[1] = node;
                    --this->m_stairs[1];
                }
            }

            if (this->get_balance() == 2)
            {
                return this->rotate(CCW);
            }
            else if (this->get_balance() == -2)
            {
                return this->rotate(CW);
            }

            ASSERT(lol::abs(this->m_stairs[1] - this->m_stairs[0]) < 3);
            return 0;
        }

        enum Rotation { CW = 0, CCW = 1 };

        tree_node * rotate(Rotation rotation)
        {
            if (rotation == CW)
            {
                tree_node * newhead = this->m_child[0];
                tree_node * tmp = this->m_child[0]->m_child[1];

                this->m_child[0]->m_child[1] = this;
                this->m_child[0] = tmp;

                this->compute_balance();
                newhead->compute_balance();

                return newhead;
            }
            else // rotation == CCW
            {
                tree_node * newhead = this->m_child[1];
                tree_node * tmp = this->m_child[1]->m_child[0];

                this->m_child[1]->m_child[0] = this;
                this->m_child[1] = tmp;

                this->compute_balance();
                newhead->compute_balance();

                return newhead;
            }

            return 0;
        }

        void compute_balance()
        {
            this->m_stairs[0] = this->m_child[0] ? lol::max(this->m_child[0]->m_stairs[0], this->m_child[0]->m_stairs[1]) + 1 : 0;
            this->m_stairs[1] = this->m_child[1] ? lol::max(this->m_child[1]->m_stairs[0], this->m_child[1]->m_stairs[1]) + 1 : 0;
        }

        int get_balance()
        {
            return this->m_stairs[1] - this->m_stairs[0];
        }

    protected:

        K m_key;
        V m_value;

        tree_node *m_child[2];
        int m_stairs[2];
    };

    tree_node * m_root;
};

}
