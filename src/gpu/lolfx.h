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
// The LolFx class
// ---------------
//

#if !defined __GPU_LOLFX_H__
#define __GPU_LOLFX_H__

namespace lol
{

class LolFx
{
    friend class LolFxParser;

public:
    LolFx();

    bool Compile(char const *command);
};

} /* namespace lol */

#endif /* __GPU_LOLFX_H__ */

