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
// The AndroidApp class
// --------------------
//

#if !defined __LOL_ANDROIDAPP_H__
#define __LOL_ANDROIDAPP_H__

#include "lol/math/vector.h"

namespace lol
{

class AndroidAppData;

class AndroidApp
{
public:
    AndroidApp(char const *title, ivec2 res, float fps);
    virtual ~AndroidApp();

    void ShowPointer(bool show);
    void Run();

private:
    AndroidAppData *data;
};

} /* namespace lol */

#endif // __LOL_ANDROIDAPP_H__

