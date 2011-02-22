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
// The EglApp class
// ----------------
//

#if !defined __DH_EGLAPP_H__
#define __DH_EGLAPP_H__

#include "matrix.h"

namespace lol
{

class EglAppData;

class EglApp
{
public:
    EglApp(char const *title, vec2i res, float fps);
    virtual ~EglApp();

    void Run();

private:
    EglAppData *data;
};

} /* namespace lol */

#endif // __DH_EGLAPP_H__

