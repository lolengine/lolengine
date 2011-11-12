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
// The Ps3App class
// ----------------
//

#if !defined __LOL_PS3APP_H__
#define __LOL_PS3APP_H__

#include "matrix.h"

namespace lol
{

class Ps3AppData;

class Ps3App
{
public:
    Ps3App(char const *title, ivec2 res, float fps);
    virtual ~Ps3App();

    void ShowPointer(bool show);
    void Run();

private:
    Ps3AppData *data;
};

} /* namespace lol */

#endif // __LOL_PS3APP_H__

