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
// The SdlApp class
// ----------------
//

#if !defined __LOL_SDLAPP_H__
#define __LOL_SDLAPP_H__

#include "matrix.h"

namespace lol
{

class SdlAppData;

class SdlApp
{
public:
    SdlApp(char const *title, ivec2 res, float fps);
    virtual ~SdlApp();

    void Run();

private:
    SdlAppData *data;
};

} /* namespace lol */

#endif // __LOL_SDLAPP_H__

