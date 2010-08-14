//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include <SDL.h>

#include "sdlvideo.h"
#include "sdlinput.h"
#include "game.h"
#include "ticker.h"

int main(int argc, char **argv)
{
    SdlVideo *video = new SdlVideo("Deus Hax", 640, 480);
    Game *game = new Game("maps/testmap.tmx");

    /* Register the input driver */
    new SdlInput(game);

    while (!game->Finished())
    {
        Ticker::TickGame(33.33333f);

        video->PreRender();
        Ticker::TickRender(33.33333f);
        video->PostRender(33.33333f);
    }

    delete video;

    return EXIT_SUCCESS;
}

