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

int main(int argc, char **argv)
{
    Application app("Quad", ivec2(640, 480), 60.0f);

    /* Register some debug stuff */
    new DebugFps(5, 5);
    //new DebugQuad();

    app.Run();

    return EXIT_SUCCESS;
}

