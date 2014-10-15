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
        bool created = false;
        if (!m_root)
        {

            this->m_root = this->m_allocator.allocate(key, value, &this->m_root);
            created = true;
        }

        if (!created)
            created = this->m_root->insert(key, value);

        if (this->m_allocator.check_reallocation())
            this->m_allocator.repair_root(&this->m_root);

        return created;
    }

    bool erase(K const & key)
    {
        if (!m_root)
            return false;

        return this->m_root->erase(key);
    }

    bool exists(K const & key)
    {
        if (!m_root)
            return false;

        return this->m_root->exists(key);
    }

    bool try_get_value(K const & key, V * & value_ptr)
    {
        if (this->m_root)
            return this->m_root->try_get_value(key, value_ptr);

        return false;
    }

protected:

    class tree_node
    {
    public:
        tree_node() :
            m_parent_slot(nullptr)
        {
            m_child[0] = m_child[1] = nullptr;
            m_stairs[0] = m_stairs[1] = 0;
        }

        tree_node(K key, V value, tree_node ** parent_slot) :
            m_key(key),
            m_value(value),
            m_parent_slot(parent_slot)
        {
            m_child[0] = m_child[1] = nullptr;
            m_stairs[0] = m_stairs[1] = 0;
        }

        /* Insert a value in tree and return true or update an existing value for
         * the existing key and return false */
        bool insert(K const & key, V const & value)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                this->m_value = value;
                return false;
            }

            bool created = false;

            if (this->m_child[i])
                created = this->m_child[i]->insert(key, value);
            else
            {
                this->m_child[i] = this->m_allocator->allocate(key, value, &this->m_child[i]);
                created = true;
            }

            if (created)
                this->rebalance_if_needed();

            return created;
        }

        /* Erase a value in tree and return true or return false */
        bool erase(K const & key)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                this->erase_self();
                this->m_allocator->deallocate(this);
                return true;
            }
            else if(this->m_child[i]->erase(key))
            {
                this->rebalance_if_needed();
                return true;
            }

            return false;
        }

        bool try_get_value(K const & key, V * & value_ptr)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                value_ptr = &this->m_value;
                return true;
            }

            if (this->m_child[i])
                return this->m_child[i]->try_get_value(key, value_ptr);

            return false;
        }

        void update_balance()
        {
            this->m_stairs[0] = this->m_child[0] ? lol::max(this->m_child[0]->m_stairs[0], this->m_child[0]->m_stairs[1]) + 1 : 0;
            this->m_stairs[1] = this->m_child[1] ? lol::max(this->m_child[1]->m_stairs[0], this->m_child[1]->m_stairs[1]) + 1 : 0;
        }

        void rebalance_if_needed()
        {
            this->update_balance();

            int i = (this->get_balance() ==  2);
            int j = (this->get_balance() == -2);

            if (i || j)
            {
                tree_node * save = this->m_child[i];
                tree_node ** save_parent = this->m_parent_slot;

                this->set_child(i, save->m_child[j]);
                save->set_child(j, this);

                save->m_parent_slot = save_parent;
                *save_parent = save;

                this->update_balance();
                save->update_balance();
            }
        }

        void set_child(int i, tree_node * node)
        {
            this->m_child[i] = node;
            if (node)
                node->m_parent_slot = &this->m_child[i];
        }

        bool exists(K const & key)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
                return true;

            if (this->m_child[i])
                return this->m_child[i]->exists(key);

            return false;
        }

        int get_balance()
        {
            return this->m_stairs[1] - this->m_stairs[0];
        }

        K get_key()
        {
            return this->m_key;
        }

        class tree_node_allocator
        {
        public:

            tree_node_allocator() :
                m_current(0),
                m_size(1),
                m_allocated(0)
            {
                m_buffers[0] = new tree_node[1];
                m_buffers[1] = nullptr;
            }

            ~tree_node_allocator()
            {
                delete[] this->m_buffers[this->m_current];
            }

            tree_node * allocate(K const & key, V const & value, tree_node ** parent_slot)
            {
                tree_node * new_node = &this->m_buffers[this->m_current][this->m_allocated++];

                new_node->m_key = key;
                new_node->m_value = value;
                new_node->m_parent_slot = parent_slot;
                new_node->m_allocator = this;

                return new_node;
            }

            bool check_reallocation()
            {
                if (this->m_allocated == this->m_size)
                {
                    int next = this->m_current ? 0 : 1;
                    size_t new_size = this->m_size * 2;

                    this->m_buffers[next] = new tree_node[new_size];

                    for (int i = 0 ; i < this->m_size ; ++i)
                        this->m_buffers[next][i] = this->m_buffers[this->m_current][i];

                    // Fix new links between child and parents and the new_array
                    for (int i = 0 ; i < this->m_size ; ++i)
                    {
                        for (int j = 0 ; j < 2 ; ++j)
                        {
                            if (this->m_buffers[this->m_current][i].m_child[j])
                            {
                                ptrdiff_t position0 = this->m_buffers[this->m_current][i].m_child[j] - this->m_buffers[this->m_current];
                                this->m_buffers[next][i].m_child[j] = &this->m_buffers[next][position0];
                                this->m_buffers[next][position0].m_parent_slot = &this->m_buffers[next][i].m_child[j];
                            }
                        }
                    }

                    delete[] this->m_buffers[this->m_current];
                    this->m_buffers[this->m_current] = nullptr;
                    this->m_current = next;
                    this->m_size = new_size;

                    return true;
                }

                return false;
            }

            void deallocate(tree_node * replace)
            {
                *replace = this->m_buffers[m_current][this->m_allocated - 1];
                if (replace->m_parent_slot)
                    *replace->m_parent_slot = replace;

                this->m_buffers[m_current][this->m_allocated - 1] = tree_node();

                for (int i = 0 ; i < 2 ; ++i)
                    if (replace->m_child[i])
                        replace->m_child[i]->m_parent_slot = &replace->m_child[i];

                --this->m_allocated;
            }

            void repair_root(tree_node ** root)
            {
                bool root_found = false;

                for (int i = 0 ; i < this->m_allocated ; ++i)
                {
                    if (this->m_buffers[this->m_current][i].m_parent_slot == root)
                    {
                        *root = &this->m_buffers[this->m_current][i];
                        root_found = true;
                    }
                }

                ASSERT(root_found); // Something went really bad
            }

        private:

            tree_node * m_buffers[2];
            int m_current;

            size_t m_size;
            ptrdiff_t m_allocated;
        };

    protected:

        void erase_self()
        {
            int i = (this->get_balance() == -1);

            tree_node * replacement = this->m_child[1 - i];

            if (replacement)
            {
                while (replacement->m_child[i])
                    replacement = replacement->m_child[i];
            }
            if (replacement)
            {
                *replacement->m_parent_slot = replacement->m_child[1 - i];
                if (*replacement->m_parent_slot)
                    (*replacement->m_parent_slot)->rebalance_if_needed();

                replacement->m_parent_slot = this->m_parent_slot;
                *replacement->m_parent_slot = replacement;

                replacement->m_child[0] = this->m_child[0];
                replacement->m_child[1] = this->m_child[1];
            }
            else
                *this->m_parent_slot = nullptr;

            this->m_parent_slot = nullptr;
            this->m_child[0] = nullptr;
            this->m_child[1] = nullptr;
        }

        K m_key;
        V m_value;

        tree_node *m_child[2];
        int m_stairs[2];

        tree_node ** m_parent_slot;
        tree_node_allocator * m_allocator;
    };

    tree_node * m_root;
    typename tree_node::tree_node_allocator m_allocator;
};

}
