//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <SDL.h>

#include "sdlvideo.h"
#include "video.h"

/*
 * SDL Video implementation class
 */

class SdlVideoData
{
    friend class SdlVideo;

private:
    SDL_Surface *video;
    Uint32 start, ticks;
    int frames;
};

/*
 * Public SdlVideo class
 */

SdlVideo::SdlVideo(char const *title, int width, int height)
{
    data = new SdlVideoData();

    /* Initialise SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Cannot initialise SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    data->video = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
    if (!data->video)
    {
        fprintf(stderr, "Cannot create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption(title, NULL);
    SDL_ShowCursor(0);
    SDL_WM_GrabInput(SDL_GRAB_ON);

    Video::Setup(data->video->w, data->video->h);

    /* Initialise timer */
    data->start = data->ticks = SDL_GetTicks();
    data->frames = 0;
}

int SdlVideo::GetWidth() const
{
    return data->video->w;
}

int SdlVideo::GetHeight() const
{
    return data->video->h;
}

void SdlVideo::PreRender()
{
    Video::Clear();
}

void SdlVideo::PostRender(float milliseconds)
{
    if (milliseconds > 0.0f)
        while (SDL_GetTicks() < data->ticks + (milliseconds - 0.5f))
            SDL_Delay(1);
    data->ticks = SDL_GetTicks();
    data->frames++;

    SDL_GL_SwapBuffers();
}

void SdlVideo::FullScreen()
{
    SDL_WM_ToggleFullScreen(data->video);
}

SdlVideo::~SdlVideo()
{
    Uint32 total = SDL_GetTicks() - data->start;
    printf("%f fps\n", 1000.0f * data->frames / total);
    SDL_Quit();
    delete data;
}

