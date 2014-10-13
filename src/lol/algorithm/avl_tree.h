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
        else if (this->m_root->exists(key))
        {
            this->m_root->insert_or_update(key, value);
            return false;
        }
        else
        {
            this->m_root->insert_or_update(key, value);
            this->rebalance_if_needed(key);
        }

        return true;
    }

    bool erase(K const & key)
    {
        if (this->m_root)
        {
            tree_node * parent = this->m_root->get_parent(key);

            if (parent)
            {
                parent->erase_child(key);
                this->rebalance_if_needed(key);
            }
            else if (this->m_root->key_equals(key))
            {
                this->m_root = this->m_root->erase_self();
                this->rebalance_if_needed(key);
            }

            return true;
        }

        return false;
    }

    void rebalance_if_needed(K const & key)
    {
        this->m_root->update_balance(key);
        tree_node * node = this->m_root->get_unbalanced_parent(key);

        if (node)
        {
            node->rebalance_children(key);
            this->m_root->update_balance(node);
        }
        else if (this->m_root->get_balance() == -2)
            this->m_root = this->m_root->rotate(tree_node::CW);
        else if (this->m_root->get_balance() == 2)
            this->m_root = this->m_root->rotate(tree_node::CCW);
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

        bool exists(K key)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
                return true;

            if (this->m_child[i])
                return this->m_child[i]->exists(key);

            return false;
        }

        void insert_or_update(K const & key, V const & value)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                this->m_value = value;
            }
            else
            {
                if (this->m_child[i])
                    this->m_child[i]->insert_or_update(key, value);
                else
                    this->m_child[i] = new tree_node(key, value);
            }
        }

        void update_balance(tree_node * node)
        {
            this->update_balance(node->m_key);
        }

        void update_balance(K const & key)
        {
            if (key < this->m_key && this->m_child[0])
                this->m_child[0]->update_balance(key);
            if (this->m_key < key && this->m_child[1])
                this->m_child[1]->update_balance(key);

            this->compute_balance();
        }

        /* Retrieve the parent of the deeper unbalanced node after key insertion.
         * Do not call "get_unbalanced_parent" if key is not already present in the tree. */
        tree_node * get_unbalanced_parent(K const & key)
        {
            tree_node * parent = nullptr;

            if (key < this->m_key && this->m_child[0])
            {
                parent = this->m_child[0]->get_unbalanced_parent(key);

                if (parent)
                    return parent;
                else if (abs(this->m_child[0]->get_balance()) == 2)
                    return this;
            }
            if (this->m_key < key && this->m_child[1])
            {
                parent = this->m_child[1]->get_unbalanced_parent(key);

                if (parent)
                    return parent;
                else if (abs(this->m_child[1]->get_balance()) == 2)
                    return this;
            }

            return nullptr;
        }

        void rebalance_children(K const & key)
        {
            if (key < this->m_key)
            {
                if (this->m_child[0]->get_balance() == 2)
                    this->m_child[0] = this->m_child[0]->rotate(CCW);
                else if (this->m_child[0]->get_balance() == -2)
                    this->m_child[0] = this->m_child[0]->rotate(CW);
            }
            else if (this->m_key < key)
            {
                if (this->m_child[1]->get_balance() == 2)
                    this->m_child[1] = this->m_child[1]->rotate(CCW);
                else if (this->m_child[1]->get_balance() == -2)
                    this->m_child[1] = this->m_child[1]->rotate(CW);
            }
            else
                ASSERT(false) // Do not rebalance the "this" node here
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

        bool key_equals(K const & key)
        {
            return !(key < this->m_key) && !(this->m_key < key);
        }

        /* Retrieve the parent of a key.
         * Do not call "get_parent" if you’re not sure the key or node is present. */
        tree_node * get_parent(tree_node * node)
        {
            return this->get_parent(node->m_key);
        }

        /* Retrieve the parent of an inserted key.
         * Do not call "get_parent" if key is not already present in the tree. */
        tree_node * get_parent(K const & key)
        {
            if (key < this->m_key)
            {
                if (this->m_child[0]->key_equals(key))
                    return this;
                else
                    return this->m_child[0]->get_parent(key);
            }
            else if (this->m_key < key)
            {
                if (this->m_child[1]->key_equals(key))
                    return this;
                else
                    return this->m_child[1]->get_parent(key);
            }

            return nullptr;
        }

        void erase_child(K const & key)
        {
            tree_node * erase_me = nullptr;

            if (key < this->m_key)
            {
                erase_me = this->m_child[0];
                this->m_child[0] = erase_me->erase_self();
            }
            else if (this->m_key < key)
            {
                erase_me = this->m_child[1];
                this->m_child[1] = erase_me->erase_self();
            }
            else
                ASSERT(false) // Do not erase the "this" node here

            delete erase_me;
        }

        tree_node * erase_self()
        {
            tree_node * replacement = nullptr;

            if (this->get_balance() == -1)
            {
                replacement = this->get_deeper(0);
                if (replacement)
                    this->get_parent(replacement)->m_child[1] = replacement->m_child[0];
            }
            else // this->get_balance() >= 0
            {
                replacement = this->get_deeper(1);
                if (replacement)
                    this->get_parent(replacement)->m_child[0] = replacement->m_child[1];
            }

            if (replacement)
            {
                replacement->m_child[0] = this->m_child[0];
                replacement->m_child[1] = this->m_child[1];
            }

            return replacement;
        }

        void replace(tree_node * from, tree_node * to)
        {
            from->m_child[0] = to->m_child[0];
            from->m_child[1] = to->m_child[1];

            to->m_child[0] = nullptr;
            to->m_child[1] = nullptr;
        }

        tree_node * get_deeper(int index)
        {
            tree_node * previous = this->m_child[index];

            if (previous)
            {
                while (previous->m_child[1 - index])
                {
                    previous = previous->m_child[1 - index];
                }
            }

            return previous;
        }

        int get_balance()
        {
            return this->m_stairs[1] - this->m_stairs[0];
        }

        K get_key()
        {
            return this->m_key;
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
