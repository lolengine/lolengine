//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Hash class
// --------------
// A very simple Hash class.
//

#if !defined __LOL_HASH_H__
#define __LOL_HASH_H__

namespace lol
{

template<typename T> class Hash;

template<> class Hash<int8_t>   { public: uint32_t operator()(int8_t); };
template<> class Hash<uint8_t>  { public: uint32_t operator()(uint8_t); };
template<> class Hash<int16_t>  { public: uint32_t operator()(int16_t); };
template<> class Hash<uint16_t> { public: uint32_t operator()(uint16_t); };
template<> class Hash<int32_t>  { public: uint32_t operator()(int32_t); };
template<> class Hash<uint32_t> { public: uint32_t operator()(uint32_t); };
template<> class Hash<int64_t>  { public: uint32_t operator()(int64_t); };
template<> class Hash<uint64_t> { public: uint32_t operator()(uint64_t); };

template<> class Hash<char const *>
{
public:
    uint32_t operator()(char const *x);
};

template<> class Hash<String>
{
public:
    uint32_t operator()(String const &s);
};

} /* namespace lol */

#endif // __LOL_HASH_H__

