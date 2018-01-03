//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
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
// Simple map utilities
// --------------------
//

#include <lol/base/array.h>

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
static inline array<typename T::key_type> keys(T const &m)
{
    array<typename T::key_type> ret;
    for (auto const &it : m)
        ret << it.first;
    return ret;
}

} /* namespace lol */

