//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
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
    int nw, nh, ntiles;
    float tx, ty;

    SDL_Surface *img;
    GLuint texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path)
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

    data->nw = data->img->w / 32;
    data->nh = data->img->h / 32;
    data->ntiles = data->nw * data->nh;
    data->tx = 32.0f / data->img->w;
    data->ty = 32.0f / data->img->h;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

Entity::Group TileSet::GetGroup()
{
    return GROUP_BEFORE;
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
        glGenTextures(1, &data->texture);
        glBindTexture(GL_TEXTURE_2D, data->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, 4, data->img->w, data->img->h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, data->img->pixels);

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
    int off = o ? 32 : 0;

    if (!data->img)
    {
        glBindTexture(GL_TEXTURE_2D, data->texture);
        glBegin(GL_QUADS);
            glTexCoord2f(tx, ty);
            glVertex3f(x, sqrt2 * (y - 38 - off), sqrt2 * (z + off));
            glTexCoord2f(tx + data->tx, ty);
            glVertex3f(x + 32, sqrt2 * (y - 38 - off), sqrt2 * (z + off));
            glTexCoord2f(tx + data->tx, ty + data->ty);
            glVertex3f(x + 32, sqrt2 * (y - 70), sqrt2 * z);
            glTexCoord2f(tx, ty + data->ty);
            glVertex3f(x, sqrt2 * (y - 70), sqrt2 * z);
        glEnd();
    }
}

