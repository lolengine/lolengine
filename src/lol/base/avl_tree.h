//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//            © 2013-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//            © 2013-2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

namespace lol
{

#include <lol/base/all.h>

template<typename K, typename V>
class avl_tree
{
public:
    avl_tree() :
        m_root(nullptr),
        m_count(0)
    {
    }

    avl_tree(avl_tree const & other) :
        m_root(nullptr),
        m_count(0)
    {
        for (auto it : other)
            insert(it.key, it.value);
    }

    avl_tree & operator=(avl_tree const & other)
    {
        if (&other != this)
        {
            clear();

            for (auto it : other)
                insert(it.key, it.value);
        }

        return *this;
    }

    ~avl_tree()
    {
        clear();
    }

    bool insert(K const & key, V const & value)
    {
        if (!m_root)
        {
            m_root = new tree_node(key, value, &m_root);
            ++m_count;
            return true;
        }

        if (m_root->insert(key, value))
        {
            ++m_count;
            return true;
        }

        return false;
    }

    bool erase(K const & key)
    {
        if (!m_root)
            return false;

        if (m_root->erase(key))
        {
            --m_count;
            return true;
        }

        return false;
    }

    bool exists(K const & key)
    {
        if (!m_root)
            return false;

        return m_root->exists(key);
    }

    void clear()
    {
        if (m_root)
        {
            tree_node * node = nullptr;
            m_root->get_min(node);

            while (node)
            {
                tree_node * next = node->get_next();
                delete node;
                node = next;
            }
        }

        m_root = nullptr;
        m_count = 0;
    }

    bool try_get(K const & key, V * & value_ptr) const
    {
        if (m_root)
            return m_root->try_get(key, value_ptr);

        return false;
    }

    bool try_get_min(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * min_node = nullptr;

        if (m_root)
        {
            m_root->get_min(min_node);
            key_ptr = &min_node->get_key();
            value_ptr = &min_node->get_value();

            return true;
        }

        return false;
    }

    bool try_get_max(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * max_node = nullptr;

        if (m_root)
        {
            m_root->get_max(max_node);
            key_ptr = &max_node->get_key();
            value_ptr = &max_node->get_value();

            return true;
        }

        return false;
    }

    class iterator;
    class const_iterator;

    iterator begin()
    {
        tree_node * node = nullptr;

        if (m_root)
            m_root->get_min(node);

        return iterator(node);
    }

    const_iterator begin() const
    {
        tree_node * node = nullptr;

        if (m_root)
            m_root->get_min(node);

        return const_iterator(node);
    }

