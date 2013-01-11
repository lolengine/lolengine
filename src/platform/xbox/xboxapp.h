//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The XboxApp class
// ----------------
//

#if !defined __LOL_XBOXAPP_H__
#define __LOL_XBOXAPP_H__

#include "lol/math/vector.h"

namespace lol
{

class XboxAppData;

class XboxApp
{
public:
    XboxApp(char const *title, ivec2 res, float fps);
    virtual ~XboxApp();

    void ShowPointer(bool show);
    void Run();

private:
    XboxAppData *data;
};

} /* namespace lol */

#endif // __LOL_XBOXAPP_H__

