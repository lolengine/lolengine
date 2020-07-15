//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include "lolgl.h"

namespace lol::engine
{

bool has_opengl()
{
#if defined LOL_USE_GLEW || defined HAVE_GL_2X || defined HAVE_GLES_2X
    return true;
#else
    return false;
#endif
}

} // namespace lol::engine

