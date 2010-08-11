//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

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

#include "tileset.h"

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    char *name;
    int *tiles;
    int ntiles;

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
    data->ntiles = 0;
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
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickRender(float delta_time)
{
    Asset::TickRender(delta_time);

    if (data->img)
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
    else if (ref == 0)
    {
        glDeleteTextures(1, &data->texture);
        destroy = 1;
    }
}

char const *TileSet::GetName()
{
    return data->name;
}

void TileSet::BlitTile(uint32_t id, int x, int y)
{
    float tx = .0625f * (id & 0xf);
    float ty = .0625f * ((id >> 4) & 0xf);

    if (!data->img)
    {
        glBindTexture(GL_TEXTURE_2D, data->texture);
        glBegin(GL_QUADS);
            glTexCoord2f(tx, ty);
            glVertex2f(x, y);
            glTexCoord2f(tx + .0625f, ty);
            glVertex2f(x + 32, y);
            glTexCoord2f(tx + .0625f, ty + .0625f);
            glVertex2f(x + 32, y + 32);
            glTexCoord2f(tx, ty + .0625f);
            glVertex2f(x, y + 32);
        glEnd();
    }
}

