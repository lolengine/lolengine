//
// Lol Engine - Debug Quad test program
//
// Copyright: (c) 2005-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include "core.h"
#include "lolgl.h"
#include "loldebug.h"

using namespace std;
using namespace lol;

#if defined __CELLOS_LV2__
#   include "platform/ps3/ps3app.h"
#elif defined HAVE_GLES_2X
#   include "eglapp.h"
#else
#   include "platform/sdl/sdlapp.h"
#   include "platform/sdl/sdlinput.h"
#endif

#if defined _WIN32
#   undef main /* FIXME: still needed? */
#endif

int main(int argc, char **argv)
{
#if defined __CELLOS_LV2__
    Ps3App app("Quad", ivec2(640, 480), 60.0f);
#elif defined HAVE_GLES_2X
    EglApp app("Quad", ivec2(640, 480), 60.0f);
#else
    SdlApp app("Quad", ivec2(640, 480), 60.0f);
#endif

    /* Register an input driver and some debug stuff */
#if !defined HAVE_GLES_2X
    new SdlInput();
#endif
    //new DebugFps(20, 20);
    new DebugQuad();

    app.Run();

    return EXIT_SUCCESS;
}

