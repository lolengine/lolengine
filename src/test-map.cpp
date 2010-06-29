// Test stuff

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

#include <stdio.h>
#include <math.h>

#include "video.h"

/* Storage for one texture  */
GLuint texture[1];

/* Storage for 3 vertex buffers */
GLuint buflist[3];

/* Storage for map layers */
int *layers[128];
int width = 32, height = 32;
int nlayers = 0;

// Load Bitmaps And Convert To Textures
void LoadGLTextures(void)
{	
    SDL_Surface *image1 = IMG_Load("art/test/groundtest.png");

    if (!image1)
    {
        SDL_Quit();
        exit(1);
    }

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, image1->w, image1->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image1->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
};

void MakeVBOs(void)
{
    glGenBuffers(3, buflist);
}

void LoadMap(void)
{
    FILE *fp = popen("grep '^   ' maps/testmap.tmx | while read i; do echo -n \"$i\" | perl -MMIME::Base64 -ne 'print decode_base64($_)' | gunzip; done", "r");
    while (fp && !feof(fp))
    {
        layers[nlayers] = (int *)malloc(width * height * sizeof(int));
        fread(layers[nlayers], sizeof(int), width * height, fp);
        if (feof(fp))
        {
            free(layers[nlayers]);
            layers[nlayers] = 0;
            fclose(fp);
            break;
        }
        for (int n = 0; n < width * height; n++)
        {
            unsigned int i = layers[nlayers][n];
            //i = (i >> 24) | ((i >> 8) & 0xff00) | ((i << 8) & 0xff0000) | (i << 24);
            layers[nlayers][n] = i ? i - 1 : 0;
        }
        nlayers++;
    }
}

void PutMap(int const *themap, int width, int height)
{
    // Put map
    float uvs[8 * width * height];

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            int tile = themap[width * y + x];
            float ty = .0625f * (tile / 16);
            float tx = .0625f * (tile % 16);
            uvs[8 * (width * y + x) + 0] = tx;
            uvs[8 * (width * y + x) + 1] = ty;
            uvs[8 * (width * y + x) + 2] = tx + .0625f;
            uvs[8 * (width * y + x) + 3] = ty;
            uvs[8 * (width * y + x) + 4] = tx + .0625f;
            uvs[8 * (width * y + x) + 5] = ty + .0625f;
            uvs[8 * (width * y + x) + 6] = tx;
            uvs[8 * (width * y + x) + 7] = ty + .0625f;
        }
    glBindBuffer(GL_ARRAY_BUFFER, buflist[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 8 * width * height * sizeof(float), uvs, GL_STATIC_DRAW);

    float vertices[8 * width * height];
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            vertices[8 * (width * y + x) + 0] = x * 32;
            vertices[8 * (width * y + x) + 1] = y * 32;
            vertices[8 * (width * y + x) + 2] = x * 32 + 32;
            vertices[8 * (width * y + x) + 3] = y * 32;
            vertices[8 * (width * y + x) + 4] = x * 32 + 32;
            vertices[8 * (width * y + x) + 5] = y * 32 + 32;
            vertices[8 * (width * y + x) + 6] = x * 32;
            vertices[8 * (width * y + x) + 7] = y * 32 + 32;
        }
    glBindBuffer(GL_ARRAY_BUFFER, buflist[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 8 * width * height * sizeof(float), vertices, GL_STATIC_DRAW);

    int indices[4 * width * height];
    for (int n = 0; n < 4 * width * height; n++)
        indices[n] = n;
    glBindBuffer(GL_ARRAY_BUFFER, buflist[2]);
    glBufferData(GL_ARRAY_BUFFER,
                 4 * width * height * sizeof(int), indices, GL_STATIC_DRAW);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buflist[0]);
    glVertexPointer(2, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, buflist[1]);
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, buflist[2]);
    glIndexPointer(GL_INT, 0, NULL);

    glDrawArrays(GL_QUADS, 0, 4 * width * height);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
}

/* The main drawing function. */
void DrawScene()
{
/*
    int ground[20 * 15] =
    {
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
18,  1,  2,  2,  2, 34,  2,  2,  2,  2,  2,  2,  3, 34,  4, 18, 18, 18, 18, 18,
18, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 18, 20,  4, 18, 18, 18, 18,
18, 19, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 18, 17, 19, 18, 18, 18, 18,
18, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 17, 18, 17, 17, 18, 18, 18, 18,
18, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 18, 20, 36, 18, 18, 18, 18,
18, 33,  2,  2,  2,  2,  2,  2,  2,  2, 34,  2, 35,  2, 36, 18, 18, 18, 18, 18,
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    };

    int l1objects[20 * 15] =
    {
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 49,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0, 49, 49,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0, 49, 49, 49, 49, 49, 49,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0, 49, 49, 49, 49,  0,  0,  0,  0, 25,  9,  9,  9,  8,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 54, 40, 39, 39, 41,  0,  0,
 0,  0,  0, 49, 49, 32,  0, 50,  0,  0,  0, 48,  0, 24, 23, 54, 40, 55,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  0, 64,  0, 24, 23, 24, 23,  7,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6, 38, 24, 23,  7,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6, 38, 22,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
*/

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (int i = 0; i < nlayers; i++)
        PutMap(layers[i], width, height);
}

int main(int argc, char **argv)
{
    Video *video = new Video("Deus Hax", 640, 480);

    int done;

    /* Loop, drawing and checking events */
    LoadGLTextures();
    MakeVBOs();
    LoadMap();

    done = 0;
    while (!done)
    {
        DrawScene();

        video->Refresh(33.33333f);

        /* This could go in a separate function */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                done = 1;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                    video->FullScreen();
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = 1;
            }
        }
    }

    delete video;

    return EXIT_SUCCESS;
}

