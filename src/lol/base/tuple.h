//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The tuple class
// ---------------
// A very simple tuple class.
//

#include <lol/base/assert.h>

#include <tuple>

namespace lol
{

template<typename... T>
class tuple : public std::tuple<T...>
{
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7, typename T8>
class tuple<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7; T8 m8;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6, typename T7>
class tuple<T1, T2, T3, T4, T5, T6, T7>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6; T7 m7;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5,
         typename T6>
class tuple<T1, T2, T3, T4, T5, T6>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5; T6 m6;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class tuple<T1, T2, T3, T4, T5>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4; T5 m5;
};

template<typename T1, typename T2, typename T3, typename T4>
class tuple<T1, T2, T3, T4>
{
public:
    T1 m1; T2 m2; T3 m3; T4 m4;
};

template<typename T1, typename T2, typename T3>
class tuple<T1, T2, T3>
{
public:
    T1 m1; T2 m2; T3 m3;
};

template<typename T1, typename T2>
class tuple<T1, T2>
{
public:
    T1 m1; T2 m2;
};

template<typename T1>
class tuple<T1>
{
public:
    T1 m1;
};

} /* namespace lol */

