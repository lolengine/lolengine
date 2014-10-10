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

        bool exists(K key)
        {
            if (key < this->m_key)
            {
                if (this->m_child[0])
                    return this->m_child[0]->exists(key);
                else
                    return false;
            }
            if (this->m_key < key)
            {
                if (this->m_child[1])
                    return this->m_child[1]->exists(key);
                else
                    return false;
            }

            return true;
        }

        void insert_or_update(K const & key, V const & value)
        {
            if (key < this->m_key)
            {
                if (this->m_child[0])
                    this->m_child[0]->insert_or_update(key, value);
                else
                    this->m_child[0] = new tree_node(key, value);
            }
            else if (this->m_key < key)
            {
                if (this->m_child[1])
                    this->m_child[1]->insert_or_update(key, value);
                else
                    this->m_child[1] = new tree_node(key, value);
            }
            else
                this->m_value = value;
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

        /* Increases stairs according that key is inserted.
         * Do not call "increase_path" if key is not already present in the tree. */
        void increase_path(K key)
        {
            if (key < this->m_key)
            {
                this->m_child[0]->increase_path(key);
            }
            if (this->m_key < key)
            {
                this->m_child[1]->increase_path(key);
            }
        }

        /* Retrieve the parent of the deeper unbalanced node after key insertion.
         * Do not call "get_unbalanced_parent" if key is not already present in the tree. */
        tree_node * get_unbalanced_parent(K const & key)
        {
            tree_node * parent = nullptr;

            if (key < this->m_key)
            {
                parent = this->m_child[0]->get_unbalanced_parent(key);

                if (parent)
                    return parent;
                else if (abs(this->m_child[0]->get_balance()) == 2)
                    return this;
            }
            if (this->m_key < key)
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
                    this->rotateLL();
                if (this->m_child[0]->get_balance() == -2)
                    this->rotateLR();
            }
            else if (this->m_key < key)
            {
                if (this->m_child[1]->get_balance() == 2)
                    this->rotateRL();
                if (this->m_child[1]->get_balance() == -2)
                    this->rotateRR();
            }
            else
                ASSERT(false) // Do not rebalance the "this" node here
        }

        void rotateLL()
        {
            tree_node * newhead = this->m_child[0]->rotate(CCW);
            this->m_child[0] = newhead;
        }

        void rotateLR()
        {
            tree_node * newhead = this->m_child[0]->rotate(CW);
            this->m_child[0] = newhead;
        }

        void rotateRL()
        {
            tree_node * newhead = this->m_child[1]->rotate(CCW);
            this->m_child[1] = newhead;
        }

        void rotateRR()
        {
            tree_node * newhead = this->m_child[1]->rotate(CW);
            this->m_child[1] = newhead;
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
