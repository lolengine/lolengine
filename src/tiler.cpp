
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

#include "tiler.h"

/*
 * Tiler implementation class
 */

class TilerData
{
    friend class Tiler;

private:
    static int Compare(void const *p1, void const *p2)
    {
        int const *n1 = (int const *)p1;
        int const *n2 = (int const *)p2;

        return n1[2] + 32 * n1[3] - (n2[2] + 32 * n2[3]);
    }

    int *tiles;
    int ntiles;

    GLuint texture[1];
    GLuint buflist[3];
};

/*
 * Public Tiler class
 */

Tiler::Tiler()
{
    data = new TilerData();
    data->tiles = NULL;
    data->ntiles = 0;

    /* One tile texture */
    SDL_Surface *img = IMG_Load("art/test/grasstest.png");

    if (!img)
    {
        SDL_Quit();
        exit(1);
    }

    glGenTextures(1, &data->texture[0]);
    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, img->w, img->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /* Three GPU buffers */
    glGenBuffers(3, data->buflist);
}

Tiler::~Tiler()
{
    free(data->tiles);
    delete data;
}

void Tiler::AddTile(int n, int x, int y, int z)
{
    if ((data->ntiles % 1024) == 0)
    {
        data->tiles = (int *)realloc(data->tiles,
                                     (data->ntiles + 1024) * 4 * sizeof(int));
    }

    data->tiles[4 * data->ntiles] = n;
    data->tiles[4 * data->ntiles + 1] = x;
    data->tiles[4 * data->ntiles + 2] = y;
    data->tiles[4 * data->ntiles + 3] = z;

    data->ntiles++;
}

void Tiler::Render()
{
    /* Sort tiles */
    qsort(data->tiles, data->ntiles, 4 * sizeof(int), TilerData::Compare);

    /* Texture coord buffer */
    float uvs[8 * data->ntiles];
    for (int n = 0; n < data->ntiles; n++)
    {
        int tile = data->tiles[4 * n];
        float ty = .0625f * (tile / 16);
        float tx = .0625f * (tile % 16);
        uvs[8 * n + 0] = tx;
        uvs[8 * n + 1] = ty;
        uvs[8 * n + 2] = tx + .0625f;
        uvs[8 * n + 3] = ty;
        uvs[8 * n + 4] = tx + .0625f;
        uvs[8 * n + 5] = ty + .0625f;
        uvs[8 * n + 6] = tx;
        uvs[8 * n + 7] = ty + .0625f;
    }
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 8 * data->ntiles * sizeof(float), uvs, GL_STATIC_DRAW);

    /* Vertex buffer */
    float vertices[8 * data->ntiles];
    for (int n = 0; n < data->ntiles; n++)
    {
        int x = data->tiles[4 * n + 1];
        int y = data->tiles[4 * n + 2];
        vertices[8 * n + 0] = x;
        vertices[8 * n + 1] = y;
        vertices[8 * n + 2] = x + 32;
        vertices[8 * n + 3] = y;
        vertices[8 * n + 4] = x + 32;
        vertices[8 * n + 5] = y + 32;
        vertices[8 * n + 6] = x;
        vertices[8 * n + 7] = y + 32;
    }
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glBufferData(GL_ARRAY_BUFFER, 8 * data->ntiles * sizeof(float),
                 vertices, GL_STATIC_DRAW);

    /* Index buffer */
    int indices[4 * data->ntiles];
    for (int n = 0; n < 4 * data->ntiles; n++)
        indices[n] = n;
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[2]);
    glBufferData(GL_ARRAY_BUFFER, 4 * data->ntiles * sizeof(int),
                 indices, GL_STATIC_DRAW);

    /* Draw everything */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    glBindTexture(GL_TEXTURE_2D, data->texture[0]);

    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[0]);
    glVertexPointer(2, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[1]);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, data->buflist[2]);
    glIndexPointer(GL_INT, 0, NULL);

    glDrawArrays(GL_QUADS, 0, 4 * data->ntiles);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);

    /* Empty our shit */
    free(data->tiles);
    data->tiles = NULL;
    data->ntiles = 0;
}

