//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Hash class
// --------------
//

#if !defined __LOL_HASH_H__
#define __LOL_HASH_H__

namespace lol
{

class Hash
{
public:
    static uint32_t Crc32(char const *data);
};

} /* namespace lol */

#endif // __LOL_HASH_H__

