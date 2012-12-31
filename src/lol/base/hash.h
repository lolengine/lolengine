//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Hash class
// --------------
// A very simple Hash class.
//

#if !defined __LOL_CORE_HASH_H__
#define __LOL_CORE_HASH_H__

namespace lol
{

template<typename T> class Hash;

template<> class Hash<int8_t>   { public: uint32_t operator()(int8_t) const; };
template<> class Hash<uint8_t>  { public: uint32_t operator()(uint8_t) const; };
template<> class Hash<int16_t>  { public: uint32_t operator()(int16_t) const; };
template<> class Hash<uint16_t> { public: uint32_t operator()(uint16_t) const; };
template<> class Hash<int32_t>  { public: uint32_t operator()(int32_t) const; };
template<> class Hash<uint32_t> { public: uint32_t operator()(uint32_t) const; };
template<> class Hash<int64_t>  { public: uint32_t operator()(int64_t) const; };
template<> class Hash<uint64_t> { public: uint32_t operator()(uint64_t) const; };

template<> class Hash<half>     { public: uint32_t operator()(half) const; };
template<> class Hash<float>    { public: uint32_t operator()(float) const; };
template<> class Hash<double>   { public: uint32_t operator()(double) const; };
template<> class Hash<ldouble>  { public: uint32_t operator()(ldouble) const; };

template<> class Hash<char const *>
{
public:
    uint32_t operator()(char const *x) const;
};

template<> class Hash<String>
{
public:
    uint32_t operator()(String const &s) const;
};

} /* namespace lol */

#endif // __LOL_CORE_HASH_H__

