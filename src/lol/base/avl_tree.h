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
            this->insert(it.key, it.value);
    }

    avl_tree & operator=(avl_tree const & other)
    {
        this->clear();

        for (auto it : other)
            this->insert(it.key, it.value);

        return *this;
    }

    ~avl_tree()
    {
        this->clear();
    }

    bool insert(K const & key, V const & value)
    {
        if (!m_root)
        {
            this->m_root = new tree_node(key, value, &this->m_root);
            ++this->m_count;
            return true;
        }

        if(this->m_root->insert(key, value))
        {
            ++this->m_count;
            return true;
        }

        return false;
    }

    bool erase(K const & key)
    {
        if (!m_root)
            return false;

        if(this->m_root->erase(key))
        {
            --this->m_count;
            return true;
        }

        return false;
    }

    bool exists(K const & key)
    {
        if (!m_root)
            return false;

        return this->m_root->exists(key);
    }

    void clear()
    {
        if (this->m_root)
        {
            tree_node * node = nullptr;
            this->m_root->GetMin(node);

            while (node)
            {
                tree_node * next = node->GetNext();
                delete node;
                node = next;
            }
        }

        this->m_root = nullptr;
        this->m_count = 0;
    }

    bool try_get(K const & key, V * & value_ptr) const
    {
        if (this->m_root)
            return this->m_root->try_get(key, value_ptr);

        return false;
    }

    bool try_get_min(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * min_node = nullptr;

        if (this->m_root)
        {
            this->m_root->GetMin(min_node);
            key_ptr = &min_node->GetKey();
            value_ptr = &min_node->GetValue();

            return true;
        }

        return false;
    }

    bool try_get_max(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * max_node = nullptr;

        if (this->m_root)
        {
            this->m_root->GetMax(max_node);
            key_ptr = &max_node->GetKey();
            value_ptr = &max_node->GetValue();

            return true;
        }

        return false;
    }

    class iterator;
    class const_iterator;

    iterator begin()
    {
        tree_node * node = nullptr;

        if (this->m_root)
            this->m_root->GetMin(node);

        return iterator(node);
    }

    const_iterator begin() const
    {
        tree_node * node = nullptr;

        if (this->m_root)
            this->m_root->GetMin(node);

        return const_iterator(node);
    }

    int GetCount() const
    {
        return this->m_count;
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

        K const & GetKey()
        {
            return this->m_key;
        }

        V & GetValue()
        {
            return this->m_value;
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

            bool b_created = false;

            if (this->m_child[i])
                b_created = this->m_child[i]->insert(key, value);
            else
            {
                b_created = true;

                this->m_child[i] = new tree_node(key, value, &this->m_child[i]);

                this->m_child[i]->m_chain[i] = this->m_chain[i];
                this->m_child[i]->m_chain[i ? 0 : 1] = this;

                if (this->m_chain[i])
                    this->m_chain[i]->m_chain[i ? 0 : 1] = this->m_child[i];
                this->m_chain[i] = this->m_child[i];
            }

            if (b_created)
                this->RebalanceIfNeeded();

            return b_created;
        }

        /* Erase a value in tree and return true or return false */
        bool erase(K const & key)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                this->EraseSelf();
                delete this;
                return true;
            }
            else if(this->m_child[i]->erase(key))
            {
                this->RebalanceIfNeeded();
                return true;
            }

            return false;
        }

        bool try_get(K const & key, V * & value_ptr)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
            {
                value_ptr = &this->m_value;
                return true;
            }

            if (this->m_child[i])
                return this->m_child[i]->try_get(key, value_ptr);

            return false;
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

        void GetMin(tree_node * & min_node)
        {
            min_node = this;

            while (min_node->m_child[0])
                min_node = min_node->m_child[0];
        }

        void GetMax(tree_node * & max_node) const
        {
            max_node = this;

            while (max_node->m_child[1])
                max_node = max_node->m_child[1];
        }

        int GetBalance() const
        {
            return this->m_stairs[1] - this->m_stairs[0];
        }

        tree_node * GetPrevious() const
        {
            return this->m_chain[0];
        }

        tree_node * GetNext() const
        {
            return this->m_chain[1];
        }

    protected:

        void UpdateBalance()
        {
            this->m_stairs[0] = this->m_child[0] ? (this->m_child[0]->m_stairs[0] > this->m_child[0]->m_stairs[1] ? this->m_child[0]->m_stairs[0] : this->m_child[0]->m_stairs[1]) + 1 : 0;
            this->m_stairs[1] = this->m_child[1] ? (this->m_child[1]->m_stairs[0] > this->m_child[1]->m_stairs[1] ? this->m_child[1]->m_stairs[0] : this->m_child[1]->m_stairs[1]) + 1 : 0;
        }

        void RebalanceIfNeeded()
        {
            this->UpdateBalance();

            int i = (this->GetBalance() ==  2);
            int j = (this->GetBalance() == -2);

            if (i || j)
            {
                tree_node * save = this->m_child[i];
                tree_node ** save_parent = this->m_parent_slot;

                this->SetChild(i, save->m_child[j]);
                save->SetChild(j, this);

                save->m_parent_slot = save_parent;
                *save_parent = save;

                this->UpdateBalance();
                save->UpdateBalance();
            }
        }

        void SetChild(int i, tree_node * node)
        {
            this->m_child[i] = node;
            if (node)
                node->m_parent_slot = &this->m_child[i];
        }

        void EraseSelf()
        {
            int i = (this->GetBalance() == -1);

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
                    (*replacement->m_parent_slot)->RebalanceIfNeeded();

                replacement->m_parent_slot = this->m_parent_slot;
                *replacement->m_parent_slot = replacement;

                replacement->m_child[0] = this->m_child[0];
                replacement->m_child[1] = this->m_child[1];
            }
            else
                *this->m_parent_slot = nullptr;

            this->replace_chain(replacement);

            this->m_parent_slot = nullptr;
            this->m_child[0] = nullptr;
            this->m_child[1] = nullptr;
        }

        void replace_chain(tree_node * replacement)
        {
            for (int i = 0 ; i < 2 ; ++i)
            {
                if (replacement)
                {
                    if (replacement->m_chain[i])
                        replacement->m_chain[i]->m_chain[i ? 0 : 1] = replacement->m_chain[i ? 0 : 1];

                    replacement->m_chain[i] = this->m_chain[i];
                    if (replacement->m_chain[i])
                        replacement->m_chain[i]->m_chain[i ? 0 : 1] = replacement;
                }
                else
                {
                    if (this->m_chain[i])
                        this->m_chain[i]->m_chain[i ? 0 : 1] = this->m_chain[i ? 0 : 1];
                }

                this->m_chain[i] = nullptr;
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

    struct OutputValue
    {
        OutputValue(K const & _key, V & _value) :
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
            this->m_node = this->m_node->get_next();

            return *this;
        }

        iterator & operator--(int)
        {
            this->m_node = this->m_node->get_previous();

            return *this;
        }

        iterator operator++()
        {
            tree_node * ret = this->m_node;
            this->m_node = this->m_node->GetNext();

            return iterator(ret);
        }

        iterator operator--()
        {
            tree_node * ret = this->m_node;
            this->m_node = this->m_node->GetPrevious();

            return iterator(ret);
        }

        OutputValue operator*()
        {
            return OutputValue(this->m_node->GetKey(), this->m_node->GetValue());
        }

        bool operator!=(iterator const & that) const
        {
            return this->m_node != that.m_node;
        }

    protected:

        tree_node * m_node;
    };

    struct ConstOutputValue
    {
        ConstOutputValue(K const & _key, V const & _value) :
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
            this->m_node = this->m_node->get_next();

            return *this;
        }

        const_iterator & operator--(int)
        {
            this->m_node = this->m_node->get_previous();

            return *this;
        }

        const_iterator operator++()
        {
            tree_node * ret = this->m_node;
            this->m_node = this->m_node->GetNext();

            return const_iterator(ret);
        }

        const_iterator operator--()
        {
            tree_node * ret = this->m_node;
            this->m_node = this->m_node->GetPrevious();

            return const_iterator(ret);
        }

        ConstOutputValue operator*()
        {
            return ConstOutputValue(this->m_node->GetKey(), this->m_node->GetValue());
        }

        bool operator!=(const_iterator const & that) const
        {
            return this->m_node != that.m_node;
        }

    protected:

        tree_node * m_node;
    };

protected:

    tree_node * m_root;

    int m_count;
};

}
