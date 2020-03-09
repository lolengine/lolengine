//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Guillaume Bittoun <guillaume.bittoun@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The map utilities
// —————————————————
// Contains some utilities to work with std::map and std::vector objects.
//

#include <vector>
#include <map>

namespace lol
{

template <typename T>
static inline bool has_key(T const &m, typename T::key_type const &key)
{
    return m.count(key) > 0;
}

template <typename T>
static inline bool try_get(T const &m, typename T::key_type const &key,
                           typename T::mapped_type &val)
{
    auto const &kv = m.find(key);
    if (kv == m.end())
        return false;
    val = kv->second;
    return true;
}

template <typename T>
static inline std::vector<typename T::key_type> keys(T const &m)
{
    std::vector<typename T::key_type> ret;
    for (auto const &it : m)
        ret.push_back(it.first);
    return ret;
}

template <typename T>
static inline auto insert_at(std::vector<T> &v, size_t i, T const &x)
{
    return v.insert(v.begin() + i, x);
}

template <typename T>
static inline auto remove_at(std::vector<T> &v, size_t i)
{
    return v.erase(v.begin() + i);
}

template <typename T>
static inline auto remove_item(std::vector<T> &v, T const &x)
{
    return v.erase(std::remove(v.begin(), v.end(), x), v.end());
}

} /* namespace lol */

