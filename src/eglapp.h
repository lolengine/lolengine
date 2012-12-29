//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EglApp class
// ----------------
//

#if !defined __LOL_EGLAPP_H__
#define __LOL_EGLAPP_H__

#include "lol/math/vector.h"

namespace lol
{

class EglAppData;

class EglApp
{
public:
    EglApp(char const *title, ivec2 res, float fps);
    virtual ~EglApp();

    void ShowPointer(bool show);
    void Run();

private:
    EglAppData *data;
};

} /* namespace lol */

#endif // __LOL_EGLAPP_H__

