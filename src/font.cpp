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
        data->width = data->img->w / 16;
        data->height = data->img->h / 16;

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
        float tx = .0625f * (ch & 0xf);
        float ty = .0625f * ((ch >> 4) & 0xf);

        if (ch != ' ')
        {
            glTexCoord2f(tx, ty + .0625f);
            glVertex2f(x, y);
            glTexCoord2f(tx + .0625f, ty + .0625f);
            glVertex2f(x + data->width, y);
            glTexCoord2f(tx + .0625f, ty);
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

