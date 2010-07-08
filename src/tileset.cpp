
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
#include <SDL_image.h>

#include <malloc.h>

#include "tileset.h"

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    static int Compare(void const *p1, void const *p2)
    {
        int const *n1 = (int const *)p1;
        int const *n2 = (int const *)p2;

        return n1[2] + 32 * n1[3] - (n2[2] + 32 * n2[3]);
    }

    char *name;
    int ref;
    int *tiles;
    int ntiles;

    GLuint texture[1];
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path)
{
    SDL_Surface *img = NULL;

    data = new TileSetData();
    data->name = strdup(path);
    data->ref = 0;
    data->tiles = NULL;
    data->ntiles = 0;

    /* One tile texture */
    for (char const *name = path; *name; name++)
        if ((img = IMG_Load(name)))
            break;

    if (!img)
    {
        SDL_Quit();
        exit(1);
    }

    glGenTextures(1, data->texture);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, img->w, img->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

TileSet::~TileSet()
{
    glDeleteTextures(1, data->texture);

    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::Ref()
{
    data->ref++;
}

int TileSet::Unref()
{
    return --data->ref;
}

char const *TileSet::GetName()
{
    return data->name;
}

void TileSet::BlitTile(uint32_t id, int x, int y)
{
    float tx = .0625f * (id & 0xf);
    float ty = .0625f * ((id >> 4) & 0xf);

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);
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

