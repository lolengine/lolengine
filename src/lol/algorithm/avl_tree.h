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
        m_root(nullptr),
        m_count(0)
    {
    }

    avl_tree(avl_tree const & other) :
        m_root(nullptr),
        m_count(0)
    {
        for (auto iterator : other)
            this->insert(iterator.key, iterator.value);
    }

    avl_tree & operator=(avl_tree const & other)
    {
        if (this->m_root)
        {
            this->m_root->cascade_delete();
            delete this->m_root;
            this->m_root = nullptr;
        }

        this->m_count = other->m_count;

        for (auto iterator : other)
            this->insert(iterator.key, iterator.value);
    }

    ~avl_tree()
    {
        if (this->m_root)
        {
            this->m_root->cascade_delete();
            delete this->m_root;
        }
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
            this->m_root->cascade_delete();
            delete this->m_root;
            this->m_root = nullptr;
        }
    }

    bool try_get_value(K const & key, V * & value_ptr) const
    {
        if (this->m_root)
            return this->m_root->try_get_value(key, value_ptr);

        return false;
    }

    bool try_get_min(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * min_node = nullptr;

        if (this->m_root)
        {
            this->m_root->get_min(min_node);
            key_ptr = &min_node->get_key();
            value_ptr = &min_node->get_value();

            return true;
        }

        return false;
    }

    bool try_get_max(K const * & key_ptr, V * & value_ptr) const
    {
        tree_node * max_node = nullptr;

        if (this->m_root)
        {
            this->m_root->get_max(max_node);
            key_ptr = &max_node->get_key();
            value_ptr = &max_node->get_value();

            return true;
        }

        return false;
    }

    class Iterator;
    class ConstIterator;

    Iterator begin()
    {
        tree_node * node = nullptr;

        if (this->m_root)
            this->m_root->get_min(node);

        return Iterator(node);
    }

    ConstIterator begin() const
    {
        tree_node * node = nullptr;

        if (this->m_root)
            this->m_root->get_min(node);

        return ConstIterator(node);
    }

    int get_count() const
    {
        return this->m_count;
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator end() const
    {
        return ConstIterator(nullptr);
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
            return this->m_key;
        }

        V & get_value()
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

            bool created = false;

            if (this->m_child[i])
                created = this->m_child[i]->insert(key, value);
            else
            {
                created = true;

                this->m_child[i] = new tree_node(key, value, &this->m_child[i]);

                this->m_child[i]->m_chain[i] = this->m_chain[i];
                this->m_child[i]->m_chain[i ? 0 : 1] = this;

                if (this->m_chain[i])
                    this->m_chain[i]->m_chain[i ? 0 : 1] = this->m_child[i];
                this->m_chain[i] = this->m_child[i];
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
                delete this;
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

        bool exists(K const & key)
        {
            int i = -1 + (key < this->m_key) + 2 * (this->m_key < key);

            if (i < 0)
                return true;

            if (this->m_child[i])
                return this->m_child[i]->exists(key);

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

        void cascade_delete()
        {
            for (int i = 0 ; i < 2 ; ++i)
            {
                if (this->m_child[i])
                {
                    this->m_child[i]->cascade_delete();
                    delete this->m_child[i];
                }
            }
        }

        int get_balance() const
        {
            return this->m_stairs[1] - this->m_stairs[0];
        }

        tree_node * get_previous() const
        {
            return this->m_chain[0];
        }

        tree_node * get_next() const
        {
            return this->m_chain[1];
        }

    protected:

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

            this->replace_chain(replacement);

            this->m_parent_slot = nullptr;
            this->m_child[0] = nullptr;
            this->m_child[1] = nullptr;
            this->m_chain[0] = nullptr;
            this->m_chain[1] = nullptr;
        }

        void replace_chain(tree_node * replacement)
        {
            for (int i = 0 ; i < 2 ; ++i)
            {
                if (this->m_chain[i])
                {
                    this->m_chain[i]->m_chain[i ? 0 : 1] = replacement;
                    if (replacement)
                        replacement->m_chain[i ? 0 : 1] = this->m_chain[i];
                }
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

    class Iterator
    {
    public:

        Iterator(tree_node * node) :
            m_node(node)
        {
        }

        Iterator & operator++(int)
        {
            ASSERT(this->m_node);

            this->m_node = this->m_node->get_next();

            return *this;
        }

        Iterator & operator--(int)
        {
            ASSERT(this->m_node);

            this->m_node = this->m_node->get_previous();

            return *this;
        }

        Iterator operator++()
        {
            ASSERT(this->m_node);

            tree_node * ret = this->m_node;
            this->m_node = this->m_node->get_next();

            return Iterator(ret);
        }

        Iterator operator--()
        {
            ASSERT(this->m_node);

            tree_node * ret = this->m_node;
            this->m_node = this->m_node->get_previous();

            return Iterator(ret);
        }

        OutputValue operator*()
        {
            return OutputValue(this->m_node->get_key(), this->m_node->get_value());
        }

        bool operator!=(Iterator const & that) const
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

    class ConstIterator
    {
    public:

        ConstIterator(tree_node * node) :
            m_node(node)
        {
        }

        ConstIterator & operator++(int)
        {
            ASSERT(this->m_node);

            this->m_node = this->m_node->get_next();

            return *this;
        }

        ConstIterator & operator--(int)
        {
            ASSERT(this->m_node);

            this->m_node = this->m_node->get_previous();

            return *this;
        }

        ConstIterator operator++()
        {
            ASSERT(this->m_node);

            tree_node * ret = this->m_node;
            this->m_node = this->m_node->get_next();

            return ConstIterator(ret);
        }

        ConstIterator operator--()
        {
            ASSERT(this->m_node);

            tree_node * ret = this->m_node;
            this->m_node = this->m_node->get_previous();

            return ConstIterator(ret);
        }

        ConstOutputValue operator*()
        {
            return ConstOutputValue(this->m_node->get_key(), this->m_node->get_value());
        }

        bool operator!=(ConstIterator const & that) const
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
