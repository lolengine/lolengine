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
 * Font implementation class
 */

class FontData
{
    friend class Font;

private:
    char *name;

    SDL_Surface *img;
    int width, height;
    float tx, ty;
    GLuint texture;
};

/*
 * Public Font class
 */

Font::Font(char const *path)
  : data(new FontData())
{
    data->name = strdup(path);
    data->img = NULL;

    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
        SDL_Quit();
        exit(1);
    }

    data->width = data->img->w / 16;
    data->height = data->img->h / 16;
    data->tx = 7.0f / PotUp(data->img->w);
    data->ty = 15.0f / PotUp(data->img->h);

    drawgroup = DRAWGROUP_BEFORE;
}

Font::~Font()
{
    delete data;
}

void Font::TickDraw(float deltams)
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

        int w = PotUp(data->img->w);
        int h = PotUp(data->img->h);

        uint8_t *pixels = (uint8_t *)data->img->pixels;
        if (w != data->img->w || h != data->img->h)
        {
            uint8_t *tmp = (uint8_t *)malloc(planes * w * h);
            for (int line = 0; line < data->img->h; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * data->img->w * line,
                       planes * data->img->w);
            pixels = tmp;
        }

        glGenTextures(1, &data->texture);
        glBindTexture(GL_TEXTURE_2D, data->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, planes, w, h, 0,
                     format, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (pixels != data->img->pixels)
            free(pixels);
        SDL_FreeSurface(data->img);
        data->img = NULL;
    }
}

char const *Font::GetName()
{
    return data->name;
}

void Font::Print(int x, int y, char const *str)
{
    if (data->img)
        return;

    glBindTexture(GL_TEXTURE_2D, data->texture);
    glBegin(GL_QUADS);
    while (*str)
    {
        uint32_t ch = (uint8_t)*str++;
        float tx = data->tx * (ch & 0xf);
        float ty = data->ty * ((ch >> 4) & 0xf);

        if (ch != ' ')
        {
            glTexCoord2f(tx, ty + data->ty);
            glVertex2f(x, y);
            glTexCoord2f(tx + data->tx, ty + data->ty);
            glVertex2f(x + data->width, y);
            glTexCoord2f(tx + data->tx, ty);
            glVertex2f(x + data->width, y + data->height);
            glTexCoord2f(tx, ty);
            glVertex2f(x, y + data->height);
        }

        x += data->width;
    }
    glEnd();
}

void Font::PrintBold(int x, int y, char const *str)
{
    static struct { int dx, dy; float r, g, b; } tab[] =
    {
        { -1,  0, 0.0, 0.0, 0.0 },
        {  0, -1, 0.0, 0.0, 0.0 },
        {  0,  1, 0.0, 0.0, 0.0 },
        {  1, -1, 0.0, 0.0, 0.0 },
        {  1,  1, 0.0, 0.0, 0.0 },
        {  2,  0, 0.0, 0.0, 0.0 },
        {  1,  0, 0.5, 0.5, 0.5 },
        {  0,  0, 1.0, 1.0, 1.0 },
    };

    if (data->img)
        return;

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
    for (unsigned int i = 0; i < sizeof(tab) / sizeof(*tab); i++)
    {
        glColor3f(tab[i].r, tab[i].g, tab[i].b);
        Print(x + tab[i].dx, y + tab[i].dy, str);
    }
    glPopAttrib();
}

