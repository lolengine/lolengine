// Test stuff

#include <SDL.h>

#include <stdio.h>
#include <math.h>

#include "video.h"
#include "tiler.h"

/* Global objects */
Video *video;
Tiler *tiler;

/* Storage for map layers */
int *layers[128];
int width = 32, height = 32;
int nlayers = 0;

void LoadMap(void)
{
    FILE *fp = popen("grep '^   [^< ]' maps/testmap.tmx | while read i; do echo -n \"$i\" | perl -MMIME::Base64 -ne 'print decode_base64($_)' | gunzip; done", "r");
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

/* The main drawing function. */
void DrawScene()
{
    video->Clear();

    for (int i = 0; i < nlayers; i++)
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                tiler->AddTile(layers[i][y * width + x], x * 32, y * 32, i);

    /* Test stuff */
    int playerx, playery;
    SDL_GetMouseState(&playerx, &playery);
    tiler->AddTile(50, playerx, playery, 1);
    tiler->AddTile(50, playerx + 64, playery + 32, 3);

    tiler->Render();
    video->Refresh(33.33333f);
}

int main(int argc, char **argv)
{
    video = new Video("Deus Hax", 640, 480);
    tiler = new Tiler();

    int done;

    /* Loop, drawing and checking events */
    LoadMap();

    done = 0;
    while (!done)
    {
        DrawScene();

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

    delete tiler;
    delete video;

    return EXIT_SUCCESS;
}

