//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The hash class
// --------------
// A very simple hash class.
//

#if !defined __LOL_BASE_HASH_H__
#define __LOL_BASE_HASH_H__

namespace lol
{

template<typename T> struct hash;

template<> struct hash<int8_t>   { uint32_t operator()(int8_t) const; };
template<> struct hash<uint8_t>  { uint32_t operator()(uint8_t) const; };
template<> struct hash<int16_t>  { uint32_t operator()(int16_t) const; };
template<> struct hash<uint16_t> { uint32_t operator()(uint16_t) const; };
template<> struct hash<int32_t>  { uint32_t operator()(int32_t) const; };
template<> struct hash<uint32_t> { uint32_t operator()(uint32_t) const; };
template<> struct hash<int64_t>  { uint32_t operator()(int64_t) const; };
template<> struct hash<uint64_t> { uint32_t operator()(uint64_t) const; };

template<> struct hash<half>     { uint32_t operator()(half) const; };
template<> struct hash<float>    { uint32_t operator()(float) const; };
template<> struct hash<double>   { uint32_t operator()(double) const; };
template<> struct hash<ldouble>  { uint32_t operator()(ldouble) const; };

template<> struct hash<char const *>
{
    uint32_t operator()(char const *x) const;
    uint32_t operator()(String const &s) const;
};

template<> struct hash<String>
{
    uint32_t operator()(String const &s) const;
    uint32_t operator()(char const *x) const;
};

} /* namespace lol */

#endif // __LOL_BASE_HASH_H__