    int count() const
    {
        return m_count;
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

protected:

    class tree_node
    {
    public:
        tree_node(K key, V value, tree_node ** parent_slot) :
            m_key(key),
            m_value(value),
            m_parent_slot(parent_slot)
        {
            m_child[0] = m_child[1] = nullptr;
            m_stairs[0] = m_stairs[1] = 0;
            m_chain[0] = m_chain[1] = nullptr;
        }

        K const & get_key()
        {
            return m_key;
        }

        V & get_value()
        {
            return m_value;
        }

        /* Insert a value in tree and return true or update an existing value for
         * the existing key and return false */
        bool insert(K const & key, V const & value)
        {
            int i = -1 + (key < m_key) + 2 * (m_key < key);

            bool created = false;

            if (i < 0)
                m_value = value;
            else if (m_child[i])
                created = m_child[i]->insert(key, value);
            else
            {
                created = true;

                m_child[i] = new tree_node(key, value, &m_child[i]);

                m_child[i]->m_chain[i] = m_chain[i];
                m_child[i]->m_chain[i ? 0 : 1] = this;

                if (m_chain[i])
                    m_chain[i]->m_chain[i ? 0 : 1] = m_child[i];
                m_chain[i] = m_child[i];
            }

            if (created)
            {
                rebalance_if_needed();
            }


            return created;
        }

        /* Erase a value in tree and return true or return false */
        bool erase(K const & key)
        {
            int i = -1 + (key < m_key) + 2 * (m_key < key);

            bool erased = false;

            if (i < 0)
            {
                erase_self();
                delete this;
                erased = true;
            }
            else if (m_child[i] && m_child[i]->erase(key))
            {
                rebalance_if_needed();
                erased = true;
            }

            return erased;
        }

        bool try_get(K const & key, V * & value_ptr)
        {
            int i = -1 + (key < m_key) + 2 * (m_key < key);

            if (i < 0)
            {
                value_ptr = &m_value;
                return true;
            }

            if (m_child[i])
                return m_child[i]->try_get(key, value_ptr);

            return false;
        }

        bool exists(K const & key)
        {
            int i = -1 + (key < m_key) + 2 * (m_key < key);

            if (i < 0)
                return true;

            if (m_child[i])
                return m_child[i]->exists(key);

            return false;
        }

        void get_min(tree_node * & min_node)
        {
            min_node = this;

            while (min_node->m_child[0])
                min_node = min_node->m_child[0];
        }

        void get_max(tree_node * & max_node) const
        {
            max_node = this;

            while (max_node->m_child[1])
                max_node = max_node->m_child[1];
        }

        int get_balance() const
        {
            return m_stairs[1] - m_stairs[0];
        }

        tree_node * get_previous() const
        {
            return m_chain[0];
        }

        tree_node * get_next() const
        {
            return m_chain[1];
        }

    protected:

        void update_balance()
        {
            m_stairs[0] = m_child[0] ? (m_child[0]->m_stairs[0] > m_child[0]->m_stairs[1] ? m_child[0]->m_stairs[0] : m_child[0]->m_stairs[1]) + 1 : 0;
            m_stairs[1] = m_child[1] ? (m_child[1]->m_stairs[0] > m_child[1]->m_stairs[1] ? m_child[1]->m_stairs[0] : m_child[1]->m_stairs[1]) + 1 : 0;
        }

        void rebalance_if_needed()
        {
            update_balance();

            int i = -1 + (get_balance() == -2) + 2 * (get_balance() == 2);

            if (i != -1)
            {
                tree_node * replacement = nullptr;


                if (get_balance() / 2 + m_child[i]->get_balance() == 0)
                {
                    replacement = m_child[i]->m_child[1 - i];
                    tree_node * save0 = replacement->m_child[i];
                    tree_node * save1 = replacement->m_child[1 - i];

                    replacement->m_parent_slot = this->m_parent_slot;
                    *replacement->m_parent_slot = replacement;

                    replacement->m_child[i] = m_child[i];
                    m_child[i]->m_parent_slot = &replacement->m_child[i];

                    replacement->m_child[1 - i] = this;
                    this->m_parent_slot = &replacement->m_child[1 - i];

                    replacement->m_child[i]->m_child[1 - i] = save0;
                    if (save0)
                        save0->m_parent_slot = &replacement->m_child[i]->m_child[1 - i];

                    replacement->m_child[1 - i]->m_child[i] = save1;
                    if (save1)
                        save1->m_parent_slot = &replacement->m_child[i]->m_child[1 - i];
                }
                else
                {

                    replacement = m_child[i];
                    tree_node * save = replacement->m_child[1 - i];

                    replacement->m_parent_slot = this->m_parent_slot;
                    *replacement->m_parent_slot = replacement;

                    replacement->m_child[1 - i] = this;
                    this->m_parent_slot = &replacement->m_child[1 - i];

                    this->m_child[i] = save;
                    if (save)
                        save->m_parent_slot = &this->m_child[i];
                }

                replacement->m_child[0]->update_balance();
                replacement->m_child[1]->update_balance();
                replacement->update_balance();
            }
        }

        void erase_self()
        {
            int i = (get_balance() == -1);

            tree_node * replacement = m_child[1 - i];

            if (replacement)
            {
                while (replacement->m_child[i])
                    replacement = replacement->m_child[i];
            }

            if (replacement)
            {
                *replacement->m_parent_slot = replacement->m_child[1 - i];

                replacement->m_parent_slot = m_parent_slot;
                *replacement->m_parent_slot = replacement;

                replacement->m_child[0] = m_child[0];
                if (replacement->m_child[0])
                    replacement->m_child[0]->m_parent_slot = &replacement->m_child[0];

                replacement->m_child[1] = m_child[1];
                if (replacement->m_child[1])
                    replacement->m_child[1]->m_parent_slot = &replacement->m_child[1];

                if (replacement->m_child[1-i])
                    replacement->m_child[1-i]->deep_balance(replacement->m_key);

                replacement->update_balance();
            }
            else
                *m_parent_slot = nullptr;

            replace_chain(replacement);
        }

        void deep_balance(K const & key)
        {
            int i = -1 + (key < m_key) + 2 * (m_key < key);

            if (i != -1 && m_child[i])
                m_child[i]->deep_balance(key);

            update_balance();
        }

        void replace_chain(tree_node * replacement)
        {
            if (replacement)
            {
                if (replacement->m_chain[0])
                    replacement->m_chain[0]->m_chain[1] = replacement->m_chain[1];

                if (replacement->m_chain[1])
                    replacement->m_chain[1]->m_chain[0] = replacement->m_chain[0];

                replacement->m_chain[0] = m_chain[0];
                replacement->m_chain[1] = m_chain[1];

                if (replacement->m_chain[0])
                    replacement->m_chain[0]->m_chain[1] = replacement;
                if (replacement->m_chain[1])
                    replacement->m_chain[1]->m_chain[0] = replacement;
            }
            else
            {
                if (m_chain[0])
                    m_chain[0]->m_chain[1] = m_chain[1];
                if (m_chain[1])
                    m_chain[1]->m_chain[0] = m_chain[0];
            }
        }

        K m_key;
        V m_value;

        tree_node *m_child[2];
        int m_stairs[2];

        tree_node ** m_parent_slot;

        tree_node * m_chain[2]; // Linked list used to keep order between nodes
    };

public:

    /* Iterators related */

    struct output_value
    {
        output_value(K const & _key, V & _value) :
            key(_key),
            value(_value)
        {
        }

        K const & key;
        V & value;
    };

    class iterator
    {
    public:

        iterator(tree_node * node) :
            m_node(node)
        {
        }

        iterator & operator++(int)
        {
            m_node = m_node->get_next();

            return *this;
        }

        iterator & operator--(int)
        {
            m_node = m_node->get_previous();

            return *this;
        }

        iterator operator++()
        {
            tree_node * ret = m_node;
            m_node = m_node->get_next();

            return iterator(ret);
        }

        iterator operator--()
        {
            tree_node * ret = m_node;
            m_node = m_node->get_previous();

            return iterator(ret);
        }

        output_value operator*()
        {
            return output_value(m_node->get_key(), m_node->get_value());
        }

        bool operator!=(iterator const & that) const
        {
            return m_node != that.m_node;
        }

    protected:

        tree_node * m_node;
    };

    struct const_output_value
    {
        const_output_value(K const & _key, V const & _value) :
            key(_key),
            value(_value)
        {
        }

        K const & key;
        V const & value;
    };

    class const_iterator
    {
    public:

        const_iterator(tree_node * node) :
            m_node(node)
        {
        }

        const_iterator & operator++(int)
        {
            m_node = m_node->get_next();

            return *this;
        }

        const_iterator & operator--(int)
        {
            m_node = m_node->get_previous();

            return *this;
        }

        const_iterator operator++()
        {
            tree_node * ret = m_node;
            m_node = m_node->get_next();

            return const_iterator(ret);
        }

        const_iterator operator--()
        {
            tree_node * ret = m_node;
            m_node = m_node->get_previous();

            return const_iterator(ret);
        }

        const_output_value operator*()
        {
            return const_output_value(m_node->get_key(), m_node->get_value());
        }

        bool operator!=(const_iterator const & that) const
        {
            return m_node != that.m_node;
        }

    protected:

        tree_node * m_node;
    };

protected:

    tree_node * m_root;

    int m_count;
};

}

