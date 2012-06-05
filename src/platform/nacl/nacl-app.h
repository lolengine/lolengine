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
// The NaClApp class
// -----------------
//

#if !defined __LOL_NACLAPP_H__
#define __LOL_NACLAPP_H__

#include "lol/math/vector.h"

namespace lol
{

class NaClAppData;

class NaClApp
{
public:
    NaClApp(char const *title, ivec2 res, float fps);
    virtual ~NaClApp();

    void ShowPointer(bool show);
    void Run();

private:
    NaClAppData *data;
};

} /* namespace lol */

#endif // __LOL_NACLAPP_H__

