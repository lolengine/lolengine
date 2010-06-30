
#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#   include <GL/glext.h>
#endif

#include <SDL.h>

#include "video.h"

/*
 * Video implementation class
 */

class VideoData
{
    friend class Video;

private:
    SDL_Surface *video;
    Uint32 start, ticks;
    int frames;
};

/*
 * Public Video class
 */

Video::Video(char const *title, int width, int height)
{
    data = new VideoData();

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

    /* Initialise OpenGL */
    glViewport(0, 0, data->video->w, data->video->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, data->video->w, data->video->h, 0, -1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Initialise timer */
    data->start = data->ticks = SDL_GetTicks();
    data->frames = 0;
}

int Video::GetWidth() const
{
    return data->video->w;
}

int Video::GetHeight() const
{
    return data->video->h;
}

void Video::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void Video::Refresh(float milliseconds)
{
    if (milliseconds > 0.0f)
        while (SDL_GetTicks() < data->ticks + (milliseconds - 0.5f))
            SDL_Delay(1);
    data->ticks = SDL_GetTicks();
    data->frames++;

    SDL_GL_SwapBuffers();
}

void Video::FullScreen()
{
    SDL_WM_ToggleFullScreen(data->video);
}

Video::~Video()
{
    Uint32 total = SDL_GetTicks() - data->start;
    printf("%f fps\n", 1000.0f * data->frames / total);
    SDL_Quit();
    delete data;
}

