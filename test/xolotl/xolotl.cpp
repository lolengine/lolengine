//
// Lol Engine - Xolotl algorithm test
//
// Copyright: (c) 2011 Soren Renner
//            (c) 2011-2012 Sam Hocevar <sam@hocevar.net>
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

using namespace lol;

#include "xolotl.h"

class Xolotl : public WorldEntity
{
public:
    Xolotl()
    {
        for(int i = 0; i < 2000; i++) {
          float x = (i - 1000.0) / 100.0;
          graph[i].x = x;
          graph[i].y = lol::sin(x * 10.0) / (1.0 + x * x);
        }
    }

    virtual ~Xolotl()
    {

    }

    virtual char const *GetName() { return "Xolotl"; }

    virtual void TickGame(float seconds)
    {

    }

    virtual void TickDraw(float seconds)
    {

    }

private:
    vec2 graph[2000];
};

int main(int argc, char **argv)
{
    Application app("Xolotl", ivec2(640, 480), 60.0f);

    /* Register some debug stuff */
    new DebugFps(5, 5);

    Cell<3> c;
    app.Run();

    return EXIT_SUCCESS;
}

