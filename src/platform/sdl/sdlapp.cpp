//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#if USE_SDL || USE_OLD_SDL
#   if HAVE_SDL2_SDL_H
#      include <SDL2/SDL.h>
#   elif HAVE_SDL_SDL_H
#      include <SDL/SDL.h>
#   else
#      include <SDL.h>
#   endif
#   if USE_D3D9
#       include <d3d9.h>
#       include <SDL_syswm.h>
#   endif
#endif

#include "lolgl.h"
#include "platform/sdl/sdlapp.h"
#include "platform/sdl/sdlinput.h"
#if USE_XINPUT
#   include "platform/d3d9/d3d9input.h"
#endif

#if (USE_SDL || USE_OLD_SDL) && USE_D3D9
HWND g_hwnd = nullptr;
#endif

namespace lol
{

/*
* SDL App display implementation class
*/
class SdlAppDisplayData
{
    friend class SdlAppDisplay;

private:
#if USE_SDL
    SDL_Window *m_window;
    SDL_GLContext m_glcontext;
#elif USE_OLD_SDL
    SDL_Surface *m_window;
#endif
};

/*
* Public SdlApp class
*/
SdlAppDisplay::SdlAppDisplay(char const *title, ivec2 res)
    : data(new SdlAppDisplayData())
{
#if USE_SDL || USE_OLD_SDL
    ivec2 window_size = res;
    ivec2 screen_size = res;

    /* Initialise SDL */
    if (!SDL_WasInit(0))
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0)
        {
            msg::error("cannot initialise SDL: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

#if USE_SDL
    /* This seems to fix the swap context bug.
     * However, perfs warning have been may occur. */
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    data->m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_size.x, window_size.y,
        SDL_WINDOW_OPENGL);
    if (!data->m_window)
    {
        msg::error("cannot create rendering window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_GetWindowSize(data->m_window, &res.x, &res.y);

    data->m_glcontext = SDL_GL_CreateContext(data->m_window);
#else
    const SDL_VideoInfo* vidinfo = SDL_GetVideoInfo();
    screen_size = ivec2(vidinfo->current_w, vidinfo->current_h);

    SDL_WM_SetCaption(title, nullptr);

#   if USE_D3D9
    SDL_Surface *video = SDL_SetVideoMode(res.x, res.y, 16, 0);
    SDL_SysWMinfo wminfo;
    SDL_VERSION(&wminfo.version);
    SDL_GetWMInfo(&wminfo);
    g_hwnd = wminfo.window;
#   else
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    // TODO: when implementing fullscreen, be sure to overwrite screen_w
    // and screen_h with the value of vidinfo after the call to
    // SDL_SetVideoMode.
    data->m_window = SDL_SetVideoMode(res.x, res.y, 0, SDL_OPENGL);
#   endif

    if (!data->m_window)
    {
        msg::error("cannot create rendering window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    res = ivec2(data->m_window->w, data->m_window->h);
#endif

    /* Initialise everything */
    Video::Setup(res); //TODO ?? Should it be here ?
#endif
}

SdlAppDisplay::~SdlAppDisplay()
{
#if USE_SDL
    if (data->m_window)
    {
        SDL_GL_DeleteContext(data->m_glcontext);
        SDL_DestroyWindow(data->m_window);
    }
#elif USE_OLD_SDL
    if (data->m_window)
        SDL_FreeSurface(data->m_window);
#endif

    delete data;
}

void SdlAppDisplay::SetResolution(ivec2 resolution)
{
#if USE_SDL
    SDL_SetWindowSize(data->m_window, resolution.x, resolution.y);
#elif USE_OLD_SDL && USE_D3D9
    //Not implemented
#elif USE_OLD_SDL
    //Not implemented
#endif
}
void SdlAppDisplay::SetPosition(ivec2 position)
{
#if USE_SDL
    SDL_SetWindowPosition(data->m_window, position.x, position.y);
#elif USE_OLD_SDL && USE_D3D9
    //Not implemented
#elif USE_OLD_SDL
    //Not implemented
#endif
}

void SdlAppDisplay::Enable()
{
#if USE_SDL
    //TODO: Should we do that: ?
    SDL_GL_MakeCurrent(data->m_window, data->m_glcontext);
#endif
#if (USE_SDL || USE_OLD_SDL) && defined USE_D3D9
    IDirect3DDevice9 *d3d_dev = (IDirect3DDevice9 *)Renderer::Get()->GetDevice();
    HRESULT hr;
    hr = d3d_dev->BeginScene();
    if (FAILED(hr))
        Abort();
#endif
}

void SdlAppDisplay::Disable()
{
    #if USE_SDL
        SDL_GL_SwapWindow(data->m_window);
    #elif USE_OLD_SDL && USE_D3D9
        hr = d3d_dev->EndScene();
        if (FAILED(hr))
            Abort();
        hr = d3d_dev->Present(nullptr, nullptr, nullptr, nullptr);
        if (FAILED(hr))
            Abort();
    #elif USE_OLD_SDL
        SDL_GL_SwapBuffers();
    #endif
}

#if USE_SDL
int SceneDisplay::GetPhysicalCount()
{
    return SDL_GetNumVideoDisplays();
}

const char* SceneDisplay::GetPhysicalName(int index)
{
    return SDL_GetDisplayName(index);
}
#elif USE_OLD_SDL && USE_D3D9
//  Not implemented
#elif USE_OLD_SDL
//  Not implemented
#endif

/*
 * SDL App implementation class
 */
class SdlAppData
{
    friend class SdlApp;

private:
#if USE_SDL
    SDL_Window *m_window;
    SDL_GLContext m_glcontext;
#elif USE_OLD_SDL
    SDL_Surface *m_window;
#endif
};

/*
 * Public SdlApp class
 */
SdlApp::SdlApp(char const *title, ivec2 res, float fps) :
    data(new SdlAppData())
{
#if USE_SDL || USE_OLD_SDL
    ivec2 window_size = res;
    ivec2 screen_size = res;

    /* Initialise everything */
    Ticker::Setup(fps);
    Audio::Setup(2);

    /* Autoreleased objects */
#if defined USE_XINPUT
    /* Prefer D3d9 for joysticks on Windows, because the X360 pads are not
     * advertised with the proper number of axes. */
    new D3d9Input();
#endif

    new SdlInput(res.x, res.y, screen_size.x, screen_size.y);
#endif
}

void SdlApp::ShowPointer(bool show)
{
#if USE_SDL || USE_OLD_SDL
    SDL_ShowCursor(show ? 1 : 0);
#endif
}

void SdlApp::Tick()
{
    /* Tick the renderer, show the frame and clamp to desired framerate. */
    Ticker::TickDraw();
}

SdlApp::~SdlApp()
{
#if USE_SDL
    SDL_Quit();
#elif USE_OLD_SDL
    SDL_Quit();
#endif
    delete data;
}

} /* namespace lol */

