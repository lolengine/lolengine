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
        m_root(0)
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
            m_value(value),
            m_lo(0),
            m_hi(0),
            m_stairs_lo(0),
            m_stairs_hi(0)
        {
        }

        tree_node * insert(K const & key, V const & value)
        {
            tree_node * ret = 0;

            if (key < this->m_key)
            {
                if (this->m_lo)
                    ret = this->m_lo->insert(key, value);
                else
                    ret = this->m_lo = new tree_node(key, value);
            }
            else if (this->m_key < key)
            {
                if (this->m_hi)
                    ret = this->m_hi->insert(key, value);
                else
                    ret = this->m_hi = new tree_node(key, value);
            }
            else
                this->m_value = value;

            return ret;
        }

        int path_update_balance(K const & key)
        {
            if (key < this->m_key)
                this->m_stairs_lo = lol::max(this->m_lo->path_update_balance(key), this->m_stairs_lo);
            else if (this->m_key < key)
                this->m_stairs_hi = lol::max(this->m_hi->path_update_balance(key), this->m_stairs_hi);

            return lol::max(this->m_stairs_lo, this->m_stairs_hi) + 1;
        }

        tree_node * path_rebalance(K const & key)
        {
            if (key < this->m_key)
            {
                tree_node * node = this->m_lo->path_rebalance(key);
                if (node)
                {
                    this->m_lo = node;
                    --this->m_stairs_lo;
                }
            }
            else if (this->m_key < key)
            {
                tree_node * node = this->m_hi->path_rebalance(key);
                if (node)
                {
                    this->m_hi = node;
                    --this->m_stairs_hi;
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
            else
                ASSERT(lol::abs(this->m_stairs_hi - this->m_stairs_lo) < 3);

            return 0;
        }

        enum Rotation { CW = 0, CCW = 1 };

        tree_node * rotate(Rotation rotation)
        {
            if (rotation == CW)
            {
                tree_node * lo = this->m_lo;
                tree_node * lo_hi = this->m_lo->m_hi;

                this->m_lo->m_hi = this;
                this->m_lo = lo_hi;

                this->compute_balance();
                lo->compute_balance();

                return lo;
            }
            else // rotation == CCW
            {
                tree_node * hi = this->m_hi;
                tree_node * hi_lo = this->m_hi->m_lo;

                this->m_hi->m_lo = this;
                this->m_hi = hi_lo;

                this->compute_balance();
                hi->compute_balance();

                return hi;
            }

            return 0;
        }

        void compute_balance()
        {
            this->m_stairs_lo = this->m_lo ? this->m_lo->m_stairs_lo + this->m_lo->m_stairs_hi + 1 : 0;
            this->m_stairs_hi = this->m_hi ? this->m_hi->m_stairs_lo + this->m_hi->m_stairs_hi + 1 : 0;
        }

        int get_balance()
        {
            return this->m_stairs_hi - this->m_stairs_lo;
        }

    protected:

        K m_key;
        V m_value;

        tree_node * m_lo;
        tree_node * m_hi;

        int m_stairs_lo;
        int m_stairs_hi;
    };

    tree_node * m_root;
};

}
