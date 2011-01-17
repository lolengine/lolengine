//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cmath>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "core.h"

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    char *name;
    int *tiles;
    int w, h, nw, nh, ntiles;
    float tx, ty;

    SDL_Surface *img;
    GLuint texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, int w, int h)
{
    data = new TileSetData();
    data->name = strdup(path);
    data->tiles = NULL;
    data->img = NULL;
    data->texture = 0;

    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
        SDL_Quit();
        exit(1);
    }

    if (w <= 0)
        w = 32;
    if (h <= 0)
        h = 32;

    data->w = w;
    data->h = h;
    /* FIXME: check for non-zero here */
    data->nw = data->img->w / w;
    data->nh = data->img->h / h;
    data->ntiles = data->nw * data->nh;
    data->tx = (float)w / data->img->w;
    data->ty = (float)h / data->img->h;

    drawgroup = DRAWGROUP_BEFORE;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (destroy)
    {
        if (data->img)
            SDL_FreeSurface(data->img);
        else
            glDeleteTextures(1, &data->texture);
    }
    else if (data->img)
    {
        GLuint format = data->img->format->Amask ? GL_RGBA : GL_RGB;
        int planes = data->img->format->Amask ? 4 : 3;

        glGenTextures(1, &data->texture);
        glBindTexture(GL_TEXTURE_2D, data->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, planes, data->img->w, data->img->h, 0,
                     format, GL_UNSIGNED_BYTE, data->img->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        SDL_FreeSurface(data->img);
        data->img = NULL;
    }
}

char const *TileSet::GetName()
{
    return data->name;
}

void TileSet::BlitTile(uint32_t id, int x, int y, int z, int o)
{
    float tx = data->tx * ((id & 0xffff) % data->nw);
    float ty = data->ty * ((id & 0xffff) / data->nw);

    float sqrt2 = sqrtf(2.0f);
    int off = o ? data->h : 0;
    int dx = data->w;
    int dy = data->h * 38 / 32; /* Magic... fix this one day */
    int dy2 = data->h * 70 / 32;

    if (!data->img)
    {
        glBindTexture(GL_TEXTURE_2D, data->texture);
        glBegin(GL_QUADS);
            glTexCoord2f(tx, ty);
            glVertex3f(x, sqrt2 * (y - dy - off), sqrt2 * (z + off));
            glTexCoord2f(tx + data->tx, ty);
            glVertex3f(x + dx, sqrt2 * (y - dy - off), sqrt2 * (z + off));
            glTexCoord2f(tx + data->tx, ty + data->ty);
            glVertex3f(x + dx, sqrt2 * (y - dy2), sqrt2 * z);
            glTexCoord2f(tx, ty + data->ty);
            glVertex3f(x, sqrt2 * (y - dy2), sqrt2 * z);
        glEnd();
    }
}

